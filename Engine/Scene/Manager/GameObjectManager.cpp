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

GameObject *GameObjectManager::GetGameObject(const std::string &name)
{
	for (const auto &object : game_objects_)
	{
		if (name == object->name.r_) {
			return object;
		}
	}

	return nullptr;
}

void GameObjectManager::AddObjects()
{
	if(add_objects_.size() != 0)
	{
		for(int i =  0; i < add_objects_.size();++i)
		{
			game_objects_.emplace_back(add_objects_[i]);
		}
		add_objects_.clear();
	}

}

void GameObjectManager::Finalize()
{
}
