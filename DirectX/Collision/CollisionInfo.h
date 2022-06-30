#pragma once

#include <DirectXMath.h>
#include <string>
class GameObjCommon;
class BaseCollider;

struct CollisionInfo // �Փˏ��
{
public:
	CollisionInfo(GameObjCommon *object, BaseCollider *collider, const DirectX::XMVECTOR &inter, std::string myName)
	{
		this->object = object;
		this->collider = collider;
		this->inter = inter;
		this->myName = myName;
	}

	//�Փˑ���̃I�u�W�F�N�g
	GameObjCommon *object = nullptr;
	//�Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	//�Փ˂��������̃R���C�_�[�̖��O
	std::string myName;
	//�Փ˓_
	DirectX::XMVECTOR inter;
};