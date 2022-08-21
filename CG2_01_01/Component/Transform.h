#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Math/Vector3.h"

class Transform :
	public Component
{
public:

	/// <summary>
	/// ローカル空間からワールド空間への変換
	/// </summary>
	/// <param name="position">変換したい座標</param>
	/// <returns>Vector3 ワールド座標</returns>
	static Vector3 TransformPoint(Vector3 position);

public:
	Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	XMMATRIX GetWorldMatrix() { return  world_matrix_; }


	// ローカル座標
	Vector3		local_position_		= { 0,0,0 };			// ローカル座標
	XMFLOAT4	local_quaternion_	= { 0,1,0,0, };	// ローカル回転（クオータニオン）
	Vector3		local_scale_		= { 1,1,1 };			// ローカル拡大

	// ワールド座標
	Vector3		world_position_		= { 0,0,0 };			// ワールド座標
	XMFLOAT4	world_quaternion_	= { 0,1,0,0, };	// ワールド回転（クオータニオン）
	Vector3		world_scale_		 = { 1,1,1 };			// ワールド拡大

	
private:
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ローカル行列
	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ワールド行列

	Transform *parent;

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

