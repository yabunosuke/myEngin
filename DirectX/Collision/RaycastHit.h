#pragma once

#include "BaseCollider.h"
#include <DirectXMath.h>

class GameObjCommon;

struct RaycastHit
{
	// �Փˑ���̃I�u�W�F�N�g
	GameObjCommon *object = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider *collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter;
	// �Փ˓_�܂ł̋���
	float distance = 0.0f;
};