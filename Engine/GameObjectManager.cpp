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


std::weak_ptr<GameObject> GameObjectManager::CreateObject(std::string object_name)
{
	std::weak_ptr<GameObject> gameObject;
	// 名前が入っていなければ
	if (object_name.size() == 0) {
		gameObject = Object::CreateObject<GameObject>("GameObject(" + std::to_string(game_objects_.size()) + ")");
	}
	else {
		gameObject = Object::CreateObject<GameObject>(object_name);
	}
	// 生成時にトランスフォーム
	gameObject.lock()->transform = gameObject.lock()->AddComponent<Transform>();


	return game_objects_.emplace_back(gameObject);
}
