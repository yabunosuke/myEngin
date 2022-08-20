#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//�X�V
	for (auto &e : gameObjects) {
		e->Initialize();
	}
	
}

void GameObjectManager::Update()
{
	//�X�V
	for (auto &e : gameObjects) {
		e->Update();
	}
}

void GameObjectManager::LastUpdate()
{
	//�X�V
	for (auto &e : gameObjects) {
		e->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//�`��
	for (auto &e : gameObjects) {
		e->Draw();
	}
}

void GameObjectManager::DrawHierarchy(int &select_id)
{
	int n = 0;
	for (const auto &object : gameObjects) 
	{
		ImGui::PushID(n);

		// ��\���p�`�F�b�N�{�b�N�X
		bool isBlind = object->GetIsBlind();
		if (ImGui::Checkbox("##bulind button", &isBlind)) {
			object->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// �e�[�u���ݒ�
		if(ImGui::Selectable(object->GetName().c_str(), select_id == object->GetID()))
		{
			select_id = object->GetID();
		}

		// ����ւ�����
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < gameObjects.size())
			{
				// iterator����ւ�
				gameObjects[n_next] = gameObjects[n];
				//gameObjects[n_next].reset(&temp);
				ImGui::ResetMouseDragDelta();
			}
		}
		n++;
		ImGui::PopID();
	}


	////�I�u�W�F�N�g���ꗗ�\��
	//for (int i = 0; i < gameObjects.size(); i++) {
	//	//��\���p�`�F�b�N�{�b�N�X
	//	char bufB[16];
	//	sprintf_s(bufB, "##ID %d", gameObjects[i].get()->GetID());
	//	bool isBlind = gameObjects[i].get()->GetIsBlind();
	//	if (ImGui::Checkbox(bufB, &isBlind)) {
	//		gameObjects[i].get()->SetIsBlind(isBlind);
	//	}
	//	ImGui::SameLine();

	//	//���O�̕\��
	//	char bufH[32];
	//	sprintf_s(bufH, "##Hierarchy %d", gameObjects[i].get()->GetID());
	//	if (ImGui::Selectable(bufH, selectNum == i)) {
	//		selectNum = i;
	//	}
	//	ImGui::SameLine();
	//	if (gameObjects[i].get()->GetIsActive()) {
	//		ImGui::Text(gameObjects[i].get()->GetName().c_str());
	//	}
	//	else {
	//		ImGui::TextDisabled(gameObjects[i].get()->GetName().c_str());
	//	}
	//}
}


GameObject *GameObjectManager::GetGameObject(int id)
{
	for (const auto &object : gameObjects) 
	{
		if (id == object->GetID()) {
			return object.get();
		}
	}

	return nullptr;
}

void GameObjectManager::Finalize()
{
}


std::weak_ptr<GameObject> GameObjectManager::CreateObject(std::string object_name)
{
	std::shared_ptr<GameObject> gameObject;
	// ���O�������Ă��Ȃ����
	if (object_name.size() == 0) {
		gameObject = std::make_shared<GameObject>("GameObject(" + std::to_string(gameObjects.size()) + ")");
	}
	else {
		gameObject = std::make_shared<GameObject>(object_name);
	}

	return gameObjects.emplace_back(std::move(gameObject));
}
