#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Math/Vector3.h"

class Transform :
	public Component
{
public:
	Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }
	//XMMATRIX GetLocalMatrix() { return  local_matrix_; }


	// ���[���h���
	Vector3 position_ = { 0,0,0 };			// ���W
	XMFLOAT4 quaternion_ = { 0,1,0,0, };	// �N�I�[�^�j�I����]
	Vector3 scale_ = { 1,1,1 };				// �g��

	// ���[�J�����
	Vector3 local_position_ = { 0,0,0 };				// ���W
	XMFLOAT4 local_quaternion_ = { 0,1,0,0, };	// �N�I�[�^�j�I����]
	Vector3 local_scale_ = { 1,1,1 };					// �g��


private:

	void UpdateMatrix();

	

	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ���[���h��ԍs��
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ���[�J����ԍs��
	
};

