#include "Weapon.h"
#include "Object/GameObject/GameObject.h"
#include "Object3dComponent.h"
Weapon::Weapon():
	ScriptComponent("Weapon")
{
	// ゲームオブジェクトの親のゲームオブジェクトの
	// モデルのボーンのトランスフォームを取得
	bone_ = &(game_object_->GetPearent().lock()
		->GetComponent<Object3dComponent>().lock()->
		GetObjectData()->FindNode("LeftHand")->world_transform);
}

void Weapon::Update()
{
}

void Weapon::FixedUpdate()
{
	XMMATRIX bone_matrix = DirectX::XMLoadFloat4x4(bone_);
	transform_.lock()->matrix =
		bone_matrix * transform_.lock()->matrix;
}
