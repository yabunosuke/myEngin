#pragma once
#include <DirectXMath.h>

#include "Vector3.h"
#include "Quaternion.h"
#include "Math/Mathf.h"

class Matrix4x4 :
	public DirectX::XMFLOAT4X4
{
public:
	//===========================================
	//
	//		コンストラクタ
	//
	//===========================================

	Matrix4x4()
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0;
	}
	Matrix4x4(
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44
	)
	{
		this->_11 = _11;this->_12 = _12;this->_13 = _13;this->_14 = _14;
		this->_21 = _21;this->_22 = _22;this->_23 = _23;this->_24 = _24;
		this->_31 = _31;this->_32 = _32;this->_33 = _33;this->_34 = _34;
		this->_41 = _41;this->_42 = _42;this->_43 = _43;this->_44 = _44;
	}
	Matrix4x4(const XMFLOAT4X4 &num)
	{
		*this = num;
	}
	//===========================================
	//
	//		静的関数
	//
	//===========================================

	/// <summary>
	/// 座標から平行移動行列を作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 Translation(const Vector3 &translation);

	/// <summary>
	/// スケールから拡大行列を作成
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static Matrix4x4 Scale(const Vector3 &scale);

	/// <summary>
	/// クオータニオンから回転行列を作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 Rotate(const Quaternion &quaternion);
	
	static Quaternion GetRotation(const Matrix4x4 &rotate_matrix);

	static Matrix4x4 Inverse(const Quaternion &quaternion);


	//===========================================
	//
	//		静的変数
	//
	//===========================================

	static const Matrix4x4 identity;	// 恒等行列
	static const Matrix4x4 zero;		// 全ての要素を0にした行列


	//===========================================
	//
	//　　　メンバ関数
	//
	//===========================================

	Matrix4x4 ExtractTranslation();
	Matrix4x4 ExtractScale();
	Matrix4x4 ExtractRotation();

	//===========================================
	//
	//	  演算子
	//
	//===========================================

	operator XMMATRIX() const
	{
		XMMATRIX xm_matrix =
		{
		 _11,  _12,  _13,  _14,
		 _21,  _22,  _23,  _24,
		 _31,  _32,  _33,  _34,
		 _41,  _42,  _43,  _44
		};
		return xm_matrix;
	}
};

Matrix4x4 &operator *=(Matrix4x4 &rhs,const Matrix4x4 &lhs);
Matrix4x4 &operator *(const Matrix4x4 &rhs,const Matrix4x4 &lhs);
