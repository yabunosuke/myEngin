#pragma once
#include "Math/Vector3.h"
#include "Object/Component/Collider/Collider.h"
#include <vector>
struct ContactPoint
{
	Vector3 normal;							// �ڐG�_�̖@��
	std::vector<Collider> other_collider;	// �ڐG���Ă��鑼�̃R���C�_�[
	Vector3 point;							// �ڐG�n�_
	Collider this_collider;					// �ŏ��ɐڐG�����R���C�_�[���
};