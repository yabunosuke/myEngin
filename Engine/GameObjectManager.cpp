#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//�X�V
	for (auto &e : game_objects_) {
		e->Initialize();
	}
	
}

void GameObjectManager::FixedUpdate()
{//�X�V
	for (auto &e : game_objects_) {
		e->FixedUpdate();
	}
}

void GameObjectManager::Update()
{
	//�X�V
	for (auto &e : game_objects_) {
		e->Update();
	}
}

void GameObjectManager::LastUpdate()
{
	//�X�V
	for (auto &e : game_objects_) {
		e->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//�`��
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
