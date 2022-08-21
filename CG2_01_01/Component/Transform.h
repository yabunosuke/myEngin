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
	Vector3		local_position_		= { 0,0,0 };			// ���[�J�����W
	XMFLOAT4	local_quaternion_	= { 0,1,0,0, };	// ���[�J����]�i�N�I�[�^�j�I���j
	Vector3		local_scale_		= { 1,1,1 };			// ���[�J���g��

	// ���[���h���W
	Vector3		world_position_		= { 0,0,0 };			// ���[���h���W
	XMFLOAT4	world_quaternion_	= { 0,1,0,0, };	// ���[���h��]�i�N�I�[�^�j�I���j
	Vector3		world_scale_		 = { 1,1,1 };			// ���[���h�g��

	
private:
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ���[�J���s��
	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ���[���h�s��

	Transform *parent;

	// �s��X�V
	void UpdateMatrix();

	 //�s�񂩂�e�v�f�ɕϊ�
	void MatrixDecompose(
		const XMMATRIX &matrix,
		XMFLOAT3 &scale,
		XMFLOAT4 &quaternion,
		XMFLOAT3 &position
	);
	
};

