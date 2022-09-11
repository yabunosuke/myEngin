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
	//		�R���X�g���N�^
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
	//		�ÓI�����o�ϐ�
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
	//		�ÓI�֐�
	//
	//===========================================


	/// <summary>
	/// 2�_�Ԃ̊p�x�����߂�
	/// </summary>
	/// <param name="from">�p�x���̑��茳�ƂȂ�x�N�g��</param>
	/// <param name="to">�p�x���𑪒肷��x�N�g��</param>
	/// <returns>float �p�x(degree)</returns>
	static float Angle(const Vector2 &from, const Vector2 &to);


	/// <summary>
	/// �x�N�g���̑傫���𐧌�
	/// </summary>
	/// <param name="vector">�R�s�[����x�N�g��</param>
	/// <param name="max_length">�ő�̑傫��</param>
	/// <returns>�傫���� maxLength �܂łɐ������� vector �̃R�s�[</returns>
	static Vector2 ClampMagnitude(const Vector2 &vector, const float &max_length);

	/// <summary>
	/// 2�_�Ԃ̋��������߂�
	/// </summary>
	/// <returns>2�_�Ԃ̋���</returns>
	static float Distance(const Vector2 &lhs, const Vector2 &rhs);


	/// <summary>
	/// 2�̃x�N�g���̓���
	/// </summary>
	/// <returns>float ����</returns>
	static float Dot(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// 2�̃x�N�g���̊e���������ԑ傫�Ȓl�Ńx�N�g�����쐬
	/// </summary>
	/// <returns>�쐬�����x�N�g��</returns>
	static Vector2 Max(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// 2�̃x�N�g���̊e���������ԏ����Ȓl�Ńx�N�g�����쐬
	/// </summary>
	/// <returns>�쐬�����x�N�g��</returns>
	static Vector2 Min(const Vector2 &lhs, const Vector2 &rhs);

	/// <summary>
	/// �@���Œ�`���ꂽ���ʂŃx�N�g���𔽎�
	/// </summary>
	/// <param name="inDirection">���ʂɓ�������x�N�g��</param>
	/// <param name="inNormal">���ʂ��`���鐳�K���ς݂̐����x�N�g��</param>
	/// <returns>���K���ςݔ��˃x�N�g��</returns>
	static Vector2 Reflect(const Vector2 &inDirection, const Vector2 &inNormal);



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
	/// ���K���x�N�g��
	/// </summary>
	/// <returns>Vector3 ���K���x�N�g��</returns>
	Vector2 Normalized() const;

	/// <summary>
	/// �x�N�g���̒����̓��
	/// </summary>
	/// <returns>float �x�N�g���̒����̓��</returns>
	float SqrMagnitude() const;

	




	//===========================================
	//
	//	  ���Z�q
	//
	//===========================================

	// �P�����Z�q
	Vector2 operator +() const { return *this; }
	Vector2 operator -() const
	{
		Vector2 temp = {
			-this->x,
			-this->y
		};
		return temp;
	}


	// �񍀉��Z
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

	// ��r���Z�q
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

	// �Y�������Z�q
	float operator [](const std::size_t &index) const
	{
		if (index == 0) return this->x;
		if (index == 1) return this->y;

		// �͈͊O�Q��

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