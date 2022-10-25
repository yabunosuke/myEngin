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
	/// Pointを中心にaxisでangle分回転
	/// </summary>
	/// <param name="point">対象になるワールド座標</param>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度</param>
	void RotateAround(Vector3 point, Vector3 axis, float deg);

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }

	Vector3 GetRight() { return Vector3(world_matrix_.r[0]).Normalized(); }
	Vector3 GetUp() { return Vector3(world_matrix_.r[1]).Normalized(); }
	Vector3 GetFront() { return Vector3(world_matrix_.r[2]).Normalized(); }

	void LookAt(const Vector3 &target);


	//===========================================
	//
	//	アクセッサ
	//
	//===========================================


	/// <summary>
	/// ワールド空間の座標 (AllAccess)
	/// </summary>
	yEngine::Property<Vector3> localPosition{
		&local_position_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		nullptr
	};
	/// <summary>
	/// ワールド空間の回転 (AllAccess)
	/// </summary>
	yEngine::Property<Quaternion> localQuaternion{
		&local_quaternion_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
	/// <summary>
	/// ワールド空間の座標 (AllAccess)
	/// </summary>
	yEngine::Property<Vector3> localScale{
		&local_scale_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};

	/// <summary>
	/// ローカル空間のマトリックス (get = true, set = true)
	/// </summary>
	yEngine::Property<XMMATRIX> local_matrix{
		&local_matrix_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		//nullptr
		[this](XMMATRIX mat)
		{
			// Matrix代入
			local_matrix_ = mat;

			// 各要素再計算
			MatrixDecompose(
				local_matrix_,
				local_scale_,
				local_quaternion_,
				local_position_
			);

			// ワールド行列再計算
			if (parent_ == nullptr) {
				world_matrix_ = local_matrix_;
			}
			else {
				world_matrix_ =
					local_matrix_ * parent_->matrix;
			}

			// ワールド各要素再計算
			MatrixDecompose(
				world_matrix_,
				world_scale_,
				world_quaternion_,
				world_position_
			);
		}
	};



	/// <summary>
	/// ワールド空間の座標 (get = true, set = true)
	/// </summary>
	yEngine::Property<Vector3> position
	{
		&world_position_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
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
	/// ワールド空間の回転 (get = true, set = true)
	/// </summary>
	yEngine::Property<Quaternion> quaternion
	{
		&world_quaternion_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		//nullptr
		[this](Quaternion assignment_quaternion)
		{

			// ワールド行列を計算
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
	/// ワールド空間のスケール (get = true, set = true)
	/// </summary>
	yEngine::Property<Vector3> scale
	{
		&world_scale_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		// ローカルの再計算処理
		[this](Vector3 assignment_scale)
		{

			// ワールド行列を計算
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
	/// ワールド空間のマトリックス (get = true, set = true)
	/// </summary>
	yEngine::Property<XMMATRIX> matrix
	{
		&world_matrix_,
		yEngine::AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
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
	// ローカル座標
	Vector3		local_position_ = { 0,0,0 };			// ローカル座標
	Quaternion	local_quaternion_ = { 0,0,0,0, };	// ローカル回転（クオータニオン）
	Vector3		local_scale_ = { 1,1,1 };			// ローカル拡大
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ローカル行列


	// ワールド座標
	Vector3		world_position_ = { 0,0,0 };			// ワールド座標
	Quaternion	world_quaternion_ = { 0,1,0,0, };		// ワールド回転（クオータニオン）
	Vector3		world_scale_ = { 1,1,1 };				// ワールド拡大
	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ワールド行列






	// 行列更新
	void UpdateMatrix();

	//行列から各要素に変換
	void MatrixDecompose(
		const XMMATRIX &matrix,
		XMFLOAT3 &scale,
		XMFLOAT4 &quaternion,
		XMFLOAT3 &position
	);

	// 全ての親の逆行列を取得する
	XMMATRIX InverseMatrixAllParent();
};