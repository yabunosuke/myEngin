#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//更新
	for (auto &e : game_objects_) {
		e->Initialize();
	}
	
}

void GameObjectManager::FixedUpdate()
{//更新
	for (auto &e : game_objects_) {
		e->FixedUpdate();
	}
}

void GameObjectManager::Update()
{
	//更新
	for (auto &e : game_objects_) {
		e->Update();
	}
}

void GameObjectManager::LastUpdate()
{
	//更新
	for (auto &e : game_objects_) {
		e->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//描画
	for (auto &e : game_objects_) {
		e->Draw();
	}
}



GameObject *GameObjectManager::GetGameObject(int id)
{
	for (const auto &object : game_objects_) 
	{
		if (id == object->GetInstanceID()) {
			return object;
		}
	}

	return nullptr;
}

void GameObjectManager::Finalize()
{
}
