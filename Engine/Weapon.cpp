#include "Weapon.h"
#include "Object/GameObject/GameObject.h"
#include "Object3dComponent.h"
Weapon::Weapon():
	MonoBehaviour("Weapon")
{
}

void Weapon::Start()
{

	// ゲームオブジェクトの親のゲームオブジェクトの
	// モデルのボーンのトランスフォームを取得
	hand_bone_ = &(game_object_->GetPearent()
		->GetComponent<Object3dComponent>()->
		GetObjectData()->FindNode("mixamorig:RightHand")->world_transform);
	game_object_->transform_->localScale = { 25,25,25 };
	game_object_->transform_->localPosition = { -66,-8,11 };
	game_object_->transform_->localQuaternion = Quaternion::Euler(0,4.05f*Mathf::deg_to_rad,-27.0f * Mathf::deg_to_rad);
	game_object_->transform_->user_set_parent_ = hand_bone_;
}

