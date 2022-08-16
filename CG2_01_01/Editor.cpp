#include "Editor.h"
#include "ImGui/imguiManager.h"
#include "FileManager.h"

#include "TransformComponent.h"

#include "ComponentList.h"

#include <algorithm>

#include "DirectXCommon.h"

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
	// 出力
	if(true)
	{
		DrawMulutiRender();
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

	ImGui::Text(nowScene.get()->GetName().c_str());
	nowScene.get()->GetObjectManager()->DrawHierarchy(selectedObjectNum);

	//追加
	if (ImGui::Button("add")) {
		nowScene->GetObjectManager()->CreateObject();
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

	auto *selectObject = nowScene->GetObjectManager()->gameObjects[selectedObjectNum].get();

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

	// コンポーネントグループ
	auto &component_groups = ComponentList::GetIns()->GetComponentGroup();
	// コンポーネントリスト
	auto &component_list = ComponentList::GetIns()->GetComponentList();

	if (ImGui::BeginPopup("ComponentList")) {
		// コンポーネントの検索
		static char searchComponent[64] = "";
		ImGui::InputText("##searchBox", searchComponent, 64);
		
		// 検索ボックスに何も入っていなかったら全て表示
		if (searchComponent[0] == '\0') {
			ImGui::Text("Component");

			//グループ単位で表示
			for (auto &component_group : component_groups) {
				// グループ名表示
				std::string group_name = "";
				switch (component_group.first)
				{
				case ComponentList::Group::Physics:
					group_name = "Physics";
					break;
				case ComponentList::Group::Physics2D:
					group_name = "Physics2D";
					break;
				default:
					group_name = "error";
					break;
				}
				

				// グループごとに表示
				if (ImGui::TreeNode(group_name.c_str())) {
					// 属しているコンポーネントを表示
					for (auto &component : component_group.second) {
						if (ImGui::Button(component.c_str())) {
							// コンポーネント追加
							///selectObject->AddComponent<LightComponent>();
						}
					}
					ImGui::TreePop();
				}

				//switch (component.first)
				//{
				//case ComponentList::Group::Rendering:
				//	if(ImGui::TreeNode("Rendering")){

				//		//中身を表示
				//		for (auto &component : component.second) {
				//			if (ImGui::Button(component.c_str())) {
				//				ComponentList::GetIns()->GetComponentList()[component];
				//			}
				//		}

				//		ImGui::TreePop();
				//	}
				//	break;
				//default:
				//	ImGui::TreeNode("Dummy");
				//	ImGui::TreePop();

				//	break;
				//}
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

void Editor::DrawMulutiRender()
{
	//ウィンドウ名定義
	ImGui::Begin("MulutiRender");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	D3D12_GPU_DESCRIPTOR_HANDLE my_texture_gpu_handle = 
		nowScene->GetMulutiRenderTarget()->get()->descriputor_heap_SRV_->GetGPUDescriptorHandleForHeapStart();
	UINT handle_increment = DirectXCommon::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//my_texture_gpu_handle.ptr += (handle_increment * 1);
	ImGui::Image(
		(ImTextureID)my_texture_gpu_handle.ptr,
		ImVec2((float)100, (float)100)
	);
	//終了
	ImGui::End();
}
