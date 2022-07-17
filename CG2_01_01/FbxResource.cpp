#include "FbxResource.h"
#include <filesystem>


void FbxResource::LoadModel(ID3D12Device *dev, const char *fbx_filename)
{
	// �V���A���C�Y
	std::filesystem::path cereal_filename = fbx_filename;
	cereal_filename.replace_extension(cerealize_extension);		//�g���q������cereal�ɕύX
	// �V���A���C�Y�ς݂̃f�[�^������΂����ǂݍ���
	if (std::filesystem::exists(cereal_filename.c_str())) {
		std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
		cereal::BinaryInputArchive deserialization = ifs;
		deserialization(nodes_, meshes_, materials_, animations_);
	}
	else {
		// �}�l�[�W���[�̐���
		FbxManager *fbx_manager = FbxManager::Create();
		// �V�[������
		FbxScene *fbx_scene = FbxScene::Create(fbx_manager, "MyScene");


		FbxImporter *fbxImporter = FbxImporter::Create(fbx_manager, "MyImporter");
		// �t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
		if (!fbxImporter->Initialize(fbx_filename, -1, fbx_manager->GetIOSettings()))
		{
			assert(0);
		}

		// �C���|�[�g
		if (!fbxImporter->Import(fbx_scene)) {
			assert(0);
		}

		// �O�p�`��
		FbxGeometryConverter fbx_converter(fbx_manager);
		fbx_converter.Triangulate(fbx_scene, true);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);

		// ���W�n�̓���
		FbxAxisSystem scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
		if (scene_axis_system != FbxAxisSystem::DirectX) {
			FbxAxisSystem::DirectX.DeepConvertScene(fbx_scene);
		}

		//// ���[�g���ɕϊ�
		//FbxSystemUnit scene_unit = fbx_scene->GetGlobalSettings().GetSystemUnit();
		//scale_factor_ = scene_unit.GetScaleFactor();
		//if (scene_unit != FbxSystemUnit::m) {
		//	//FbxSystemUnit::m.ConvertScene(fbx_scene);
		////	FbxSystemUnit::m.ConvertScene(fbx_scene);
		//}

		//�m�[�h�̎擾
		std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbxNode) {
			Node &node = nodes_.emplace_back();
			// type��null��������eUnknown���Z�b�g
			node.atribute = fbxNode->GetNodeAttribute() ?
				fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
			node.name = fbxNode->GetName();
			node.unique_id = fbxNode->GetUniqueID();
			// �e�����Ȃ����0����
			node.parent_index = scene_view_.indexof(fbxNode->GetParent() ?
				fbxNode->GetParent()->GetUniqueID() : 0);
			// �q�ɑ΂��čċN����
			for (int child_index = 0; child_index < fbxNode->GetChildCount(); ++child_index) {
				traverse(fbxNode->GetChild(child_index));
			}
		} };

		// �m�[�h�擾
		traverse(fbx_scene->GetRootNode());
		// ���b�V�������擾
		FetchMeshes(fbx_scene, meshes);
		// �}�e���A�������擾
		FetchMaterial(fbx_scene, materials);
		// �A�j���[�V�������擾
		FetchAnimations(fbx_scene, animation_clips_);

		// ���
		fbx_manager->Destroy();

		std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive serialization = ofs;
		serialization(scene_view_, meshes, materials, animation_clips_);

	}
}
