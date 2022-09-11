#pragma once
#include <DirectXMath.h>
#include "Math/Mathf.h"
#include "Math/Vector3.h"
using namespace DirectX;

class Vector2 :
	public XMFLOAT2
{
public:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	Vector2() = default;
	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
	Vector2(const XMFLOAT3 &num) {
		this->x = num.x;
		this->y = num.y;
	}
	Vector2(const XMVECTOR &other) {
		XMVECTOR temp = other;
		XMStoreFloat2(this, temp);
	}


	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	static const Vector2 up;		// (  0,  1 )
	static const Vector2 down;		// (  0, -1 )
	static const Vector2 right;		// (  1,  0 )
	static const Vector2 left;		// ( -1,  0 )
	static const Vector2 zero;		// (  0,  0 )
	static const Vector2 one;		// (  1,  1 )
	static const Vector2 positiveInfinity;		// (  inf,  inf )
	static const Vector2 negativeInfinity;		// ( -inf, -inf )


	//===========================================
	//
	//		静的関数
	//
	//===========================================


	/// <summary>
	/// 2点間の角度を求める
	/// </summary>
	/// <param name="from">角度差の測定元となるベクトル</param>
	/// <param name="to">角度差を測定するベクトル</param>
	/// <returns>float 角度(degree)</returns>
	static float Angle(const Vector2 &from, const Vector2 &to);


	/// <summary>
	/// ベクトルの大きさを制限
	/// </summary>
	/// <param name="vector">コピーするベクトル</param>
	/// <param name="max_length">最大の大きさ</param>
	/// <returns>大きさを maxLength までに制限した vector のコピー</returns>
	static Vector2 ClampMagnitude(const Vector2 &vector, const float &max_length);

	/// <summary>
	/// 2点間の距離を求める
	/// </summary>
	/// <returns>2点間の距離</returns>
	static float Distance(const Vector2 &lhs, const Vector2 &rhs);


	/// <summary>
	/// 2つのベクトルの内積
	/// </summary>
	/// <returns>float 内積</returns>
	static float Dot(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// 2つのベクトルの各成分から一番大きな値でベクトルを作成
	/// </summary>
	/// <returns>作成したベクトル</returns>
	static Vector2 Max(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// 2つのベクトルの各成分から一番小さな値でベクトルを作成
	/// </summary>
	/// <returns>作成したベクトル</returns>
	static Vector2 Min(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// 法線で定義された平面でベクトルを反射
	/// </summary>
	/// <param name="inDirection">平面に入る方向ベクトル</param>
	/// <param name="inNormal">平面を定義する正規化済みの垂直ベクトル</param>
	/// <returns>正規化済み反射ベクトル</returns>
	static Vector2 Reflect(const Vector2 &inDirection, const Vector2 &inNormal);



	//===========================================
	//
	//	  メンバ関数
	//
	//===========================================

	/// <summary>
	/// ベクトルの長さ
	/// </summary>
	/// <returns>float ベクトルの長さ</returns>
	float Magnitude() const;

	/// <summary>
	/// 正規化ベクトル
	/// </summary>
	/// <returns>Vector3 正規化ベクトル</returns>
	Vector2 Normalized() const;

	/// <summary>
	/// ベクトルの長さの二乗
	/// </summary>
	/// <returns>float ベクトルの長さの二乗</returns>
	float SqrMagnitude() const;

	




	//===========================================
	//
	//	  演算子
	//
	//===========================================

	// 単項演算子
	Vector2 operator +() const { return *this; }
	Vector2 operator -() const
	{
		Vector2 temp = {
			-this->x,
			-this->y
		};
		return temp;
	}


	// 二項演算
	Vector2 operator +(const Vector2 &vector) const
	{
		Vector2 temp =
		{
			this->x + vector.x,
			this->y + vector.y
		};
		return temp;
	}
	Vector2 operator -(const Vector2 &vector) const
	{
		return *this + -vector; 
	}

	Vector2 &operator +=(const Vector2 &vector) {
		*this = *this + vector;
		return *this;
	}
	Vector2 &operator -=(const Vector2 &vector) {
		*this = *this - vector;
		return *this;
	}

	// 比較演算子
	bool operator ==(const Vector2 &vector) const
	{
		bool temp
		{
			x == vector.x &&
			y == vector.y 
		};
		return temp;
	}
	bool operator !=(const Vector2 &vector) const 
	{ 
		return !(*this == vector); 
	}

	// 添え字演算子
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;

		// 範囲外参照

		return 0.0f;
	}
};



inline Vector2 operator *(const Vector2 &vector, const float &num)
{
	Vector2 temp = {
		vector.x * num,
		vector.y * num
	};
	return temp;
}

inline  Vector2 operator *(const float &num, const Vector2 &vector)
{
	Vector2 temp = vector * num;

	return temp;
}

inline Vector2 operator /(const Vector2 &vector, float num)
{
	Vector2 temp = {
		vector.x / num,
		vector.y / num
	};
	return temp;
}

inline Vector2 &operator *=(Vector2 &vector, const float &num) {
	vector = vector * num;
	return vector;
}

inline Vector2 &operator /=(Vector2 &vector, const float &num) {
	vector = vector / num;
	return vector;
}