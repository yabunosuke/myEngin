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
	//�v���_�E�����j���[
	MainMenu();
	//�q�G�����L�[
	if (isHierarchy) {
		HierarchyWindow();
	}
	//�C���X�y�N�^�[
	if (isHierarchy) {
		DrawInspector();
	}
	// �o��
	if(true)
	{
		DrawMulutiRender();

	}

	//�f��
	if (isDemo) {
		ImGui::ShowDemoWindow();
	}
}

void Editor::MainMenu()
{
	if (ImGui::BeginMainMenuBar()) {
		//�t�@�C��
		if (ImGui::BeginMenu("File")) {
			FileMenu();
			ImGui::EndMenu();
		}

		//�ҏW
		if (ImGui::BeginMenu("Edit")) {
			EditMenu();
			ImGui::EndMenu();
		}

		//�Q�[���I�u�W�F�N�g
		if (ImGui::BeginMenu("GameObject")) {
			ImGui::EndMenu();
		}

		//�\������E�B���h�E
		if (ImGui::BeginMenu("Window")) {
			WindowMenu();
			ImGui::EndMenu();
		}

		//�w���v
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Editor::FileMenu()
{
	//�㏑���ۑ�
	if (ImGui::MenuItem("Save", "Ctrl+S")) {
	}
	//���O��t���ĕۑ�
	if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {
		FileManager::GetIns()->FileSave(SAVE_TYPE::SCENE);
	}

	ImGui::Separator();


	//�r���h�ݒ�
	if (ImGui::MenuItem("Build Settings...")) {
	}

	//�r���h�ݒ�
	if (ImGui::MenuItem("Build and Run", "Ctrl+B")) {
	}

	ImGui::Separator();
	//�I������
	if (ImGui::MenuItem("Exit")) {
		
	}
}

void Editor::EditMenu()
{
	// �R�s�[
	if (ImGui::MenuItem("Copy", "Ctrl+C")) {
	}
	// �J�b�g
	if (ImGui::MenuItem("Cut", "Ctrl+X")) {
	}
	// �y�[�X�g
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
	//�E�B���h�E����`
	ImGui::Begin("Hierarchy");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	ImGui::Text(nowScene.get()->GetName().c_str());
	/*nowScene.get()->GetObjectManager()->DrawHierarchy(selected_object_id);*/
	Hierarchy(nowScene.get()->GetObjectManager()->game_objects_);
	//�ǉ�
	if (ImGui::Button("add")) {
		nowScene->GetObjectManager()->CreateObject();
	}

	//�I��
	ImGui::End();

}

void Editor::Hierarchy(std::vector<std::shared_ptr<GameObject>>& objects, bool is_child)
{
	int n = 0;
	for (const auto &object : objects)
	{
		// �e�ȊO�͌�ŕ\������
		if ((object->GetPearent().lock() != nullptr) &&
			!is_child)
		{
			continue;
		}

		ImGui::PushID(n);

		// ��\���p�`�F�b�N�{�b�N�X
		char bufB[16];
		sprintf_s(bufB, "##bulind %d", object->GetID());
		bool isBlind = object->GetIsBlind();
		if (ImGui::Checkbox(bufB, &isBlind)) {
			object->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// �e�[�u���ݒ�
		char bufT[64];
		sprintf_s(bufT, "%s ##table %d", object->GetName().c_str() ,object->GetID());
		if (ImGui::Selectable(bufT, selected_object_id == object->GetID()))
		{
			// �I��
			selected_object_id = object->GetID();

		}
		// ����ւ�����
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < objects.size())
			{
				// iterator����ւ�
				std::swap(objects[n], objects[n_next]);
				ImGui::ResetMouseDragDelta();
			}
		}

		// �q�I�u�W�F�N�g�\��
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
		// �e�ȊO�͌�ŕ\������
		if ((object.lock()->GetPearent().lock() != nullptr) &&
			!is_child)
		{
			continue;
		}

		ImGui::PushID(n);

		// ��\���p�`�F�b�N�{�b�N�X
		char bufB[16];
		sprintf_s(bufB, "##bulind %d", object.lock()->GetID());
		bool isBlind = object.lock()->GetIsBlind();
		if (ImGui::Checkbox(bufB, &isBlind)) {
			object.lock()->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// �e�[�u���ݒ�
		char bufT[64];
		sprintf_s(bufT, "##table %d", object.lock()->GetID());
		if (ImGui::Selectable(bufT, selected_object_id == object.lock()->GetID()))
		{
			// �I��
			selected_object_id = object.lock()->GetID();

		}
		// ����ւ�����
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < objects.size())
			{
				// iterator����ւ�
				std::swap(objects[n], objects[n_next]);
				ImGui::ResetMouseDragDelta();
			}
		}

		// �q�I�u�W�F�N�g�\��
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

	// ID�����������Ȃ���Ζ��\��
	if (selectObject == nullptr) {
		ImGui::End();
		return;
	}

	//�C���X�y�N�^�[�`��
	ImGui::PushID(selected_object_id);
	selectObject->DrawInspector();
	ImGui::PopID();

	// �|�b�v�A�b�v�E�B���h�E
	// �R���|�[�l���g�̒ǉ�
	static bool openAddComponent = false;
	openAddComponent = ImGui::Button("Add Component");
	if (openAddComponent) {
		ImGui::OpenPopup("ComponentList");
	}

	// �R���|�[�l���g�O���[�v
	auto &component_groups = ComponentList::GetIns()->GetComponentGroup();
	// �R���|�[�l���g���X�g
	auto &component_list = ComponentList::GetIns()->GetComponentList();

	if (ImGui::BeginPopup("ComponentList")) {
		// �R���|�[�l���g�̌���
		static char searchComponent[64] = "";
		ImGui::InputText("##searchBox", searchComponent, 64);
		
		// �����{�b�N�X�ɉ��������Ă��Ȃ�������S�ĕ\��
		if (searchComponent[0] == '\0') {
			ImGui::Text("Component");

			//�O���[�v�P�ʂŕ\��
			for (auto &component_group : component_groups) {
				// �O���[�v���\��
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
				

				// �O���[�v���Ƃɕ\��
				if (ImGui::TreeNode(group_name.c_str())) {
					// �����Ă���R���|�[�l���g��\��
					for (auto &component : component_group.second) {
						if (ImGui::Button(component.c_str())) {
							// �R���|�[�l���g�ǉ�
							///selectObject->AddComponent<LightComponent>();
						}
					}
					ImGui::TreePop();
				}

				//switch (component.first)
				//{
				//case ComponentList::Group::Rendering:
				//	if(ImGui::TreeNode("Rendering")){

				//		//���g��\��
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

			//���ׂĕ\��
			for (auto &list : ComponentList::GetIns()->GetComponentList()) {
				if (ImGui::Button(list.first.c_str())) {
					list.second(selectObject);					
				}
			}
		}
		// ����
		else {
			// ��������string�ɕϊ�
			std::string searchComponentStrnig = searchComponent;
			std::transform(searchComponentStrnig.begin(), searchComponentStrnig.end(), searchComponentStrnig.begin(), std::tolower);
			ImGui::Text("Search");

			for (auto &list : ComponentList::GetIns()->GetComponentList()) {
				// ���X�g�̃R���|�[�l���g�����������ɕϊ�
				std::string componentName = list.first;
				std::transform(componentName.begin(), componentName.end(), componentName.begin(), std::tolower);
				//�ꕔ���v������̂�����Ε\��
				if (componentName.find(searchComponentStrnig) != std::string::npos) {
					//�ǉ�
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
	//�E�B���h�E����`
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
