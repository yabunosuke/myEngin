#pragma once
#include <DirectXMath.h>

#include "Math/Vector3.h"

class Quaternion : public XMFLOAT4
{
public:
	//	**************
	//
	//	  コンストラクタ
	//
	//	**************
	
	// 成分を指定してクオータニオンを生成（非推奨）
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// 任意軸周りの回転の指定からクォータニオンを作成
	Quaternion(const Vector3 &axis_normal, float degree) {
		float _sin = sin(degree / 2.0f);
		*this = Quaternion(_sin * axis_normal.x, _sin * axis_normal.y, _sin * axis_normal.z, cos(degree / 2.0f));
	}

	//	************
	//
	//	  静的関数
	//
	//	************

	/// <summary>
	/// 2つの回転の間の角度を返す
	/// </summary>
	/// <param name="from">角度差の測定元となるクオータニオン</param>
	/// <param name="to">角度差を測定するクオータニオン</param>
	/// <returns>float 角度(degree)</returns>
	static float Angle(const Quaternion &from, const Quaternion &to);


	/// <summary>
	/// 2つの回転の内積を返す
	/// </summary>
	/// <returns>float 内積(degree)</returns>
	static float Dot(const Quaternion &lhs, const Quaternion &rhs);

	/// <summary>
	/// オイラーをクオータニオンに変換
	/// </summary>
	/// <returns>Quaternion 変換した値</returns>
	static Quaternion Euler(const float &roll, const float &pitch, const float &yaw);
	/// <summary>
	/// オイラーをクオータニオンに変換
	/// </summary>
	/// <param name="rotation_vector">回転ベクトル（degree）</param>
	/// <returns>Quaternion 変換した値</returns>
	static Quaternion Euler(const Vector3 &rotation_vector);

	/// <summary>
	/// クオータニオンを正規化する（書き換えあり）
	/// </summary>
	/// <returns>Quaternion 正規化したクオータニオン</returns>
	static Quaternion Normalize(Quaternion &quaternion);



	//===========================================
	//
	//	  メンバ関数
	//
	//===========================================

	/// <summary>
	/// 正規化したクオータニオンを返す（書き換えなし）
	/// </summary>
	/// <returns>Quaternion 正規化したクオータニオン</returns>
	Quaternion Normalized() const;


	/// <summary>
	/// クオータニオンからオイラー角表現を返す
	/// </summary>
	/// <returns>Vector3 オイラー角表現</returns>
	Vector3 EulerAngles() const;

	//===========================================
	//
	//	演算子
	//
	//===========================================


	//　単項演算子オーバーロード
	Quaternion operator + ()
	{
		return *this;
	}
	Quaternion operator - ()
	{
		Quaternion temp = 
		{
			-this->x,
			-this->y,
			-this->z,
			-this->w
		};

		return temp;
	}

	//代入演算子オーバーロード
	Quaternion &operator += (const Quaternion &quaternion)
	{
		this->x += quaternion.x;
		this->y += quaternion.y;
		this->z += quaternion.z;
		this->w += quaternion.w;

		return *this;
	}
	Quaternion &operator -= (const Quaternion &quaternion)
	{
		this->x -= quaternion.x;
		this->y -= quaternion.y;
		this->z -= quaternion.z;
		this->w -= quaternion.w;

		return *this;
	}
	Quaternion &operator *= (float s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}
	Quaternion &operator /= (float s)
	{
		return *this *= 1.0f / s;
	}
	Quaternion &operator *= (const Quaternion &q2)
	{
		Quaternion temp{
		this->x * q2.w + this->y * q2.z - this->z * q2.y + this->w * q2.x, //x成分
		-this->x * q2.z + this->y * q2.w + this->z * q2.x + this->w * q2.y, //y成分
		this->x * q2.y - this->y * q2.x + this->z * q2.w + this->w * q2.z, //z成分
		-this->x * q2.x + -this->y * q2.y - this->z * q2.z + this->w * q2.w //w成分
		};
		*this = temp;

		return *this;
	}

	// 比較演算子
	bool operator == (const Quaternion &quaternion)const
	{
		bool temp{
			x == quaternion.x &&
			y == quaternion.y &&
			z == quaternion.z &&
			w == quaternion.w
		};
		return temp;
	}
	bool operator != (const Quaternion &quaternion)const
	{
		return !(*this == quaternion);
	}


	// 添え字演算子
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;
		if (index == 2) return this->z;
		if (index == 2) return this->w;

		// 範囲外参照

		return 0.0f;
	}
};

// 二項演算子オーバーロード
inline Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result += q2;
}
inline Quaternion operator-(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result -= q2;
}
inline Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result *= q2;
}
inline Quaternion operator*(const Quaternion &q, float s)
{
	Quaternion result = q;
	return result *= s;
}
inline Quaternion operator*(float s, const Quaternion &q)
{
	Quaternion result = q;
	return result *= s;
}
inline Quaternion operator/(const Quaternion &q, float s)
{
	Quaternion result = q;
	return result /= s;
}


inline Vector3 operator *(const Quaternion &rotation, const Vector3 &point)
{
	float x = rotation.x * 2.0f;
	float y = rotation.y * 2.0f;
	float z = rotation.z * 2.0f;
	float xx = rotation.x * x;
	float yy = rotation.y * y;
	float zz = rotation.z * z;
	float xy = rotation.x * y;
	float xz = rotation.x * z;
	float yz = rotation.y * z;
	float wx = rotation.w * x;
	float wy = rotation.w * y;
	float wz = rotation.w * z;

	Vector3 res;
	res.x = (1.0f - (yy + zz)) * point.x + (xy - wz) * point.y + (xz + wy) * point.z;
	res.y = (xy + wz) * point.x + (1.0f - (xx + zz)) * point.y + (yz - wx) * point.z;
	res.z = (xz - wy) * point.x + (yz + wx) * point.y + (1.0f - (xx + yy)) * point.z;
	return res;
}