#include "Weapon.h"
#include "Object/GameObject/GameObject.h"
#include "Object3dComponent.h"
Weapon::Weapon():
	ScriptComponent("Weapon")
{
}

void Weapon::Initialize()
{
	// ゲームオブジェクトの親のゲームオブジェクトの
	// モデルのボーンのトランスフォームを取得
	hand_bone_ = &(game_object_->GetPearent().lock()
		->GetComponent<Object3dComponent>().lock()->
		GetObjectData()->FindNode("mixamorig:RightHand")->world_transform);
	game_object_->transform->lock()->localScale = { 25,25,25 };
	game_object_->transform->lock()->localPosition = { -50,-40,4 };
	game_object_->transform->lock()->localQuaternion = Quaternion::Euler(0,0.001f,0);
	game_object_->transform->lock()->user_set_parent_ = hand_bone_;
}

void Weapon::Update()
{
}

void Weapon::FixedUpdate()
{
}

void Weapon::OnCollisionEnter()
{
	int a = 0;
}
