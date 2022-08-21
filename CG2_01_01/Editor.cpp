#include "Editor.h"
#include "ImGui/imguiManager.h"
#include "ImGui/ImGuizmo.h"
#include "Camera.h"
#include "FileManager.h"

#include "Component/Transform.h"

#include "ComponentList.h"

#include <algorithm>

#include "DirectXCommon.h"

void Editor::Draw()
{
	//プルダウンメニュー
	MainMenu();
	//ヒエラルキー
	if (isHierarchy) {
		HierarchyWindow();
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

void Editor::HierarchyWindow()
{
	//ウィンドウ名定義
	ImGui::Begin("Hierarchy");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	ImGui::Text(nowScene.get()->GetName().c_str());
	/*nowScene.get()->GetObjectManager()->DrawHierarchy(selected_object_id);*/
	Hierarchy(nowScene.get()->GetObjectManager()->game_objects_);
	//追加
	if (ImGui::Button("add")) {
		nowScene->GetObjectManager()->CreateObject();
	}

	//終了
	ImGui::End();

}

void Editor::Hierarchy(std::vector<std::shared_ptr<GameObject>>& objects, bool is_child)
{
	int n = 0;
	for (const auto &object : objects)
	{
		// 親以外は後で表示する
		if ((object->GetPearent().lock() != nullptr) &&
			!is_child)
		{
			continue;
		}

		ImGui::PushID(n);

		// 非表示用チェックボックス
		char bufB[16];
		sprintf_s(bufB, "##bulind %d", object->GetID());
		bool isBlind = object->GetIsBlind();
		if (ImGui::Checkbox(bufB, &isBlind)) {
			object->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// テーブル設定
		char bufT[64];
		sprintf_s(bufT, "%s ##table %d", object->GetName().c_str() ,object->GetID());
		if (ImGui::Selectable(bufT, selected_object_id == object->GetID()))
		{
			// 選択
			selected_object_id = object->GetID();

		}
		// 入れ替え処理
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < objects.size())
			{
				// iterator入れ替え
				std::swap(objects[n], objects[n_next]);
				ImGui::ResetMouseDragDelta();
			}
		}

		// 子オブジェクト表示
		char bufC[64];
		sprintf_s(bufC, "Child ##child %d", object->GetName().c_str(), object->GetID());

		if (ImGui::TreeNode(bufC))
		{
			Hierarchy(object->GetChildren(), true);


			ImGui::TreePop();
		}


		n++;
		ImGui::PopID();
	}
}

void Editor::Hierarchy(std::vector<std::weak_ptr<GameObject>> &objects, bool is_child)
{
	int n = 0;
	for (const auto &object : objects)
	{
		// 親以外は後で表示する
		if ((object.lock()->GetPearent().lock() != nullptr) &&
			!is_child)
		{
			continue;
		}

		ImGui::PushID(n);

		// 非表示用チェックボックス
		char bufB[16];
		sprintf_s(bufB, "##bulind %d", object.lock()->GetID());
		bool isBlind = object.lock()->GetIsBlind();
		if (ImGui::Checkbox(bufB, &isBlind)) {
			object.lock()->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// テーブル設定
		char bufT[64];
		sprintf_s(bufT, "##table %d", object.lock()->GetID());
		if (ImGui::Selectable(bufT, selected_object_id == object.lock()->GetID()))
		{
			// 選択
			selected_object_id = object.lock()->GetID();

		}
		// 入れ替え処理
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < objects.size())
			{
				// iterator入れ替え
				std::swap(objects[n], objects[n_next]);
				ImGui::ResetMouseDragDelta();
			}
		}

		// 子オブジェクト表示
		if (ImGui::TreeNode(object.lock()->GetName().c_str()))
		{
			Hierarchy(object.lock()->GetChildren(), true);


			ImGui::TreePop();
		}


		n++;
		ImGui::PopID();
	}
}

void Editor::DrawInspector()
{
	ImGui::Begin("Inspector");

	auto *selectObject = nowScene->GetObjectManager()->GetGameObject(selected_object_id);

	// ID似合う物がなければ未表示
	if (selectObject == nullptr) {
		ImGui::End();
		return;
	}

	//インスペクター描画
	ImGui::PushID(selected_object_id);
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


	/*XMMATRIX test = XMMatrixIdentity();
	ImGuizmo::DrawGrid(
		Camera::GetCam()->GetViewMatrix().r->m128_f32,
		Camera::GetCam()->GetProjectionMatrix().r->m128_f32,
		test.r->m128_f32, 100);
	*/

	//ImGuizmo::DrawCubes(
	//	Camera::GetCam()->GetViewMatrix().r->m128_f32,
	//	Camera::GetCam()->GetProjectionMatrix().r->m128_f32,
	//	test.r->m128_f32, 20);

	ImGui::End();
}
