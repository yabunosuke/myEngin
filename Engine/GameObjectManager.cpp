#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//更新
	for (auto &e : game_objects_) {
		e.lock()->Initialize();
	}
	
}

void GameObjectManager::FixedUpdate()
{//更新
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

void GameObjectManager::SetPearentChild(std::weak_ptr<GameObject> pearent, std::weak_ptr<GameObject> child)
{
	pearent.lock()->SetCildrenObject(child);
	child.lock()->SetPearentObject(pearent);
}


GameObject *GameObjectManager::GetGameObject(int id)
{
	for (const auto &object : game_objects_) 
	{
		if (id == object.lock()->GetInstanceID()) {
			return object.lock().get();
		}
	}

	return nullptr;
}

void GameObjectManager::Finalize()
{
}
