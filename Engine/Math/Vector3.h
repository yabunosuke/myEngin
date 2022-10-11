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
	//		�R���X�g���N�^
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
	//		�ÓI�֐�
	//
	//===========================================

	/// <summary>
	/// 2�_�Ԃ̊p�x�����߂�
	/// </summary>
	/// <param name="from">�p�x���̑��茳�ƂȂ�x�N�g��</param>
	/// <param name="to">�p�x���𑪒肷��x�N�g��</param>
	/// <returns>float �p�x(degree)</returns>
	static float Angle(const Vector3 &from, const Vector3 &to);

	/// <summary>
	/// 2�̃x�N�g���̓���
	/// </summary>
	/// <returns>float ����</returns>
	static float Dot(const Vector3 &lhs, const Vector3 &rhs);
	
	/// <summary>
	/// 2�̃x�N�g���̊O��
	/// </summary>
	/// <returns>�O��</returns>
	static Vector3 Cross(const Vector3 &lhs, const Vector3 &rhs);

	/// <summary>
	/// ���K���x�N�g���̍쐬�i������������j
	/// </summary>
	/// <returns>Vector3 �쐬�������K���x�N�g��</returns>
	static Vector3 Normalize(Vector3 &valuse);

	/// <summary>
	/// 2�̃x�N�g���̊e���������ԑ傫�Ȓl�Ńx�N�g�����쐬
	/// </summary>
	/// <returns>Vector3 �쐬�����x�N�g��</returns>
	static Vector3 Max(const Vector3 &lhs, const Vector3 &rhs);

	/// <summary>
	/// 2�̃x�N�g���̊e���������ԏ����Ȓl�Ńx�N�g�����쐬
	/// </summary>
	/// <returns>�쐬�����x�N�g��</returns>
	static Vector3 Min(const Vector3 &lhs, const Vector3 &rhs);

	static Vector3 MoveTowards(const Vector3 &current, const Vector3 &target, float max_distance_delta);


	/// <summary>
	/// �x�N�g���̑傫���𐧌�
	/// </summary>
	/// <param name="vector">�R�s�[����x�N�g��</param>
	/// <param name="max_length">�ő�̑傫��</param>
	/// <returns>�傫���� maxLength �܂łɐ������� vector �̃R�s�[</returns>
	static Vector3 ClampMagnitude(const Vector3 &vector, const float &max_length);

	/// <summary>
	/// �@���Œ�`���ꂽ���ʂŃx�N�g���𔽎�
	/// </summary>
	/// <param name="inDirection">���ʂɓ�������x�N�g��</param>
	/// <param name="inNormal">���ʂ��`���鐳�K���ς݂̐����x�N�g��</param>
	/// <returns>Vector3 �쐬�������K��</returns>
	static Vector3 Reflect(const Vector3 &inDirection, const Vector3 &inNormal);

	/// <summary>
	/// 2�_�Ԃ̋��������߂�
	/// </summary>
	/// <returns>2�_�Ԃ̋���</returns>
	static float Distance(const Vector3 &lhs, const Vector3 &rhs);


	//===========================================
	//
	//		�ÓI�����o�ϐ�
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
	//	  �����o�֐�
	//
	//===========================================

	/// <summary>
	/// �x�N�g���̒���
	/// </summary>
	/// <returns>float �x�N�g���̒���</returns>
	float Magnitude() const;

	/// <summary>
	/// �x�N�g���̒����̓��
	/// </summary>
	/// <returns>float �x�N�g���̒����̓��</returns>
	float SqrMagnitude() const;

	/// <summary>
	/// ���K���x�N�g��
	/// </summary>
	/// <returns>Vector3 ���K���x�N�g��</returns>
	Vector3 Normalized() const;



	//===========================================
	//
	//	  ���Z�q
	//
	//===========================================

	// �P�����Z�q
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


	// �񍀉��Z
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

	// ��r���Z�q
	bool operator ==(const Vector3 &vector) const
	{
		bool temp{
			x == vector.x &&
			y == vector.y &&
			z == vector.z };
		return temp;
	}
	bool operator !=(const Vector3 &vector) const { return !(*this == vector); }

	// �Y�������Z�q
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;
		if (index == 2) return this->z;

		// �͈͊O�Q��
		
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