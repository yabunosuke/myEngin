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
	/// ローカル空間からワールド空間への変換
	/// </summary>
	/// <param name="position">変換したい座標</param>
	/// <returns>Vector3 ワールド座標</returns>

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
	//	アクセッサ
	//
	//===========================================


	/// <summary>
	/// ワールド空間の赤軸 
	/// (get = true, set = false)
	/// </summary>
	
	///// <summary>
	///// ワールド空間の座標 (get = true, set = true)
	///// </summary>
	//Property<Vector3> position{
	//	world_position_, AccessorType::AllAccess,
	//	nullptr,
	//	// ローカルの再計算処理
	//	nullptr
	//};
	///// <summary>
	///// ワールド空間の座標 (get = true, set = true)
	///// </summary>
	//Property<Vector3> position{
	//	world_position_, AccessorType::AllAccess,
	//	nullptr,
	//	// ローカルの再計算処理
	//	nullptr
	//};

	/// <summary>
	/// ワールド空間の座標 (AllAccess)
	/// </summary>
	Property<Vector3> localPosition{
		local_position_, AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
	/// <summary>
	/// ワールド空間の回転 (AllAccess)
	/// </summary>
	Property<Quaternion> localQuaternion{
		local_quaternion_,AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
	/// <summary>
	/// ワールド空間の座標 (AllAccess)
	/// </summary>
	Property<Vector3> localScale{
		local_scale_, AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};

	/// <summary>
	/// ローカル空間のマトリックス (get = true, set = true)
	/// </summary>
	Property<XMMATRIX> local_matrix{
		local_matrix_,AccessorType::AllAccess,
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
			if (parent_.expired()) {
				world_matrix_ = local_matrix_;
			}
			else {
				world_matrix_ =
					local_matrix_ * parent_.lock()->matrix;
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
	Property<Vector3> position{
		world_position_, AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
	/// <summary>
	/// ワールド空間の回転 (get = true, set = true)
	/// </summary>
	Property<Quaternion> quaternion{
		world_quaternion_,AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};
	/// <summary>
	/// ワールド空間のスケール (get = true, set = true)
	/// </summary>
	Property<Vector3> scale
	{
		world_scale_,AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		nullptr
	};

	/// <summary>
	/// ワールド空間のマトリックス (get = true, set = true)
	/// </summary>
	Property<XMMATRIX> matrix
	{
		world_matrix_,AccessorType::AllAccess,
		nullptr,
		// ローカルの再計算処理
		//nullptr
		[this](XMMATRIX mat)
		{
			// 親がいなければそのまま代入
			if (parent_.expired())
			{
				// Matrix代入
				world_matrix_ = mat;

				// ワールド各要素再計算
				MatrixDecompose(
					world_matrix_,
					world_scale_,
					world_quaternion_,
					world_position_
				);


				// ローカルも同じ値
				local_matrix_ = mat;

				// 各要素再計算
				local_scale_ = world_scale_;
				local_quaternion_ = world_quaternion_;
				local_position_ = world_position_;

			}
			// 親がいる場合は再計算
			else
			{
				int a = 0;
			}
		}
	};

	std::weak_ptr<Transform> parent_;


private:
	// ローカル座標
	Vector3		local_position_ = { 0,0,0 };			// ローカル座標
	Quaternion	local_quaternion_ = { 0,1,0,0, };	// ローカル回転（クオータニオン）
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
	
};

