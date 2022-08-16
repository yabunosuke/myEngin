#include "Editor.h"
#include "ImGui/imguiManager.h"
#include "FileManager.h"

#include "TransformComponent.h"

#include "ComponentList.h"

#include <algorithm>

#include "DirectXCommon.h"

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

void Editor::DrawHierarchy()
{
	//�E�B���h�E����`
	ImGui::Begin("Hierarchy");

	ImGui::SetWindowSize(
		ImVec2(400, 500),
		ImGuiCond_::ImGuiCond_FirstUseEver
	);

	ImGui::Text(nowScene.get()->GetName().c_str());
	nowScene.get()->GetObjectManager()->DrawHierarchy(selectedObjectNum);

	//�ǉ�
	if (ImGui::Button("add")) {
		nowScene->GetObjectManager()->CreateObject();
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

	auto *selectObject = nowScene->GetObjectManager()->gameObjects[selectedObjectNum].get();

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

	D3D12_GPU_DESCRIPTOR_HANDLE my_texture_gpu_handle = 
		nowScene->GetMulutiRenderTarget()->get()->descriputor_heap_SRV_->GetGPUDescriptorHandleForHeapStart();
	UINT handle_increment = DirectXCommon::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//my_texture_gpu_handle.ptr += (handle_increment * 1);
	ImGui::Image(
		(ImTextureID)my_texture_gpu_handle.ptr,
		ImVec2((float)100, (float)100)
	);
	//�I��
	ImGui::End();
}
