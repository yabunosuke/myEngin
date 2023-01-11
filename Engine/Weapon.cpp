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
	hand_bone_ = &(game_object_.lock()->GetPearent().lock()
		->GetComponent<Object3dComponent>().lock()->
		GetObjectData()->FindNode("Weapon.R")->world_transform);
	game_object_.lock()->transform->lock()->localScale = { 0.7f,0.7f ,0.7f };
	game_object_.lock()->transform->lock()->localPosition = { 1.2f,-1.4f,-0.15f };
	game_object_.lock()->transform->lock()->localQuaternion = Quaternion::Euler(-12.6f * Mathf::deg_to_rad,168.2f*Mathf::deg_to_rad,11.4 * Mathf::deg_to_rad);
	game_object_.lock()->transform->lock()->user_set_parent_ = hand_bone_;
}

