#pragma once

#include <DirectXMath.h>

class GameObjCommon;
class BaseCollider;

struct QueryHit // �N�G���[�ɂ����𓾂�ׂ̍\����
{
	// �Փˑ���̃I�u�W�F�N�g
	GameObjCommon *object = nullptr;
	// �Փˑ���̃R���C�_�[
	BaseCollider* collider = nullptr;
	// �Փ˓_
	DirectX::XMVECTOR inter;
	// �r�˃x�N�g��
	DirectX::XMVECTOR reject;
};

class QueryCallback // �N�G���[�Ō��������o�������̓�����K�肷��N���X
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	// �������R�[���o�b�N
	virtual bool OnQueryHit(const QueryHit& info) = 0;
};