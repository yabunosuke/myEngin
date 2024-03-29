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

bool FileManager::FileSave(SAVE_TYPE saveType)
{
	static OPENFILENAME openFile;
	//初期フルパス
	static TCHAR firstFilePath[MAX_PATH];
	//保存先フルパス
	static TCHAR saveFilePath[MAX_PATH];

	if (firstFilePath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, firstFilePath);
	}

	if (openFile.lStructSize == 0) {
		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = WinApp::GetHwnd();
		openFile.lpstrInitialDir = firstFilePath;	//初期フォルダ位置
		openFile.lpstrFile = saveFilePath;			//選択ファイル格納
		openFile.nMaxFile = MAX_PATH;
		switch (saveType)
		{
		case SAVE_TYPE::PROJECT:
			openFile.lpstrDefExt = TEXT(".project");		//拡張子
			openFile.lpstrFilter = TEXT("yabuEnginProject(*.project)\0*.project\0");
			openFile.lpstrTitle = TEXT("Save the project");
			break;
		case SAVE_TYPE::SCENE:
			SceneFileSetting(openFile);

			break;
		default:
			break;
		}

		openFile.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	//保存処理
	if (GetSaveFileName(&openFile)) {
		switch (saveType)
		{
		case SAVE_TYPE::PROJECT:
			break;
		case SAVE_TYPE::SCENE:
			CreateSceneFile();
			break;
		default:
			break;
		}
		return true;
	}


	return false;
}

bool FileManager::LoadLevelJson(const std::string &file_path)
{
	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(file_path);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// "objects"の全オブジェクトを走査
	//ノードの取得
	//std::function<void(FbxNode *)> traverse{ [&](FbxNode *fbx_node) {
	//	
	//	// 子に対して再起処理
	//	for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index) {
	//		traverse(fbx_node->GetChild(child_index));
	//	}
	//} };
	for (nlohmann::json &object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// MESH
		if (type.compare("MESH") == 0) {

			//// 要素追加
			auto game_object = GameObject::CreateObject(object["name"]);
			//levelData->objects.emplace_back(LevelData::ObjectData{});
			//// 今追加した要素の参照を得る
			//LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				// ファイル名
				std::string model_path{ object["file_name"] };
				game_object->AddComponent<Object3dComponent>(
						DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
					model_path.c_str());
			}
			// トランスフォームのパラメータ読み込み
			nlohmann::json &transform = object["transform"];
			// 平行移動
			game_object->transform_->localPosition =
				{
						(float)transform["translation"][1],
						(float)transform["translation"][2],
						(float)transform["translation"][0]
				};
			game_object->transform_->localQuaternion =
				{
						(float)transform["rotation"][1],
						(float)transform["rotation"][2],
						(float)transform["rotation"][0],
						(float)transform["rotation"][3],
				};
			game_object->transform_->localScale =
			{
					(float)transform["scaling"][1],
					(float)transform["scaling"][2],
					(float)transform["scaling"][0]
			};


			// TODO: コライダーのパラメータ読み込み
			if (object.contains("collider"))
			{
				nlohmann::json &collider = object["collider"];
				std::string type{ collider["type"] };
				if (type == "BOX")
				{
					Vector3	center{
						static_cast<float>(collider["center"][1]),
						static_cast<float>(collider["center"][2]),
						static_cast<float>(collider["center"][0]),
					};
					Vector3	scale{
						collider["size"][1],
						collider["size"][2],
						collider["size"][0],
					};
					game_object->AddComponent<OBBCollider>(
						Quaternion{ 0.0f,0.0f,0.0f,0.0f },
						scale, center);
				}
			}
		}

		//// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出で枝を走査する
		if (object.contains("children")) {

		}
	}


	return false;
}

bool FileManager::LoadFile()
{
	return false;
}

bool FileManager::SceneFileSetting(OPENFILENAME &openFile)
{

	openFile.lpstrDefExt = TEXT(".scene");		//拡張子
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
