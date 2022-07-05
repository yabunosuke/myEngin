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

void GameObjectManager::Draw() const
{
	//�`��
	for (auto &e : gameObjects) {
		e->Draw();
	}
}

void GameObjectManager::DrawHierarchy(int &selectNum)
{

	//�I�u�W�F�N�g���ꗗ�\��
	for (int i = 0; i < gameObjects.size(); i++) {
		//��\���p�`�F�b�N�{�b�N�X
		char bufB[16];
		sprintf_s(bufB, "##ID %d", gameObjects[i].get()->GetID());
		bool isBlind = gameObjects[i].get()->GetIsBlind();
		if (ImGui::Checkbox(bufB, &isBlind)) {
			gameObjects[i].get()->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		//���O�̕\��
		char bufH[32];
		sprintf_s(bufH, "##Hierarchy %d", gameObjects[i].get()->GetID());
		if (ImGui::Selectable(bufH, selectNum == i)) {
			selectNum = i;
		}
		ImGui::SameLine();
		if (gameObjects[i].get()->GetIsActive()) {
			ImGui::Text(gameObjects[i].get()->GetName().c_str());
		}
		else {
			ImGui::TextDisabled(gameObjects[i].get()->GetName().c_str());
		}
	}
}


void GameObjectManager::Finalize()
{
}

void GameObjectManager::CreateObject()
{
	std::unique_ptr<GameObject> gameObject = std::make_unique<GameObject>("GameObject(" + std::to_string(gameObjects.size()) + ")");
	
	gameObjects.push_back(std::move(gameObject));
}
