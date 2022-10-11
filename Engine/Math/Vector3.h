#pragma once
#include <DirectXMath.h>
#include "Math/Mathf.h"
using namespace DirectX;


class Vector3 :
	public XMFLOAT3
{
public:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	Vector3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	};
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(const XMFLOAT3 &num) {
		this->x = num.x;
		this->y = num.y;
		this->z = num.z;
	}
	Vector3(const XMVECTOR &other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}


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
	static float Angle(const Vector3 &from, const Vector3 &to);

	/// <summary>
	/// 2つのベクトルの内積
	/// </summary>
	/// <returns>float 内積</returns>
	static float Dot(const Vector3 &lhs, const Vector3 &rhs);
	
	/// <summary>
	/// 2つのベクトルの外積
	/// </summary>
	/// <returns>外積</returns>
	static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);

	/// <summary>
	/// 正規化ベクトルの作成（書き換えあり）
	/// </summary>
	/// <returns>Vector3 作成した正規化ベクトル</returns>
	static Vector3 Normalize(Vector3 &valuse);

	/// <summary>
	/// 2つのベクトルの各成分から一番大きな値でベクトルを作成
	/// </summary>
	/// <returns>Vector3 作成したベクトル</returns>
	static Vector3 Max(const Vector3 &lhs, const Vector3 &rhs);

	/// <summary>
	/// 2つのベクトルの各成分から一番小さな値でベクトルを作成
	/// </summary>
	/// <returns>作成したベクトル</returns>
	static Vector3 Min(const Vector3 &lhs, const Vector3 &rhs);

	static Vector3 MoveTowards(const Vector3 &current, const Vector3 &target, float max_distance_delta);


	/// <summary>
	/// ベクトルの大きさを制限
	/// </summary>
	/// <param name="vector">コピーするベクトル</param>
	/// <param name="max_length">最大の大きさ</param>
	/// <returns>大きさを maxLength までに制限した vector のコピー</returns>
	static Vector3 ClampMagnitude(const Vector3 &vector, const float &max_length);

	/// <summary>
	/// 法線で定義された平面でベクトルを反射
	/// </summary>
	/// <param name="inDirection">平面に入る方向ベクトル</param>
	/// <param name="inNormal">平面を定義する正規化済みの垂直ベクトル</param>
	/// <returns>Vector3 作成した正規化</returns>
	static Vector3 Reflect(const Vector3 &inDirection, const Vector3 &inNormal);

	/// <summary>
	/// 2点間の距離を求める
	/// </summary>
	/// <returns>2点間の距離</returns>
	static float Distance(const Vector3 &lhs, const Vector3 &rhs);


	//===========================================
	//
	//		静的メンバ変数
	//
	//===========================================

	static const Vector3 forward;	// (  0,  0,  1 )
	static const Vector3 back;		// (  0,  0, -1 )
	static const Vector3 up;		// (  0,  1,  0 )
	static const Vector3 down;		// (  0, -1,  0 )
	static const Vector3 right;		// (  1,  0,  0 )
	static const Vector3 left;		// ( -1,  0,  0 )
	static const Vector3 zero;		// (  0,  0,  0 )
	static const Vector3 one;		// (  1,  1,  1 )
	static const Vector3 positive_infinity;		// (  inf,  inf,  inf )
	static const Vector3 negative_infinity;		// ( -inf, -inf, -inf )


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
	/// ベクトルの長さの二乗
	/// </summary>
	/// <returns>float ベクトルの長さの二乗</returns>
	float SqrMagnitude() const;

	/// <summary>
	/// 正規化ベクトル
	/// </summary>
	/// <returns>Vector3 正規化ベクトル</returns>
	Vector3 Normalized() const;



	//===========================================
	//
	//	  演算子
	//
	//===========================================

	// 単項演算子
	Vector3 operator +() const { return *this; }
	Vector3 operator -() const
	{
		Vector3 temp = {
			-this->x,
			-this->y,
			-this->z
		};
		return temp;
	}


	// 二項演算
	Vector3 operator +(const Vector3 &vector) const
	{
		Vector3 temp =
		{
			this->x + vector.x,
			this->y + vector.y,
			this->z + vector.z
		};
		return temp;
	}
	Vector3 operator -(const Vector3 &vector) const { return *this + -vector; }
	Vector3 &operator +=(const Vector3 &vector) {
		*this = *this + vector;
		return *this;
	}
	Vector3 &operator -=(const Vector3 &vector) {
		*this = *this - vector;
		return *this;
	}

	// 比較演算子
	bool operator ==(const Vector3 &vector) const
	{
		bool temp{
			x == vector.x &&
			y == vector.y &&
			z == vector.z };
		return temp;
	}
	bool operator !=(const Vector3 &vector) const { return !(*this == vector); }

	// 添え字演算子
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;
		if (index == 2) return this->z;

		// 範囲外参照
		
		return 0.0f;
	}
};



inline Vector3 operator *(const Vector3 &vector,const float &num) 
{
	Vector3 temp = {
		vector.x * num,
		vector.y * num,
		vector.z * num,
	};
	return temp;
}

inline  Vector3 operator *(const float &num, const Vector3 &vector)
{
	Vector3 temp = vector * num;
	
	return temp;
}

inline Vector3 operator /(const Vector3 &vector, float num) 
{
	Vector3 temp = {
		vector.x / num,
		vector.y / num,
		vector.z / num,
	};
	return temp;
}

inline Vector3 &operator *=(Vector3 &vector,const float &num)  {
	vector = vector * num;
	return vector;
}

inline Vector3 &operator /=(Vector3 &vector, const float &num) {
	vector = vector / num;
	return vector;
}