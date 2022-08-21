#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//更新
	for (auto &e : game_objects_) {
		e->Initialize();
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

void GameObjectManager::SetPearentChild(std::weak_ptr<GameObject> pearent, std::weak_ptr<GameObject> child)
{
	pearent.lock()->SetCildrenObject(child);
	child.lock()->SetPearentObject(pearent);
}


GameObject *GameObjectManager::GetGameObject(int id)
{
	for (const auto &object : game_objects_) 
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
		gameObject = std::make_shared<GameObject>("GameObject(" + std::to_string(game_objects_.size()) + ")");
	}
	else {
		gameObject = std::make_shared<GameObject>(object_name);
	}

	return game_objects_.emplace_back(std::move(gameObject));
}
