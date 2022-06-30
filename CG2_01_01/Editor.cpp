#include "Editor.h"
#include "imguiManager.h"
#include "FileManager.h"
#include "ColliderComponent.h"

#include "TransformComponent.h"

#include "ComponentList.h"

#include <algorithm>

void Editor::Draw()
{
	//�v���_�E�����j���[
	MainMenu();
	//�q�G�����L�[
	if (isHierarchy) {
		DrawHierarchy();
	}
	//�C���X�y�N�^�[
	if (isHierarchy) {
		DrawInspector();
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
		exit(0);
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

void Editor::DrawHierarchy()
{
	//�E�B���h�E����`
	ImGui::Begin("Hierarchy");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	ImGui::Text(nowScene.get()->name.c_str());
	nowScene.get()->gameObjectManager.DrawHierarchy(selectedObjectNum);

	//�ǉ�
	if (ImGui::Button("add")) {
		nowScene->gameObjectManager.CreateObject();
	}

	//�I��
	ImGui::End();

}

void Editor::DrawInspector()
{
	ImGui::Begin("Inspector");
	//���I���̏ꍇ�͉����`�悵�Ȃ�
	if (selectedObjectNum < 0) {
		ImGui::End();
		return;
	}

	auto *selectObject = nowScene->gameObjectManager.gameObjects[selectedObjectNum].get();

	//�C���X�y�N�^�[�`��
	ImGui::PushID(selectedObjectNum);
	selectObject->DrawInspector();
	ImGui::PopID();

	// �|�b�v�A�b�v�E�B���h�E
	// �R���|�[�l���g�̒ǉ�
	static bool openAddComponent = false;
	openAddComponent = ImGui::Button("Add Component");
	if (openAddComponent) {
		ImGui::OpenPopup("ComponentList");
	}
	if (ImGui::BeginPopup("ComponentList")) {
		// �R���|�[�l���g�̌���
		static char searchComponent[64] = "";
		ImGui::InputText("##searchBox", searchComponent, 64);
		
		// �����{�b�N�X�ɉ��������Ă��Ȃ�������S�ĕ\��
		if (searchComponent[0] == '\0') {
			ImGui::Text("Component");

			//�O���[�v�P�ʂŕ\��
			for (auto &componentGroup : ComponentList::GetIns()->GetComponentGroup()) {
				switch (componentGroup.first)
				{
				case ComponentList::Group::Rendering:
					if(ImGui::TreeNode("Rendering")){

						//���g��\��
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
