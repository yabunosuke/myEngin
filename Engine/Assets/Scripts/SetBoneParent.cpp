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
	hand_bone_ = &(game_object_.lock()->GetPearent().lock()
		->GetComponent<Object3dComponent>().lock()->
		GetObjectData()->FindNode(bone_name_.c_str())->world_transform);
	game_object_.lock()->transform->lock()->user_set_parent_ = hand_bone_;
}
