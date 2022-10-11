#pragma once
#include <DirectXMath.h>

#include "Math/Vector3.h"

class Quaternion : public XMFLOAT4
{
public:
	//	**************
	//
	//	  �R���X�g���N�^
	//
	//	**************
	
	// �������w�肵�ăN�I�[�^�j�I���𐶐��i�񐄏��j
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// �C�ӎ�����̉�]�̎w�肩��N�H�[�^�j�I�����쐬
	Quaternion(const Vector3 &axis_normal, float degree) {
		float _sin = sin(degree / 2.0f);
		*this = Quaternion(_sin * axis_normal.x, _sin * axis_normal.y, _sin * axis_normal.z, cos(degree / 2.0f));
	}

	//	************
	//
	//	  �ÓI�֐�
	//
	//	************

	/// <summary>
	/// 2�̉�]�̊Ԃ̊p�x��Ԃ�
	/// </summary>
	/// <param name="from">�p�x���̑��茳�ƂȂ�N�I�[�^�j�I��</param>
	/// <param name="to">�p�x���𑪒肷��N�I�[�^�j�I��</param>
	/// <returns>float �p�x(degree)</returns>
	static float Angle(const Quaternion &from, const Quaternion &to);


	/// <summary>
	/// 2�̉�]�̓��ς�Ԃ�
	/// </summary>
	/// <returns>float ����(degree)</returns>
	static float Dot(const Quaternion &lhs, const Quaternion &rhs);

	/// <summary>
	/// �I�C���[���N�I�[�^�j�I���ɕϊ�
	/// </summary>
	/// <returns>Quaternion �ϊ������l</returns>
	static Quaternion Euler(const float &roll, const float &pitch, const float &yaw);
	/// <summary>
	/// �I�C���[���N�I�[�^�j�I���ɕϊ�
	/// </summary>
	/// <param name="rotation_vector">��]�x�N�g���idegree�j</param>
	/// <returns>Quaternion �ϊ������l</returns>
	static Quaternion Euler(const Vector3 &rotation_vector);

	/// <summary>
	/// �N�I�[�^�j�I���𐳋K������i������������j
	/// </summary>
	/// <returns>Quaternion ���K�������N�I�[�^�j�I��</returns>
	static Quaternion Normalize(Quaternion &quaternion);



	//===========================================
	//
	//	  �����o�֐�
	//
	//===========================================

	/// <summary>
	/// ���K�������N�I�[�^�j�I����Ԃ��i���������Ȃ��j
	/// </summary>
	/// <returns>Quaternion ���K�������N�I�[�^�j�I��</returns>
	Quaternion Normalized() const;


	/// <summary>
	/// �N�I�[�^�j�I������I�C���[�p�\����Ԃ�
	/// </summary>
	/// <returns>Vector3 �I�C���[�p�\��</returns>
	Vector3 EulerAngles() const;

	//===========================================
	//
	//	���Z�q
	//
	//===========================================


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

	// ��r���Z�q
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