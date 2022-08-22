#pragma once
#include <DirectXMath.h>

#include "DirectXCommon.h"
#include "Math/Vector3.h"

struct Quaternion : XMFLOAT4
{
	//	**************
	//
	//	  �R���X�g���N�^
	//
	//	**************

	// �f�t�H���g�R���X�g���N�^
	Quaternion() = default;

	// �������w�肵�ăN�I�[�^�j�I���𐶐�
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// �C�ӎ�����̉�]�̎w�肩��N�H�[�^�j�I�����쐬
	Quaternion(const Vector3 &axis_normal, float degree) {
		float _sin = sin(degree / 2.0f);
		Quaternion(_sin * axis_normal.x, _sin * axis_normal.y, _sin * axis_normal.z, cos(degree / 2.0f));
	}


	//	**********
	//
	//	  ���Z�q
	//
	//	**********


	//�@�P�����Z�q�I�[�o�[���[�h
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

	//������Z�q�I�[�o�[���[�h
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
		this->x * q2.w + this->y * q2.z - this->z * q2.y + this->w * q2.x, //x����
		-this->x * q2.z + this->y * q2.w + this->z * q2.x + this->w * q2.y, //y����
		this->x * q2.y - this->y * q2.x + this->z * q2.w + this->w * q2.z, //z����
		-this->x * q2.x + -this->y * q2.y - this->z * q2.z + this->w * q2.w //w����
		};
		*this = temp;

		return *this;
	}
	// �Y�������Z�q
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;
		if (index == 2) return this->z;
		if (index == 2) return this->w;

		// �͈͊O�Q��

		return 0.0f;
	}
};

// �񍀉��Z�q�I�[�o�[���[�h
Quaternion operator+(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result += q2;
}
Quaternion operator-(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result -= q2;
}
Quaternion operator*(const Quaternion &q1, const Quaternion &q2)
{
	Quaternion result = q1;
	return result *= q2;
}
Quaternion operator*(const Quaternion &q, float s)
{
	Quaternion result = q;
	return result *= s;
}
Quaternion operator*(float s, const Quaternion &q)
{
	Quaternion result = q;
	return result *= s;
}
Quaternion operator/(const Quaternion &q, float s)
{
	Quaternion result = q;
	return result /= s;
}


//public:
//	// �f�t�H���g�R���X�g���N�^
//	Quaternion() {
//		this->x = 0.0f;
//		this->y = 0.0f;
//		this->z = 0.0f;
//		this->w = 1.0f;
//	}
//
//	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
//		this->x = x;
//		this->y = y;
//		this->z = z;
//		this->w = w;
//	}
//
//	Quaternion(const Vector3 &v, float angle) {
//		float _sin = sin(angle / 2.0f);
//		Quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
//	}
////	Quaternion(const Vector3 &v, Quaternion &q) {
////		Quaternion qq = { 1,1,1,1 };
////		Quaternion qv = { 1,1,1,1 };
////
////	}
////
//};
////
////
//////���ς����߂�
////float dot(const Quaternion &q1, const Quaternion &q2);
////
//////�m���������߂�
////float length(const Quaternion &q);
////
//////���K������
////Quaternion normalize(const Quaternion &q);
////
//////�����l�����������߂�
////Quaternion conjugate(const Quaternion &q);
////
//////�P�����Z�q�I�[�o�[���[�h
////Quaternion operator + (const Quaternion &q);
////Quaternion operator - (const Quaternion &q);
////
//////������Z�q�I�[�o�[���[�h
////Quaternion &operator += (Quaternion &q1, const Quaternion &q2);
////Quaternion &operator -= (Quaternion &q1, const Quaternion &q2);
////Quaternion &operator *= (Quaternion &q, float s);
////Quaternion &operator /= (Quaternion &q, float s);
////Quaternion &operator *= (Quaternion &q1, const Quaternion &q2);
////
//////2�����Z�q�I�[�o�[���[�h
////Quaternion operator + (const Quaternion &q1, const Quaternion &q2);
////Quaternion operator - (const Quaternion &q1, const Quaternion &q2);
////Quaternion operator * (const Quaternion &q1, const Quaternion &q2);
////Quaternion operator * (const Quaternion &q, float s);
////Quaternion operator * (float s, const Quaternion &q);
////Quaternion operator / (const Quaternion &q, float s);
////
////Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t);
////Quaternion lerp(const Quaternion &q1, const Quaternion &q2, float t);
////Matrix4 rotate(const Quaternion &q);
////Quaternion quaternion(const Matrix4 &m);
////Vector3 getAxis(const Quaternion &q);