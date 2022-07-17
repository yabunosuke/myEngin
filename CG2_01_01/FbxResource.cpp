#include "FbxResource.h"
#include <filesystem>


void FbxResource::LoadModel(ID3D12Device *dev, const char *fbx_filename)
{
	// シリアライズ
	std::filesystem::path cereal_filename = fbx_filename;
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

		// 座標系の統一
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
		std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
			Node &node = nodes_.emplace_back();
			// typeがnullだったらeUnknownをセット
			node.atribute = fbxNode->GetNodeAttribute() ?
				fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
			node.name = fbxNode->GetName();
			node.unique_id = fbxNode->GetUniqueID();
			// 親がいなければ0を代入
			node.parent_index = scene_view_.indexof(fbxNode->GetParent() ?
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

		std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive serialization = ofs;
		serialization(scene_view_, meshes, materials, animation_clips_);

	}
}
