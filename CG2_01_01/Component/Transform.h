#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

class Transform :
	public Component
{
public:

	/// <summary>
	/// ���[�J����Ԃ��烏�[���h��Ԃւ̕ϊ�
	/// </summary>
	/// <param name="position">�ϊ����������W</param>
	/// <returns>Vector3 ���[���h���W</returns>

public:
	Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }


	
	Vector3 GetRight() {return Vector3(world_matrix_.r[0]).Normalized();}
	Vector3 GetUp() {return Vector3(world_matrix_.r[1]).Normalized();}
	Vector3 GetFront() {return Vector3(world_matrix_.r[2]).Normalized();}



	//===========================================
	//
	//	�A�N�Z�b�T
	//
	//===========================================


	/// <summary>
	/// ���[���h��Ԃ̐Ԏ� 
	/// (get = true, set = false)
	/// </summary>
	
	///// <summary>
	///// ���[���h��Ԃ̍��W (get = true, set = true)
	///// </summary>
	//Property<Vector3> position{
	//	world_position_, AccessorType::AllAccess,
	//	nullptr,
	//	// ���[�J���̍Čv�Z����
	//	nullptr
	//};
	///// <summary>
	///// ���[���h��Ԃ̍��W (get = true, set = true)
	///// </summary>
	//Property<Vector3> position{
	//	world_position_, AccessorType::AllAccess,
	//	nullptr,
	//	// ���[�J���̍Čv�Z����
	//	nullptr
	//};

	/// <summary>
	/// ���[���h��Ԃ̍��W (AllAccess)
	/// </summary>
	Property<Vector3> localPosition{
		local_position_, AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̉�] (AllAccess)
	/// </summary>
	Property<Quaternion> localQuaternion{
		local_quaternion_,AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̍��W (AllAccess)
	/// </summary>
	Property<Vector3> localScale{
		local_scale_, AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};

	/// <summary>
	/// ���[�J����Ԃ̃}�g���b�N�X (get = true, set = true)
	/// </summary>
	Property<XMMATRIX> local_matrix{
		local_matrix_,AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		//nullptr
		[this](XMMATRIX mat)
		{
			// Matrix���
			local_matrix_ = mat;

			// �e�v�f�Čv�Z
			MatrixDecompose(
				local_matrix_,
				local_scale_,
				local_quaternion_,
				local_position_
			);

			// ���[���h�s��Čv�Z
			if (parent_.expired()) {
				world_matrix_ = local_matrix_;
			}
			else {
				world_matrix_ =
					local_matrix_ * parent_.lock()->matrix;
			}

			// ���[���h�e�v�f�Čv�Z
			MatrixDecompose(
				world_matrix_,
				world_scale_,
				world_quaternion_,
				world_position_
			);
		}
	};



	/// <summary>
	/// ���[���h��Ԃ̍��W (get = true, set = true)
	/// </summary>
	Property<Vector3> position{
		world_position_, AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̉�] (get = true, set = true)
	/// </summary>
	Property<Quaternion> quaternion{
		world_quaternion_,AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̃X�P�[�� (get = true, set = true)
	/// </summary>
	Property<Vector3> scale
	{
		world_scale_,AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};

	/// <summary>
	/// ���[���h��Ԃ̃}�g���b�N�X (get = true, set = true)
	/// </summary>
	Property<XMMATRIX> matrix
	{
		world_matrix_,AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		//nullptr
		[this](XMMATRIX mat)
		{
			// �e�����Ȃ���΂��̂܂ܑ��
			if (parent_.expired())
			{
				// Matrix���
				world_matrix_ = mat;

				// ���[���h�e�v�f�Čv�Z
				MatrixDecompose(
					world_matrix_,
					world_scale_,
					world_quaternion_,
					world_position_
				);


				// ���[�J���������l
				local_matrix_ = mat;

				// �e�v�f�Čv�Z
				local_scale_ = world_scale_;
				local_quaternion_ = world_quaternion_;
				local_position_ = world_position_;

			}
			// �e������ꍇ�͍Čv�Z
			else
			{
				int a = 0;
			}
		}
	};

	std::weak_ptr<Transform> parent_;


private:
	// ���[�J�����W
	Vector3		local_position_ = { 0,0,0 };			// ���[�J�����W
	Quaternion	local_quaternion_ = { 0,1,0,0, };	// ���[�J����]�i�N�I�[�^�j�I���j
	Vector3		local_scale_ = { 1,1,1 };			// ���[�J���g��
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ���[�J���s��


	// ���[���h���W
	Vector3		world_position_ = { 0,0,0 };			// ���[���h���W
	Quaternion	world_quaternion_ = { 0,1,0,0, };		// ���[���h��]�i�N�I�[�^�j�I���j
	Vector3		world_scale_ = { 1,1,1 };				// ���[���h�g��
	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ���[���h�s��


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

