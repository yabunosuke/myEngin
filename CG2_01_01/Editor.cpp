#include "Editor.h"
#include "imguiManager.h"
#include "FileManager.h"
#include "ColliderComponent.h"

#include "TransformComponent.h"

#include "ComponentList.h"

#include <algorithm>

void Editor::Draw()
{
	//プルダウンメニュー
	MainMenu();
	//ヒエラルキー
	if (isHierarchy) {
		DrawHierarchy();
	}
	//インスペクター
	if (isHierarchy) {
		DrawInspector();
	}
	//デモ
	if (isDemo) {
		ImGui::ShowDemoWindow();
	}
}

void Editor::MainMenu()
{
	if (ImGui::BeginMainMenuBar()) {
		//ファイル
		if (ImGui::BeginMenu("File")) {
			FileMenu();
			ImGui::EndMenu();
		}

		//編集
		if (ImGui::BeginMenu("Edit")) {
			EditMenu();
			ImGui::EndMenu();
		}

		//ゲームオブジェクト
		if (ImGui::BeginMenu("GameObject")) {
			ImGui::EndMenu();
		}

		//表示するウィンドウ
		if (ImGui::BeginMenu("Window")) {
			WindowMenu();
			ImGui::EndMenu();
		}

		//ヘルプ
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Editor::FileMenu()
{
	//上書き保存
	if (ImGui::MenuItem("Save", "Ctrl+S")) {
	}
	//名前を付けて保存
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
		FileManager::GetIns()->FileSave(SAVE_TYPE::SCENE);
	}

	ImGui::Separator();


	//ビルド設定
	if (ImGui::MenuItem("Build Settings...")) {
	}

	//ビルド設定
	if (ImGui::MenuItem("Build and Run", "Ctrl+B")) {
	}

	ImGui::Separator();
	//終了処理
	if (ImGui::MenuItem("Exit")) {
		exit(0);
	}
}

void Editor::EditMenu()
{
	// コピー
	if (ImGui::MenuItem("Copy", "Ctrl+C")) {
	}
	// カット
	if (ImGui::MenuItem("Cut", "Ctrl+X")) {
	}
	// ペースト
	if (ImGui::MenuItem("Paste", "Ctrl+V")) {
	}

}

void Editor::WindowMenu()
{
	if (ImGui::Checkbox("Hierarchy", &isHierarchy)) {
	}
	if (ImGui::Checkbox("Insupector", &isInsupector)) {
	}
	if (ImGui::Checkbox("Demo", &isDemo)) {
	}
}

void Editor::DrawHierarchy()
{
	//ウィンドウ名定義
	ImGui::Begin("Hierarchy");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	ImGui::Text(nowScene.get()->name.c_str());
	nowScene.get()->gameObjectManager.DrawHierarchy(selectedObjectNum);

	//追加
	if (ImGui::Button("add")) {
		nowScene->gameObjectManager.CreateObject();
	}

	//終了
	ImGui::End();

}

void Editor::DrawInspector()
{
	ImGui::Begin("Inspector");
	//未選択の場合は何も描画しない
	if (selectedObjectNum < 0) {
		ImGui::End();
		return;
	}

	auto *selectObject = nowScene->gameObjectManager.gameObjects[selectedObjectNum].get();

	//インスペクター描画
	ImGui::PushID(selectedObjectNum);
	selectObject->DrawInspector();
	ImGui::PopID();

	// ポップアップウィンドウ
	// コンポーネントの追加
	static bool openAddComponent = false;
	openAddComponent = ImGui::Button("Add Component");
	if (openAddComponent) {
		ImGui::OpenPopup("ComponentList");
	}
	if (ImGui::BeginPopup("ComponentList")) {
		// コンポーネントの検索
		static char searchComponent[64] = "";
		ImGui::InputText("##searchBox", searchComponent, 64);
		
		// 検索ボックスに何も入っていなかったら全て表示
		if (searchComponent[0] == '\0') {
			ImGui::Text("Component");

			//グループ単位で表示
			for (auto &componentGroup : ComponentList::GetIns()->GetComponentGroup()) {
				switch (componentGroup.first)
				{
				case ComponentList::Group::Rendering:
					if(ImGui::TreeNode("Rendering")){

						//中身を表示
						for (auto &component : componentGroup.second) {
							if (ImGui::Button(component.c_str())) {
								ComponentList::GetIns()->GetComponentList()[component];
							}
						}

						ImGui::TreePop();
					}
					break;
				default:
					ImGui::TreeNode("Dummy");
					ImGui::TreePop();

					break;
				}
				//ImGui::Button(component.first)
				
			}

			//すべて表示
			for (auto &list : ComponentList::GetIns()->GetComponentList()) {
				if (ImGui::Button(list.first.c_str())) {
					list.second(selectObject);					
				}
			}
		}
		// 検索
		else {
			// 小文字のstringに変換
			std::string searchComponentStrnig = searchComponent;
			std::transform(searchComponentStrnig.begin(), searchComponentStrnig.end(), searchComponentStrnig.begin(), std::tolower);
			ImGui::Text("Search");

			for (auto &list : ComponentList::GetIns()->GetComponentList()) {
				// リストのコンポーネント名を小文字に変換
				std::string componentName = list.first;
				std::transform(componentName.begin(), componentName.end(), componentName.begin(), std::tolower);
				//一部合致するものがあれば表示
				if (componentName.find(searchComponentStrnig) != std::string::npos) {
					//追加
					if (ImGui::Button(list.first.c_str())) {
						list.second(selectObject);
					}
				}
			}
		}
		


		ImGui::EndPopup();
	}
	
	ImGui::SameLine();


	ImGui::End();


}
