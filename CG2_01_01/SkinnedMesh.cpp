#include "SkinnedMesh.h"
#include <sstream>
#include <functional>
#include <filesystem>
#include <memory>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "Texture.h"
#include "Camera.h"

// ボーン影響度
struct BoneInfluence {
	uint32_t bone_index;
	float bone_weight;
};
using BoneInfluencesPerControlPoint = std::vector<BoneInfluence>;

void FetchBoneInfluences(const FbxMesh *fbx_mesh, std::vector<BoneInfluencesPerControlPoint> &bone_influences) {
	const int control_points_count = fbx_mesh->GetControlPointsCount();
	bone_influences.resize(control_points_count);

	const int skin_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int skin_index = 0; skin_index < skin_count; ++skin_index) {
		const FbxSkin *fbx_skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
		const int cluster_count = fbx_skin->GetClusterCount();

		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index) {
			const FbxCluster *fbx_cluster = fbx_skin->GetCluster(cluster_index);

			const int control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();

			for (int control_point_indices_index = 0; control_point_indices_index < control_point_indices_count; ++control_point_indices_index) {
				int control_point_index{ fbx_cluster->GetControlPointIndices()[control_point_indices_index] };
				double control_point_weight = fbx_cluster->GetControlPointWeights()[control_point_indices_index];
				BoneInfluence & bone_influence = bone_influences.at(control_point_index).emplace_back();
				bone_influence.bone_index = static_cast<uint32_t>(cluster_index);
				bone_influence.bone_weight = static_cast<float>(control_point_weight);
			}
		}
	}
}

inline XMFLOAT4X4 SkinnedMesh::ConvertXMFLOAT4X4FromFbx(const FbxMatrix &fbx_matrix)
{
	XMFLOAT4X4 xmfloat4x4;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			xmfloat4x4.m[row][column] = static_cast<float>(fbx_matrix[row][column]);
		}
	}
	return xmfloat4x4;
}

inline XMFLOAT3 SkinnedMesh::ConvertXMFLOAT3FromFbx(const FbxDouble3 &fbx_double3)
{
	XMFLOAT3 xmfloat3;
	xmfloat3.x = static_cast<float>(fbx_double3[0]);
	xmfloat3.y = static_cast<float>(fbx_double3[1]);
	xmfloat3.z = static_cast<float>(fbx_double3[2]);
	return xmfloat3;
}

inline XMFLOAT4 SkinnedMesh::ConvertXMFLOAT4FromFbx(const FbxDouble4 &fbx_double4)
{
	XMFLOAT4 xmfloat4;
	xmfloat4.x = static_cast<float>(fbx_double4[0]);
	xmfloat4.y = static_cast<float>(fbx_double4[1]);
	xmfloat4.z = static_cast<float>(fbx_double4[2]);
	xmfloat4.w = static_cast<float>(fbx_double4[3]);
	return xmfloat4;
}

SkinnedMesh::SkinnedMesh(ID3D12Device *dev, const char *fileName, bool trianglate)
{
	// マネージャーの生成
	FbxManager *fbx_manager = FbxManager::Create();
	// シーン生成
	FbxScene *fbx_scene = FbxScene::Create(fbx_manager, "MyScene");


	FbxImporter *fbxImporter = FbxImporter::Create(fbx_manager, "");
	// ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(fileName, -1, fbx_manager->GetIOSettings()))
	{
		assert(0);
	}

	// インポート
	if (!fbxImporter->Import(fbx_scene)) {
		assert(0);
	}

	
	// 座標系の統一
	FbxAxisSystem scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
	if (scene_axis_system != FbxAxisSystem::DirectX) {
		FbxAxisSystem::DirectX.DeepConvertScene(fbx_scene);
	}

	// 三角形化
	FbxGeometryConverter fbx_converter(fbx_manager);
	if (trianglate) {
		fbx_converter.Triangulate(fbx_scene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
	}

	// メートルに変換
	FbxSystemUnit scene_unit = fbx_scene->GetGlobalSettings().GetSystemUnit();
	scale_factor_ = scene_unit.GetScaleFactor();
	if (scene_unit != FbxSystemUnit::m) {
		//FbxSystemUnit::m.ConvertScene(fbx_scene);
	//	FbxSystemUnit::m.ConvertScene(fbx_scene);
	}

	//ノードの取得
	std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
		Scene::Node &node = scene_view_.nodes.emplace_back();
		// typeがnullだったらeUnknownをセット
		node.atribute = fbxNode->GetNodeAttribute() ?
			fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
		node.name = fbxNode->GetName();
		node.uniqueID = fbxNode->GetUniqueID();
		// 親がいなければ0を代入
		node.pearent_index = scene_view_.indexof(fbxNode->GetParent() ?
			fbxNode->GetParent()->GetUniqueID() : 0);
		// 子に対して再起処理
		for (int child_index = 0; child_index < fbxNode->GetChildCount(); ++child_index) {
			traverse(fbxNode->GetChild(child_index));
		}
	} };
	
	// ノード取得
	traverse(fbx_scene->GetRootNode());
	// メッシュ情報を取得
	FetchMeshes(fbx_scene, meshes);
	// マテリアル情報を取得
	FetchMaterial(fbx_scene, materials);
	// アニメーションを取得
	FetchAnimations(fbx_scene, animation_clips_);

	// 解放
	fbx_manager->Destroy();
	// オブジェクト生成
	CreateComObjects(dev, fileName);
}

void SkinnedMesh::FetchMeshes(FbxScene *fbxScene, std::vector<Mesh> &meshes)
{
	// すべてのノードをチェック
	for (const Scene::Node &node : scene_view_.nodes) {
		// メッシュでなければスキップ
		if (node.atribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}
		// ノードとメッシュを抜き出し
		FbxNode *fbx_node = fbxScene->FindNodeByName(node.name.c_str());
		FbxMesh *fbx_mesh = fbx_node->GetMesh();
		
		// メッシュ構造体に当てはめる
		Mesh &mesh = meshes.emplace_back();
		mesh.unique_id = fbx_mesh->GetNode()->GetUniqueID();
		mesh.name = fbx_mesh->GetNode()->GetName();
		mesh.node_index = scene_view_.indexof(mesh.unique_id);
		
		mesh.default_grlobal_transform = ConvertXMFLOAT4X4FromFbx(fbx_mesh->GetNode()->EvaluateGlobalTransform());

		// ボーン影響度の獲得
		std::vector<BoneInfluencesPerControlPoint> bone_influences;
		FetchBoneInfluences(fbx_mesh, bone_influences);
		// バインドポーズの取得
		FetchSkelton(fbx_mesh, mesh.bind_pose);

		// コンテナの取得
		std::vector<Mesh::Subset> &subsets = mesh.subsets;
		// マテリアルカウント
		const int material_count = fbx_mesh->GetNode()->GetMaterialCount();
		// あらかじめ領域を確保（最低1）
		subsets.resize(material_count > 0 ? material_count : 1);
		// 全てのマテリアルidと名前を取得
		for (int material_index = 0; material_index < material_count; ++material_index) {
			const FbxSurfaceMaterial *fbx_material = fbx_mesh->GetNode()->GetMaterial(material_index);
			subsets.at(material_index).material_name = fbx_material->GetName();
			subsets.at(material_index).material_unique_id = fbx_material->GetUniqueID();
		}

		// ポリゴン数の取得
		const int polygon_count = fbx_mesh->GetPolygonCount();

		// マテリアルが設定されていればポリゴンごとにマテリアルを設定する
		if (material_count > 0) {
			for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index) {
				const int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index);
				subsets.at(material_index).index_count += 3;
			}
			uint32_t offset = 0;
			for (Mesh::Subset &subset : subsets) {
				subset.start_index = offset;
				offset += subset.index_count;
				subset.index_count = 0;
			}
		}

		// ポリゴンの数から頂点とインデックスをあらかじめ確保
		mesh.vertices.resize(polygon_count * 3LL);
		mesh.indices.resize(polygon_count * 3LL);

		// UVを取得
		FbxStringList uv_names;
		fbx_mesh->GetUVSetNames(uv_names);
		// コントロールポイントの取得
		const FbxVector4 *control_points = fbx_mesh->GetControlPoints();
		for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index) {
			const int material_index =
				material_count > 0 ?
				fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) :
				0;
			Mesh::Subset &subset = subsets.at(material_index);
			const uint32_t offset = subset.start_index + subset.index_count;

			for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon) {
				// 頂点のインデックス
				const int vertex_index = polygon_index * 3 + position_in_polygon;

				// 頂点
				Vertex vertex;
				const int polygon_vertex = fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon);
				vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
				vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
				vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);

				// 4つまで影響受ける
				const BoneInfluencesPerControlPoint &influences_per_control_point = bone_influences.at(polygon_vertex);
				for (size_t influence_index = 0; influence_index < influences_per_control_point.size(); ++influence_index) {
					if (influence_index < MAX_BONE_INFLUENCES) {
						vertex.bone_weights[influence_index] = influences_per_control_point.at(influence_index).bone_weight;
						vertex.bone_indices[influence_index] = influences_per_control_point.at(influence_index).bone_index;

					}
				}


				// 法線があればコピー
				if (fbx_mesh->GetElementNormalCount() > 0) {
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				// UVがあればコピー
				if (fbx_mesh->GetElementUVCount() > 0) {
					FbxVector2 uv;
					bool unmapped_uv;
					fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				mesh.vertices.at(vertex_index) = std::move(vertex);
				mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
				subset.index_count++;
			}
		}

	}

}

void SkinnedMesh::FetchMaterial(FbxScene *fbx_scene, std::unordered_map<uint64_t, Material> &materials)
{
	// ノードの数
	const size_t node_count = scene_view_.nodes.size();
	CreateDummyMaterial(materials);
	for (int node_index = 0; node_index < node_count; ++node_index) {
		const Scene::Node &node = scene_view_.nodes.at(node_index);
		const FbxNode *fbx_node = fbx_scene->FindNodeByName(node.name.c_str());
		
		const int material_count = fbx_node->GetMaterialCount();
		for (int material_index = 0; material_index < material_count; ++material_index) {
			// マテリアル取得
			const FbxSurfaceMaterial *fbx_material = fbx_node->GetMaterial(material_index);

			Material material;
			material.name = fbx_material->GetName();
			material.unique_id = fbx_material->GetUniqueID();

			// あとでADS全部とれるようにする
			// デュフューズ
			FbxProperty fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (fbx_property.IsValid()) {

				const FbxDouble3 diffuse = fbx_property.Get<FbxDouble3>();
				material.Kd.x = static_cast<float>(diffuse[0]);
				material.Kd.y = static_cast<float>(diffuse[1]);
				material.Kd.z = static_cast<float>(diffuse[2]);
				material.Kd.w = 1.0f;

				const FbxFileTexture *fbx_texture = fbx_property.GetSrcObject<FbxFileTexture>();
				material.texture_filenames[0] =
					fbx_texture ? fbx_texture->GetRelativeFileName() : "";
			}

			materials.emplace(material.unique_id, std::move(material));
		}
	}
	/*if (materials.size() == 0) {
		CreateDummyMaterial(materials);
	}*/
}

void SkinnedMesh::FetchSkelton(FbxMesh *fbx_mesh, Skeleton &bind_pose)
{
	const int deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformer_index = 0; deformer_index < deformer_count; ++deformer_index) {
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
		const int cluster_count = skin->GetClusterCount();
		bind_pose.bones.resize(cluster_count);

		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index) {
			FbxCluster *cluster = skin->GetCluster(cluster_index);

			Skeleton::Bone &bone = bind_pose.bones.at(cluster_index);
			bone.name = cluster->GetLink()->GetName();
			bone.unique_id = cluster->GetLink()->GetUniqueID();
			bone.parent_index = bind_pose.indexof(cluster->GetLink()->GetParent()->GetUniqueID());
			bone.node_index = scene_view_.indexof(bone.unique_id);

			// 初期ポーズ
			FbxAMatrix reference_global_init_position;
			cluster->GetTransformAssociateModelMatrix(reference_global_init_position);

			FbxAMatrix cluster_globar_init_position;
			cluster->GetTransformLinkMatrix(cluster_globar_init_position);

			// offsetの取得
			bone.offset_transform =
				ConvertXMFLOAT4X4FromFbx(cluster_globar_init_position.Inverse() * reference_global_init_position);
		}
	}
}


void SkinnedMesh::FetchAnimations(FbxScene *fbx_scene, std::vector<Animation> &animation_clips, float sampling_rate)
{
	// アニメーションコンテナ
	FbxArray<FbxString *> animation_stack_names;
	// アニメーションの名前を格納
	fbx_scene->FillAnimStackNameArray(animation_stack_names);
	const int animation_stack_count = animation_stack_names.GetCount();
	for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index) {
		Animation &animation_clip = animation_clips.emplace_back();
		animation_clip.name = animation_stack_names[animation_stack_index]->Buffer();

		FbxAnimStack *animation_stack = fbx_scene->FindMember<FbxAnimStack>(animation_clip.name.c_str());
		fbx_scene->SetCurrentAnimationStack(animation_stack);

		// FBXのキーフレーム単位を変換
		const FbxTime::EMode time_mode = fbx_scene->GetGlobalSettings().GetTimeMode();
		FbxTime one_second;
		one_second.SetTime(0, 0, 1, 0, 0, time_mode);
		animation_clip.sampling_rate = sampling_rate > 0 ?
			sampling_rate : static_cast<float>(one_second.GetFrameRate(time_mode));
		
		// サンプリング間隔を設定
		const FbxTime sampling_interval = static_cast<FbxLongLong>(one_second.Get() / animation_clip.sampling_rate);
		const FbxTakeInfo *take_info = fbx_scene->GetTakeInfo(animation_clip.name.c_str());
		const FbxTime start_time = take_info->mLocalTimeSpan.GetStart();
		const FbxTime stop_time = take_info->mLocalTimeSpan.GetStop();
		
		// キーフレームの取得
		for (FbxTime time = start_time; time < stop_time; time += sampling_interval) {
			Animation::Keyframe &keyframe = animation_clip.sequence.emplace_back();

			const size_t node_count = scene_view_.nodes.size();
			keyframe.nodes.resize(node_count);
			for (size_t node_index = 0; node_index < node_count; ++node_index) {
				FbxNode *fbx_node = fbx_scene->FindNodeByName(scene_view_.nodes.at(node_index).name.c_str());
				if (fbx_node) {
					Animation::Keyframe::Node &node = keyframe.nodes.at(node_index);
					// グローバルトランスフォームの取得
					node.global_transform = ConvertXMFLOAT4X4FromFbx(fbx_node->EvaluateGlobalTransform(time));

					const FbxAMatrix &local_transform = fbx_node->EvaluateLocalTransform(time);
					node.scaling = ConvertXMFLOAT3FromFbx(local_transform.GetS());
					node.rotation = ConvertXMFLOAT4FromFbx(local_transform.GetQ());
					node.translation = ConvertXMFLOAT3FromFbx(local_transform.GetT());
				}
			}
		}
	}
	for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index) {
		delete animation_stack_names[animation_stack_index];
	}
}

void SkinnedMesh::UpdateAnimation(Animation::Keyframe &keyframe)
{
	size_t node_count = keyframe.nodes.size();
	for (size_t node_index = 0; node_index < node_count; ++node_index) {
		Animation::Keyframe::Node &node{ keyframe.nodes.at(node_index) };
		XMMATRIX S = XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z);
		XMMATRIX R = XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation));
		XMMATRIX T = XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z);
		
		int64_t parent_index = scene_view_.nodes.at(node_index).pearent_index;
		XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() :
			XMLoadFloat4x4(&keyframe.nodes.at(parent_index).global_transform) };
		
		XMStoreFloat4x4(&node.global_transform, S * R * T * P);
	}
}

bool SkinnedMesh::AppendAnimations(const char *animation_filename, float sampling_rate)
{
}

void SkinnedMesh::CreateComObjects(ID3D12Device *dev, const char *fileName)
{
	//メッシュの数処理を繰り返す
	for (Mesh &mesh : meshes) {
		HRESULT result;

		// 頂点データ全体のサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
		// 頂点バッファの生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.vertexBuffer));
		// 頂点バッファへのデータ転送
		Vertex *vertMap = nullptr;
		result = mesh.vertexBuffer->Map(0, nullptr, (void **)&vertMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.vertices.begin(), mesh.vertices.end(), vertMap);
			mesh.vertexBuffer->Unmap(0, nullptr);
		}
		// 頂点バッファビュー(VBV)の作成
		mesh.vbView.BufferLocation = mesh.vertexBuffer->GetGPUVirtualAddress();
		mesh.vbView.SizeInBytes = sizeVB;
		mesh.vbView.StrideInBytes = sizeof(mesh.vertices[0]);


		// 頂点インデックス全体のサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh.indices.size());
		// インデックスバッファ生成
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.indexBuffer));
		// インデックスバッファへのデータ転送
		unsigned short *indexMap = nullptr;
		result = mesh.indexBuffer->Map(0, nullptr, (void **)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(mesh.indices.begin(), mesh.indices.end(), indexMap);
			mesh.indexBuffer->Unmap(0, nullptr);
		}
		// インデックスバッファビュー(IBV)の作成
		mesh.ibView.BufferLocation = mesh.indexBuffer->GetGPUVirtualAddress();
		mesh.ibView.Format = DXGI_FORMAT_R16_UINT;
		mesh.ibView.SizeInBytes = sizeIB;

		for (Mesh::Subset &subset : mesh.subsets) {
			// メッシュ定数バッファの生成
			result = dev->CreateCommittedResource
			(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MeshConstantBuffer) + 0xff) & ~0xff),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&subset.mesh_constant_buffer_));
		}
	}


	//パイプライン生成処理
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; //頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    //ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト


	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile
	(
		L"SkinnedMeshVS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob
	);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile
	(
		L"SkinnedMeshPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob
	);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ // xy座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // ボーン影響度
			"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // ボーン
			"BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ(テクスチャを四枚まで)
	CD3DX12_DESCRIPTOR_RANGE desc_range_srv[4] = {};
	desc_range_srv[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
	desc_range_srv[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 レジスタ
	desc_range_srv[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2); // t2 レジスタ
	desc_range_srv[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3); // t3 レジスタ


	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// CBV（シーンバッファ）
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[2].InitAsDescriptorTable(_countof(desc_range_srv), desc_range_srv, D3D12_SHADER_VISIBILITY_ALL);
	////CBV(スキニング用)
	//rootparams[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	//// CBV（マテリアル用）
	//rootparams[3].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	//// CBV（ライト用）
	//rootparams[4].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc[3] = { 
		CD3DX12_STATIC_SAMPLER_DESC(0,D3D12_FILTER_MIN_MAG_MIP_LINEAR),
		CD3DX12_STATIC_SAMPLER_DESC(1,D3D12_FILTER_MIN_MAG_MIP_LINEAR),
		CD3DX12_STATIC_SAMPLER_DESC(2,D3D12_FILTER_MIN_MAG_MIP_LINEAR)
	};

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, _countof(sampler_desc), sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature_.Get();

	// グラフィックスパイプラインの生成
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	// シーン定数バッファの生成
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneConstantBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&scene_constant_buffer_)
	);


	// シェーダーリソースビューの生成
	for (std::unordered_map<uint64_t, Material>::iterator itr = materials.begin(); itr != materials.end(); ++itr) {
		// テクスチャが指定されていれば設定
		if (itr->second.texture_filenames[0].size() > 0) {
			// ファイル名保存
			std::filesystem::path path = fileName;
			path.replace_filename(itr->second.texture_filenames[0]);

			// テクスチャ読み込み
			itr->second.shader_resource_views[0] = Texture::LoadTextureFromFile(dev, path.c_str());
			
		}
		// テクスチャがない場合
		else {
			// ダミーテクスチャ生成
			itr->second.shader_resource_views[0] = Texture::MakeDummyTexture(dev);
		}
	}
	
}

void SkinnedMesh::Render(ID3D12Device *dev, ComPtr<ID3D12GraphicsCommandList> cmdList, const XMFLOAT4X4 &world, const XMFLOAT4 &material_color, const Animation::Keyframe *keyframe)
{
	HRESULT result;
	
	// シーンバッファ更新
	SceneConstantBuffer *scene_constant_buffer_map = nullptr;
	result = scene_constant_buffer_->Map(0, nullptr, (void **)&scene_constant_buffer_map);
	if (SUCCEEDED(result))
	{
		//仮で古いカメラクラスを使用する
		DirectX::XMStoreFloat4x4(&scene_constant_buffer_map->view_projection, Camera::GetCam()->GetViewProjectionMatrix());
		scene_constant_buffer_map->light_direction = { 0,0.5,0.5, 0 };
		scene_constant_buffer_map->camera_position = { Camera::GetCam()->eye.x,Camera::GetCam()->eye.y,Camera::GetCam()->eye.z ,1};
		scene_constant_buffer_->Unmap(0, nullptr);
	}

	//メッシュ回数分
	for (const Mesh &mesh : meshes) {

		//各バッファのセット
		// パイプラインステートの設定
		cmdList->SetPipelineState(pipelinestate_.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rootsignature_.Get());
		// プリミティブ形状を設定
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//// メッシュ定数バッファビューをセット
		//cmdList->SetGraphicsRootConstantBufferView(0, mesh.mesh_constant_buffer_->GetGPUVirtualAddress());
		// シーン定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(1, scene_constant_buffer_->GetGPUVirtualAddress());


		// 頂点バッファをセット(VBV)
		cmdList->IASetVertexBuffers(0, 1, &mesh.vbView);
		// インデックスバッファをセット(IBV)
		cmdList->IASetIndexBuffer(&mesh.ibView);

		// デスクリプタヒープのセット
		ID3D12DescriptorHeap *ppHeaps[] = { Texture::descriptor_heap_.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		

		
		MeshConstantBuffer mesh_constant_deta ;
		const Animation::Keyframe::Node &mesh_node = keyframe->nodes.at(mesh.node_index);
		XMStoreFloat4x4(&mesh_constant_deta.world, XMLoadFloat4x4(&mesh_node.global_transform) * XMLoadFloat4x4(&world));
		//XMStoreFloat4x4(&mesh_constant_deta.world, XMLoadFloat4x4(&mesh.default_grlobal_transform) * XMLoadFloat4x4(&world));


		// サブセット単位で描画
		for (const Mesh::Subset &subset : mesh.subsets) {
			// メッシュ定数バッファビューをセット
			cmdList->SetGraphicsRootConstantBufferView(0, subset.mesh_constant_buffer_->GetGPUVirtualAddress());
			const Material &material = materials.at(subset.material_unique_id);

			// バッファ転送用データ
			MeshConstantBuffer *mesh_constant_buffer_map = nullptr;
			// メッシュ定数バッファ更新
			result = subset.mesh_constant_buffer_->Map(0, nullptr, (void **)&mesh_constant_buffer_map);
			if (SUCCEEDED(result))
			{
				// world計算
				mesh_constant_buffer_map->world = mesh_constant_deta.world;
				//XMStoreFloat4x4(&mesh_constant_buffer_map->world, XMLoadFloat4x4(&mesh.default_grlobal_transform) * XMLoadFloat4x4(&world));
				//const Animation::Keyframe::Node &mesh_node = keyframe->nodes.at(mesh.node_index);
				//XMStoreFloat4x4(&mesh_constant_buffer_map->world, XMLoadFloat4x4(&mesh_node.global_transform) * XMLoadFloat4x4(&world));
				
				// ボーン計算
				const size_t bone_count = mesh.bind_pose.bones.size();
				for (int bone_index = 0; bone_index < bone_count; ++bone_index) {
					const Skeleton::Bone &bone = mesh.bind_pose.bones.at(bone_index);
					const Animation::Keyframe::Node &bone_node = keyframe->nodes.at(bone.node_index);
					XMStoreFloat4x4(&mesh_constant_buffer_map->bone_transforms[bone_index],
						XMLoadFloat4x4(&bone.offset_transform) *
						XMLoadFloat4x4(&bone_node.global_transform) *
						XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.default_grlobal_transform))
					);
				}

				// 色情報
				XMStoreFloat4(&mesh_constant_buffer_map->material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));


				subset.mesh_constant_buffer_->Unmap(0, nullptr);
			}

			// シェーダリソースビューをセット
			cmdList->SetGraphicsRootDescriptorTable(
				2, material.shader_resource_views[0]);

			//描画コマンド
			cmdList->DrawIndexedInstanced(subset.index_count,1, subset.start_index, 0, 0);
		}
	}
}

void SkinnedMesh::CreateDummyMaterial(std::unordered_map<uint64_t, Material> &materials)
{
	Material material;
	material.name = "dummy";

	material.unique_id = 0;

	material.Kd.x = 1.0f;
	material.Kd.y = 0.0f;
	material.Kd.z = 1.0f;
	material.Kd.w = 1.0f;

	materials.emplace(material.unique_id, std::move(material));


}
