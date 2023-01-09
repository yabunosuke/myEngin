#include "SetBoneParent.h"
#include "Object3dComponent.h"

SetBoneParent::SetBoneParent(const std::string &boneName):
	MonoBehaviour("SetParent"),
	bone_name_(boneName)
{
}

void SetBoneParent::Start()
{

	// �Q�[���I�u�W�F�N�g�̐e�̃Q�[���I�u�W�F�N�g��
	// ���f���̃{�[���̃g�����X�t�H�[�����擾
	hand_bone_ = &(game_object_->GetPearent()
		->GetComponent<Object3dComponent>()->
		GetObjectData()->FindNode(bone_name_.c_str())->world_transform);
	game_object_->transform_->user_set_parent_ = hand_bone_;
}
