#include "BoxCollider.h"

BoxCollider::BoxCollider(Quaternion q, Vector3 extent, Vector3 center)
{
	local_extent_ = extent;
	local_center_ = center;
	collision_type_ = CollisonType::OBB;
}

void BoxCollider::ComponentInitialize()
{
	game_object_->AddCollider(this);
}

void BoxCollider::ComponentUpdate()
{
	// ���a�ɃX�P�[���K��
	extent = Vector3::Scale(local_extent_, transform_->scale);
	// �I�t�Z�b�g�K��
	center = local_center_ + transform_->position;


}