#include "FileManager.h"
#include <json.hpp>
#include <fstream>
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Collider/OBBCollider/OBBCollider.h"
#include "Object3dComponent.h"
#include <DirectXCommon.h>

FileManager *FileManager::GetIns()
{
	static FileManager *instance;
	return instance;
}


bool FileManager::FileSave(
	OPENFILENAME &open_file,
	TCHAR first_file_path[],
	TCHAR save_file_path[], 
	const std::wstring &ext, 
	const std::wstring &filter, 
	const std::wstring &title)
{
	if (first_file_path[0] == TEXT('\0'))
	{
		GetCurrentDirectory(MAX_PATH, first_file_path);
	}

	if (open_file.lStructSize == 0) {
		open_file.lStructSize = sizeof(OPENFILENAME);
		open_file.hwndOwner = WinApp::GetHwnd();
		open_file.lpstrInitialDir = first_file_path;	//�����t�H���_�ʒu
		open_file.lpstrFile = save_file_path;			//�I���t�@�C���i�[
		open_file.nMaxFile = MAX_PATH;
		open_file.lpstrDefExt = ext.c_str();;
		open_file.lpstrFilter = filter.c_str();
		open_file.lpstrTitle = title.c_str();

		open_file.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	return true;
}
//
//bool FileManager::LoadLevelJson(const std::string &file_path)
//{
//	// �t�@�C���X�g���[��
//	std::ifstream file;
//
//	// �t�@�C�����J��
//	file.open(file_path);
//	// �t�@�C���I�[�v�����s���`�F�b�N
//	if (file.fail()) {
//		assert(0);
//	}
//
//	// JSON�����񂩂�𓀂����f�[�^
//	nlohmann::json deserialized;
//
//	// ��
//	file >> deserialized;
//
//	// ���������x���f�[�^�t�@�C�����`�F�b�N
//	assert(deserialized.is_object());
//	assert(deserialized.contains("name"));
//	assert(deserialized["name"].is_string());
//
//	// "name"�𕶎���Ƃ��Ď擾
//	std::string name = deserialized["name"].get<std::string>();
//	// ���������x���f�[�^�t�@�C�����`�F�b�N
//	assert(name.compare("scene") == 0);
//
//	// "objects"�̑S�I�u�W�F�N�g�𑖍�
//	//�m�[�h�̎擾
//	//std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbx_node) {
//	//	
//	//	// �q�ɑ΂��čċN����
//	//	for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index) {
//	//		traverse(fbx_node->GetChild(child_index));
//	//	}
//	//} };
//	for (nlohmann::json &object : deserialized["objects"]) {
//		assert(object.contains("type"));
//
//		// ��ʂ��擾
//		std::string type = object["type"].get<std::string>();
//
//		// MESH
//		if (type.compare("MESH") == 0) {
//
//			//// �v�f�ǉ�
//			auto game_object = GameObject::CreateObject(object["name"]);
//			//levelData->objects.emplace_back(LevelData::ObjectData{});
//			//// ���ǉ������v�f�̎Q�Ƃ𓾂�
//			//LevelData::ObjectData& objectData = levelData->objects.back();
//
//			if (object.contains("file_name")) {
//				// �t�@�C����
//				std::string model_path{ object["file_name"] };
//				game_object.lock()->AddComponent<Object3dComponent>(
//						DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
//					model_path.c_str());
//			}
//			// �g�����X�t�H�[���̃p�����[�^�ǂݍ���
//			nlohmann::json &transform = object["transform"];
//			// ���s�ړ�
//			game_object.lock()->transform->localPosition =
//				{
//						(float)transform["translation"][1],
//						(float)transform["translation"][2],
//						(float)transform["translation"][0]
//				};
//			game_object.lock()->transform->localQuaternion =
//				{
//						(float)transform["rotation"][1],
//						(float)transform["rotation"][2],
//						(float)transform["rotation"][0],
//						(float)transform["rotation"][3],
//				};
//			game_object.lock()->transform->localScale =
//			{
//					(float)transform["scaling"][1],
//					(float)transform["scaling"][2],
//					(float)transform["scaling"][0]
//			};
//
//
//			// TODO: �R���C�_�[�̃p�����[�^�ǂݍ���
//			if (object.contains("collider"))
//			{
//				nlohmann::json &collider = object["collider"];
//				std::string type{ collider["type"] };
//				if (type == "BOX")
//				{
//					Vector3	center{
//						static_cast<float>(collider["center"][1]),
//						static_cast<float>(collider["center"][2]),
//						static_cast<float>(collider["center"][0]),
//					};
//					Vector3	scale{
//						collider["size"][1],
//						collider["size"][2],
//						collider["size"][0],
//					};
//					game_object.lock()->AddComponent<OBBCollider>(
//						Quaternion{ 0.0f,0.0f,0.0f,0.0f },
//						scale, center);
//				}
//			}
//		}
//
//		//// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo�Ŏ}�𑖍�����
//		if (object.contains("children")) {
//
//		}
//	}
//
//
//	return false;
//}

bool FileManager::LoadFile()
{
	return false;
}

bool FileManager::SceneFileSetting(OPENFILENAME &openFile)
{

	openFile.lpstrDefExt = TEXT(".scene");		//�g���q
	openFile.lpstrFilter = TEXT("yabuEnginScene(*.scene)\0*.scene\0");
	openFile.lpstrTitle = TEXT("Save the scene");

	return true;
}

bool FileManager::CreateSceneFile()
{
	using json = nlohmann::json;

	json scene_date
	{
		{"test int",10}
	};

	
	return false;
}
