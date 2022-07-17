#include "FbxResource.h"
#include <filesystem>
#include "Texture.h"

inline DirectX::XMFLOAT4X4 ToXMFLOAT4x4(const FbxAMatrix& fbxamatrix)
{
	DirectX::XMFLOAT4X4 xmfloat4x4;
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
		}
	}
	return xmfloat4x4;
}
inline DirectX::XMFLOAT2 ToXMFLOAT2(const FbxDouble2& fbxdouble3)
{
	DirectX::XMFLOAT2 xmfloat2;
	xmfloat2.x = static_cast<float>(fbxdouble3[0]);
	xmfloat2.y = static_cast<float>(fbxdouble3[1]);
	return xmfloat2;
}
inline DirectX::XMFLOAT3 ToXMFLOAT3(const FbxDouble3& fbxdouble3)
{
	DirectX::XMFLOAT3 xmfloat3;
	xmfloat3.x = static_cast<float>(fbxdouble3[0]);
	xmfloat3.y = static_cast<float>(fbxdouble3[1]);
	xmfloat3.z = static_cast<float>(fbxdouble3[2]);
	return xmfloat3;
}
inline DirectX::XMFLOAT4 ToXMFLOAT4(const FbxDouble4& fbxdouble4)
{
	DirectX::XMFLOAT4 xmfloat4;
	xmfloat4.x = static_cast<float>(fbxdouble4[0]);
	xmfloat4.y = static_cast<float>(fbxdouble4[1]);
	xmfloat4.z = static_cast<float>(fbxdouble4[2]);
	xmfloat4.w = static_cast<float>(fbxdouble4[3]);
	return xmfloat4;
}


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
		const FbxSkin* fbx_skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin));
		const int cluster_count = fbx_skin->GetClusterCount();

		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index) {
			const FbxCluster* fbx_cluster = fbx_skin->GetCluster(cluster_index);

			const int control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();

			for (int control_point_indices_index = 0; control_point_indices_index < control_point_indices_count; ++control_point_indices_index) {
				int control_point_index{ fbx_cluster->GetControlPointIndices()[control_point_indices_index] };
				double control_point_weight = fbx_cluster->GetControlPointWeights()[control_point_indices_index];
				BoneInfluence& bone_influence = bone_influences.at(control_point_index).emplace_back();
				bone_influence.bone_index = static_cast<uint32_t>(cluster_index);
				bone_influence.bone_weight = static_cast<float>(control_point_weight);
			}
		}
	}
}


void FbxResource::LoadModel(ID3D12Device *dev, const char *fbx_filename)
{
	// シリアライズ
	std::filesystem::path cereal_filename = fbx_filename;
	filename_ = fbx_filename;
	cereal_filename.replace_extension(cerealize_extension);		//拡張子部分をcerealに変更
	// シリアライズ済みのデータがあればそれを読み込む
	if (std::filesystem::exists(cereal_filename.c_str())) {
		std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
		cereal::BinaryInputArchive deserialization = ifs;
		deserialization(nodes_, meshes_, materials_, animations_);
	}
	else {
		// マネージャーの生成
		FbxManager *fbx_manager = FbxManager::Create();
		// シーン生成
		FbxScene *fbx_scene = FbxScene::Create(fbx_manager, "MyScene");


		FbxImporter *fbxImporter = FbxImporter::Create(fbx_manager, "MyImporter");
		// ファイル名を指定してFBXファイルを読み込む
		if (!fbxImporter->Initialize(fbx_filename, -1, fbx_manager->GetIOSettings()))
		{
			assert(0);
		}

		// インポート
		if (!fbxImporter->Import(fbx_scene)) {
			assert(0);
		}

		// 三角形化
		FbxGeometryConverter fbx_converter(fbx_manager);
		fbx_converter.Triangulate(fbx_scene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);

		// 座標系をDirectXで統一
		FbxAxisSystem scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
		if (scene_axis_system != FbxAxisSystem::DirectX) {
			FbxAxisSystem::DirectX.DeepConvertScene(fbx_scene);
		}

		//// メートルに変換
		//FbxSystemUnit scene_unit = fbx_scene->GetGlobalSettings().GetSystemUnit();
		//scale_factor_ = scene_unit.GetScaleFactor();
		//if (scene_unit != FbxSystemUnit::m) {
		//	//FbxSystemUnit::m.ConvertScene(fbx_scene);
		////	FbxSystemUnit::m.ConvertScene(fbx_scene);
		//}

		//ノードの取得
		std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbx_node) {
			Node &node = nodes_.emplace_back();
			// typeがnullだったらeUnknownをセット
			node.attribute = fbx_node->GetNodeAttribute() ?
				fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
			node.name = fbx_node->GetName();
			node.id = fbx_node->GetUniqueID();
			// 親がいなければ0を代入
			node.parent_index = FindNodeIndex(fbx_node->GetParent() ?
				fbx_node->GetParent()->GetUniqueID() : 0);

			// トランスフォーム
			node.scaling = ToXMFLOAT3(fbx_node->EvaluateLocalTransform().GetS());
			node.rotation = ToXMFLOAT4(fbx_node->EvaluateLocalTransform().GetQ());
			node.translation = ToXMFLOAT3(fbx_node->EvaluateLocalTransform().GetT());


			// 子に対して再起処理
			for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index) {
				traverse(fbx_node->GetChild(child_index));
			}
		} };

		// ノード取得
		traverse(fbx_scene->GetRootNode());

		// メッシュ情報を取得
		FetchMeshes(fbx_scene, meshes_);
		// マテリアル情報を取得
		FetchMaterial(fbx_scene, materials_);
		// アニメーションを取得
		FetchAnimations(fbx_scene, animations_);

		// 解放
		fbx_manager->Destroy();

		// シリアライズ
		std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive serialization = ofs;
		serialization(nodes_, meshes_, materials_, animations_);
	}
	BindMaterial();

	CreateComObjects(dev);
}

FbxResource::NodeId FbxResource::FindNodeIndex(NodeId nodeId) const
{
	int nodeCount = static_cast<int>(nodes_.size());
	for (int i = 0; i < nodeCount; ++i)
	{
		if (nodes_[i].id == nodeId)
		{
			return i;
		}
	}
	return -1;
}

void FbxResource::FetchMeshes(FbxScene *fbx_scene, std::vector<Mesh> &meshes)
{
	// すべてのノードをチェック
	for (const Node& node : nodes_) {
		// メッシュでなければスキップ
		if (node.attribute != FbxNodeAttribute::EType::eMesh) {
			continue;
		}
		
		// ノードとメッシュを抜き出し
		FbxNode* fbx_node = fbx_scene->FindNodeByName(node.name.c_str());
		FbxMesh* fbx_mesh = fbx_node->GetMesh();

		Mesh &mesh = meshes.emplace_back();
		mesh.id = FindNodeIndex(fbx_mesh->GetNode()->GetUniqueID());

		// ボーン影響度の獲得
		std::vector<BoneInfluencesPerControlPoint> bone_influences;
		FetchBoneInfluences(fbx_mesh, bone_influences);
		// バインドポーズの取得
		FetchSkeleton(fbx_mesh, &mesh);

		// サブセットの取得
		std::vector<Subset> &subsets = mesh.subsets;
		// マテリアルカウント
		const int material_count = fbx_mesh->GetNode()->GetMaterialCount();
		// あらかじめ領域を確保（最低1）
		subsets.resize(material_count > 0 ? material_count : 1);
		// 全てのマテリアルidと名前を取得
		for (int material_index = 0; material_index < material_count; ++material_index) {
			const FbxSurfaceMaterial *fbx_material = fbx_mesh->GetNode()->GetMaterial(material_index);
			subsets.at(material_index).material_index = fbx_material->GetUniqueID();
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
			for (Subset &subset : subsets) {
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
				fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0;
			Subset &subset = subsets.at(material_index);
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

				// タンジェントがあればコピー
				if (fbx_mesh->GenerateTangentsData(0, false))
				{
					const FbxGeometryElementTangent *tangent = fbx_mesh->GetElementTangent(0);
					vertex.tangent.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[0]);
					vertex.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[1]);
					vertex.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[2]);
				}

				mesh.vertices.at(vertex_index) = std::move(vertex);
				mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
				subset.index_count++;
			}
		}
	}

}

void FbxResource::FetchSkeleton(FbxMesh *fbx_mesh, Mesh *mesh)
{
	const int deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int deformer_index = 0; deformer_index < deformer_count; deformer_index++) {
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
		const int cluster_count = skin->GetClusterCount();
		mesh->offset_transforms.resize(cluster_count);
		mesh->node_indices.resize(cluster_count);

		for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index) {
			FbxCluster *cluster = skin->GetCluster(cluster_index);
			mesh->node_indices.at(cluster_index) = FindNodeIndex(cluster->GetLink()->GetUniqueID());

			// 初期姿勢
			FbxAMatrix reference_gloval_init_position;
			cluster->GetTransformMatrix(reference_gloval_init_position);

			FbxAMatrix cluster_global_init_position;
			cluster->GetTransformLinkMatrix(cluster_global_init_position);

			mesh->offset_transforms.at(cluster_index)
				= ToXMFLOAT4x4(cluster_global_init_position.Inverse() * reference_gloval_init_position);
		}
	}
}



void FbxResource::FetchMaterial(FbxScene *fbx_scene, std::vector<Material> &materials)
{
	// ノードの数
	const size_t node_count = nodes_.size();
	// ダミーマテリアルはここ
	for (size_t node_index = 0; node_index < node_count; ++node_index) {
		const Node &node = nodes_.at(node_index);
		const FbxNode *fbx_node = fbx_scene->FindNodeByName(node.name.c_str());

		const int material_count= fbx_node->GetMaterialCount();
		for (int material_index = 0; material_index < material_count; ++material_index) {
			// マテリアル取得
			const FbxSurfaceMaterial *fbx_material= fbx_node->GetMaterial(material_index);

			Material material;
			material.name = fbx_material->GetName();
			material.id = fbx_material->GetUniqueID();


			bool skip = false;
			for (Material &m : materials)
			{
				if (m.id == material.id)
				{
					skip = true;
				}
			}
			if (skip)
			{
				continue;
			}

			// ディフューズのチェック
			FbxProperty fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (fbx_property.IsValid())
			{
				const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
				material.color.x = static_cast<float>(color[0]);
				material.color.y = static_cast<float>(color[1]);
				material.color.z = static_cast<float>(color[2]);
				material.color.w = 1.0f;

				const FbxFileTexture *fbx_texture = fbx_property.GetSrcObject<FbxFileTexture>();
				material.texture_filenames[0] =
					fbx_texture ? fbx_texture->GetRelativeFileName() : "";
			}

			materials.emplace_back(material);
		}
	}
}

void FbxResource::FetchAnimations(FbxScene *fbx_scene, std::vector<Animation> &animation_clips, float sampling_rate)
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
		const FbxTime::EMode time_mode{ fbx_scene->GetGlobalSettings().GetTimeMode() };
		FbxTime one_second;
		one_second.SetTime(0, 0, 1, 0, 0, time_mode);
		animation_clip.sampling_rate = sampling_rate > 0 ?
			sampling_rate : static_cast<float>(one_second.GetFrameRate(time_mode));

		// サンプリング間隔を設定
		const FbxTime sampling_interval = static_cast<FbxLongLong>(one_second.Get() / animation_clip.sampling_rate);
		float fsamplng_interval = one_second.Get() / animation_clip.sampling_rate;
		const FbxTakeInfo *take_info = fbx_scene->GetTakeInfo(animation_clip.name.c_str());
		const FbxTime start_time = take_info->mLocalTimeSpan.GetStart();
		const FbxTime stop_time = take_info->mLocalTimeSpan.GetStop();

		// アニメーション時間計算
		float stop = stop_time.GetFrameCount() / animation_clip.sampling_rate;
		float start = start_time.GetFrameCount() / animation_clip.sampling_rate;
		animation_clip.seconds_length = stop - start;

		// キーフレームの取得
		for (FbxTime time = start_time; time < stop_time; time += sampling_interval)
		{
			Keyframe &keyframe = animation_clip.keyframes.emplace_back();


			const size_t node_count = nodes_.size();
			keyframe.nodeKeys.resize(node_count);

			float f_time = time.GetFrameCount() / animation_clip.sampling_rate;
			keyframe.seconds = f_time - start;

			for (size_t node_index = 0; node_index < node_count; ++node_index) {
				FbxNode *fbx_node = fbx_scene->FindNodeByName(nodes_.at(node_index).name.c_str());
				if (fbx_node) {
					NodeKeyData &node = keyframe.nodeKeys.at(node_index);

					const FbxAMatrix &local_transform = fbx_node->EvaluateLocalTransform(time);
					node.scale = ToXMFLOAT3(local_transform.GetS());
					node.rotate = ToXMFLOAT4(local_transform.GetQ());
					node.translate = ToXMFLOAT3(local_transform.GetT());
				}
			}
		}
	}
	for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
	{
		delete animation_stack_names[animation_stack_index];
	}
}

void FbxResource::BindMaterial()
{
	bool bind = false;
	for (Mesh &mesh : meshes_) {
		for (Subset &subset : mesh.subsets) {
			bind = false;
			for (Material &material : materials_) {
				if (subset.material_index == material.id) {
					subset.material = &material;
					bind = true;
					break;
				}
			}
			// ダミーマテリアル生成
			if (!bind) {
				Material material;
				material.id = subset.material_index;
				material.name = "dummy";
				material.texture_filenames[0] = "";
				material.texture_filenames[1] = "";

				materials_.emplace_back(material);
				subset.material = &materials_.back();
			}
		}
	}
}

void FbxResource::CreateComObjects(ID3D12Device *dev)
{
	HRESULT result;

	//メッシュの数処理を繰り返す
	for (Mesh &mesh : meshes_) {

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

		// メッシュ定数バッファの生成
		result = dev->CreateCommittedResource
		(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(MeshConstantBuffer) + 0xff) & ~0xff),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mesh.mesh_constant_buffer_)
		);

		// サブセット定数バッファの生成
		for (Subset &subset : mesh.subsets) {
			result = dev->CreateCommittedResource
			(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SubsetConstantBuffer) + 0xff) & ~0xff),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&subset.subset_constant_buffer_)
			);
		}
	}

	

	for (Material &material : materials_) {
		// テクスチャデータ
		for (int i = 0; i < static_cast<int>(TextureType::MAX); ++i) {
			std::filesystem::path texture_name;			// テクスチャの名前
			std::filesystem::path texture_extension;	// 拡張子

			if (static_cast<TextureType>(i) == TextureType::NORMAL) {
				texture_name = material.texture_filenames[1];
				texture_extension = material.texture_filenames[1];
			}
			else {
				texture_name = material.texture_filenames[0];
				texture_extension = material.texture_filenames[0];
			}
			texture_name = texture_name.replace_extension("");
			texture_extension = texture_extension.extension();

			std::filesystem::path texture_path =
				texture_name.string() + filename[i] + texture_extension.string();

			std::filesystem::path path = filename_;
			path.replace_filename(texture_path);

			// テクスチャのチェック
			// ファイルがあれば読み込み
			if (std::filesystem::exists(path)) {
				// マテリアルのみの場合はダミーを貼る
				if (texture_path.c_str()[0] == 0) {
					material.shader_resource_views[i] = Texture::MakeDummyTexture(dev);
				}
				else {
					material.shader_resource_views[i] = Texture::LoadTextureFromFile(dev, path.c_str());
				}
			}
			else {
				material.shader_resource_views[i] = Texture::MakeDummyTexture(dev);
			}
		}
	}
}
