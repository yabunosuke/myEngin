#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//更新
	for (auto &e : gameObjects) {
		e->Initialize();
	}
	
}

void GameObjectManager::Update()
{
	//更新
	for (auto &e : gameObjects) {
		e->Update();
	}
}

void GameObjectManager::LastUpdate()
{
	//更新
	for (auto &e : gameObjects) {
		e->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//描画
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

		// 非表示用チェックボックス
		bool isBlind = object->GetIsBlind();
		if (ImGui::Checkbox("##bulind button", &isBlind)) {
			object->SetIsBlind(isBlind);
		}
		ImGui::SameLine();

		// テーブル設定
		if(ImGui::Selectable(object->GetName().c_str(), select_id == object->GetID()))
		{
			select_id = object->GetID();
		}

		// 入れ替え処理
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = n + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < gameObjects.size())
			{
				// iterator入れ替え
				gameObjects[n_next] = gameObjects[n];
				//gameObjects[n_next].reset(&temp);
				ImGui::ResetMouseDragDelta();
			}
		}
		n++;
		ImGui::PopID();
	}


	////オブジェクトを一覧表示
	//for (int i = 0; i < gameObjects.size(); i++) {
	//	//非表示用チェックボックス
	//	char bufB[16];
	//	sprintf_s(bufB, "##ID %d", gameObjects[i].get()->GetID());
	//	bool isBlind = gameObjects[i].get()->GetIsBlind();
	//	if (ImGui::Checkbox(bufB, &isBlind)) {
	//		gameObjects[i].get()->SetIsBlind(isBlind);
	//	}
	//	ImGui::SameLine();

	//	//名前の表示
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
	// 名前が入っていなければ
	if (object_name.size() == 0) {
		gameObject = std::make_shared<GameObject>("GameObject(" + std::to_string(gameObjects.size()) + ")");
	}
	else {
		gameObject = std::make_shared<GameObject>(object_name);
	}

	return gameObjects.emplace_back(std::move(gameObject));
}
