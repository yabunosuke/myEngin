#pragma once
#include <DirectXMath.h>
#include "Object/Component/Component.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4x4.h"

class Transform :
	public Component
{
public:
	Transform();
	~Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	/// <summary>
	/// Point�𒆐S��axis��angle����]
	/// </summary>
	/// <param name="point">�ΏۂɂȂ郏�[���h���W</param>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x</param>
	void RotateAround(Vector3 point, Vector3 axis, float deg);

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }

	Vector3 GetRight() { return Vector3(world_matrix_.r[0]).Normalized(); }
	Vector3 GetUp() { return Vector3(world_matrix_.r[1]).Normalized(); }
	Vector3 GetFront() { return Vector3(world_matrix_.r[2]).Normalized(); }

	void LookAt(const Vector3 &target);


	//===========================================
	//
	//	�A�N�Z�b�T
	//
	//===========================================


	/// <summary>
	/// ���[���h��Ԃ̍��W (AllAccess)
	/// </summary>
	yEngine::Property<Vector3> localPosition{
		&local_position_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̉�] (AllAccess)
	/// </summary>
	yEngine::Property<Quaternion> localQuaternion{
		&local_quaternion_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};
	/// <summary>
	/// ���[���h��Ԃ̍��W (AllAccess)
	/// </summary>
	yEngine::Property<Vector3> localScale{
		&local_scale_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		nullptr
	};

	/// <summary>
	/// ���[�J����Ԃ̃}�g���b�N�X (get = true, set = true)
	/// </summary>
	yEngine::Property<XMMATRIX> local_matrix{
		&local_matrix_,
		yEngine::AccessorType::AllAccess,
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
			if (parent_ == nullptr) {
				world_matrix_ = local_matrix_;
			}
			else {
				world_matrix_ =
					local_matrix_ * parent_->matrix;
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
	yEngine::Property<Vector3> position
	{
		&world_position_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		[this](Vector3 assignment_position)
		{
			XMMATRIX temp = world_matrix_;
			temp.r[3] = {
				assignment_position.x,
				assignment_position.y,
				assignment_position.z,
				1.0f
			};

			matrix = temp;
		}
	};

	/// <summary>
	/// ���[���h��Ԃ̉�] (get = true, set = true)
	/// </summary>
	yEngine::Property<Quaternion> quaternion
	{
		&world_quaternion_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		//nullptr
		[this](Quaternion assignment_quaternion)
		{

			// ���[���h�s����v�Z
			XMMATRIX S = DirectX::XMMatrixScaling(
				world_scale_.x,
				world_scale_.y,
				world_scale_.z
			);
			XMMATRIX R = DirectX::XMMatrixRotationQuaternion(
				XMLoadFloat4(&assignment_quaternion)
			);

			XMMATRIX T = DirectX::XMMatrixTranslation(
				world_position_.x,
				world_position_.y,
				world_position_.z
			);

			XMMATRIX temp = S * R * T;

			matrix = temp;

		}
	};
	/// <summary>
	/// ���[���h��Ԃ̃X�P�[�� (get = true, set = true)
	/// </summary>
	yEngine::Property<Vector3> scale
	{
		&world_scale_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		// ���[�J���̍Čv�Z����
		[this](Vector3 assignment_scale)
		{

			// ���[���h�s����v�Z
			XMMATRIX S = DirectX::XMMatrixScaling(
				assignment_scale.x,
				assignment_scale.y,
				assignment_scale.z
			);
			XMMATRIX R = DirectX::XMMatrixRotationQuaternion(
				XMLoadFloat4(&world_quaternion_)
			);

			XMMATRIX T = DirectX::XMMatrixTranslation(
				world_position_.x,
				world_position_.y,
				world_position_.z
			);

			XMMATRIX temp = S * R * T;

			matrix = temp;
		}
	};

	/// <summary>
	/// ���[���h��Ԃ̃}�g���b�N�X (get = true, set = true)
	/// </summary>
	yEngine::Property<XMMATRIX> matrix
	{
		&world_matrix_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ���[�J���̍Čv�Z����
		//nullptr
		[this](XMMATRIX mat)
		{
			world_matrix_ = mat;
			MatrixDecompose(
				world_matrix_,
				world_scale_,
				world_quaternion_,
				world_position_
			);

			local_matrix_ = mat * InverseMatrixAllParent();

			MatrixDecompose(
				local_matrix_,
				local_scale_,
				local_quaternion_,
				local_position_
			);
		}
	};

	Transform *parent_{ nullptr };

	XMFLOAT4X4 *user_set_parent_{ nullptr };

private:
	// ���[�J�����W
	Vector3		local_position_ = { 0,0,0 };			// ���[�J�����W
	Quaternion	local_quaternion_ = { 0,0,0,0, };	// ���[�J����]�i�N�I�[�^�j�I���j
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

	// �S�Ă̐e�̋t�s����擾����
	XMMATRIX InverseMatrixAllParent();
};