#include "SetBoneParent.h"
#include "Object3dComponent.h"

SetBoneParent::SetBoneParent(const std::string &boneName):
	MonoBehaviour("SetParent"),
	bone_name_(boneName)
{
}

void SetBoneParent::Start()
{

	// ゲームオブジェクトの親のゲームオブジェクトの
	// モデルのボーンのトランスフォームを取得
	hand_bone_ = &(game_object_->GetPearent()
		->GetComponent<Object3dComponent>()->
		GetObjectData()->FindNode(bone_name_.c_str())->world_transform);
	game_object_->transform_->user_set_parent_ = hand_bone_;
}
