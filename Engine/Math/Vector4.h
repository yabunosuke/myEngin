#pragma once
#include <DirectXMath.h>

class Vector4:
	public DirectX::XMFLOAT4
{

public:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	Vector4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	};
	Vector4(float x, float y, float z,float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Vector4(const XMFLOAT4 &num)
	{
		*this = num;
	}
	Vector4(const DirectX::XMVECTOR &other)
	{
		DirectX::XMVECTOR temp = other;
		XMStoreFloat4(this, temp);
	}

	//===========================================
	//
	//	  演算子
	//
	//===========================================

	// 添え字演算子
	float operator [](const int &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;
		if (index == 2) return this->z;
		if (index == 3) return this->w;

		// 範囲外参照

		return 0.0f;
	}
};

