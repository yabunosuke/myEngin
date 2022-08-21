#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Math/Vector3.h"

class Transform :
	public Component
{
public:

	/// <summary>
	/// ���[�J����Ԃ��烏�[���h��Ԃւ̕ϊ�
	/// </summary>
	/// <param name="position">�ϊ����������W</param>
	/// <returns>Vector3 ���[���h���W</returns>
	static Vector3 TransformPoint(Vector3 position);

public:
	Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }


	// ���[�J�����W
	Vector3 local_position_		= { 0,0,0 };			// ���W
	XMFLOAT4 local_quaternion_	= { 0,1,0,0, };	// �N�I�[�^�j�I����]
	Vector3 local_scale_		= { 1,1,1 };			// �g��

	
private:
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ���[�J���s��
	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ���[���h�s��

	Transform *parent;

	// �s��X�V
	void UpdateMatrix();

	
};

