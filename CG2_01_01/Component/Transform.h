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


	// ワールド空間
	Vector3 position_ = { 0,0,0 };			// 座標
	XMFLOAT4 quaternion_ = { 0,1,0,0, };	// クオータニオン回転
	Vector3 scale_ = { 1,1,1 };				// 拡大

	// ローカル空間
	Vector3 local_position_ = { 0,0,0 };				// 座標
	XMFLOAT4 local_quaternion_ = { 0,1,0,0, };	// クオータニオン回転
	Vector3 local_scale_ = { 1,1,1 };					// 拡大


private:

	void UpdateMatrix();

	

	XMMATRIX world_matrix_ = XMMatrixIdentity();		// ワールド空間行列
	XMMATRIX local_matrix_ = XMMatrixIdentity();		// ローカル空間行列
	
};

