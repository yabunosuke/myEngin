#include "GameObjectManager.h"
void GameObjectManager::Initialize()
{
	//�X�V
	for (auto &e : game_objects_) {
		e.lock()->Initialize();
	}
	
}

void GameObjectManager::FixedUpdate()
{//�X�V
	for (auto &e : game_objects_) {
		e.lock()->FixedUpdate();
	}
}

void GameObjectManager::Update()
{
	//�X�V
	for (auto &e : game_objects_) {
		e.lock()->Update();
		
	}
}

void GameObjectManager::LastUpdate()
{
	//�X�V
	for (auto &e : game_objects_) {
		e.lock()->LastUpdate();
	}
}

void GameObjectManager::Draw() const
{
	//�`��
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
