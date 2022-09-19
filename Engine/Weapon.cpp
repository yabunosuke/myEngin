#include "Weapon.h"
#include "Object/GameObject/GameObject.h"
#include "Object3dComponent.h"
Weapon::Weapon():
	MonoBehaviour("Weapon")
{
}

void Weapon::Start()
{
	// �Q�[���I�u�W�F�N�g�̐e�̃Q�[���I�u�W�F�N�g��
	// ���f���̃{�[���̃g�����X�t�H�[�����擾
	hand_bone_ = &(game_object_.lock()->GetPearent().lock()
		->GetComponent<Object3dComponent>().lock()->
		GetObjectData()->FindNode("mixamorig:RightHand")->world_transform);
	game_object_.lock()->transform->lock()->localScale = { 25,25,25 };
	game_object_.lock()->transform->lock()->localPosition = { -50,-40,4 };
	game_object_.lock()->transform->lock()->localQuaternion = Quaternion::Euler(0,0.001f,0);
	game_object_.lock()->transform->lock()->user_set_parent_ = hand_bone_;
}

