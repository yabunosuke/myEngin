#include "GameObjectManager.h"


void GameObjectManager::Initialize()
{
	//更新
	for (auto &e : game_objects_) {
		//e.lock()->Initialize();
	}
	
}

void GameObjectManager::FixedUpdate()
{
	//更新
	for (auto &e : game_objects_) {
		e.lock()->FixedUpdate();
	}
}

void GameObjectManager::Update()
{
	//更新
	for (auto &e : game_objects_) {
		e.lock()->Update();
	}
}

void GameObjectManager::LastUpdate()
{
	//更新
	for (auto &e : game_objects_) {
		e.lock()->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//描画
	for (auto &e : game_objects_) {
		e.lock()->Draw();
	}
}



std::weak_ptr<GameObject> GameObjectManager::GetGameObject(int id)
{
	for (const auto &object : game_objects_) 
	{
		if (id == object.lock()->GetInstanceID()) {
			return object;
		}
	}

	return std::weak_ptr<GameObject>();
}

std::weak_ptr<GameObject> GameObjectManager::GetGameObject(const std::string &name)
{
	for (const auto &object : game_objects_)
	{
		if (name == object.lock()->name.r_) {
			return object;
		}
	}

	return std::weak_ptr<GameObject>();
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
