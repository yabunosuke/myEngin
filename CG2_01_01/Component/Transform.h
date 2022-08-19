#pragma once
#include <DirectXMath.h>
#include "Component/Component.h"
#include "Vector3.h"

class Transform :
	public Component
{
public:
	Transform();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;

	XMMATRIX GetMatrix() { return  matrix_; }
	
	Vector3 position_ = { 0,0,0 };			// 座標
	XMFLOAT4 quaternion_ = { 0,1,0,0, };	// クオータニオン回転
	Vector3 scale_ = { 1,1,1 };			// 拡大

	
private:
	XMMATRIX matrix_ = XMMatrixIdentity();		// 行列
	
};

