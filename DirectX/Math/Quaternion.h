//#pragma once
//#include"Vector3.h"
//#include"Matrix4.h"
//
//struct Quaternion
//{
//	float x; //x成分
//	float y; //y成分
//	float z; //z成分
//	float w; //w成分
//
//	//コンストラクタ
//	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {
//		this->x = x;
//		this->y = y;
//		this->z = z;
//		this->w = w;
//	}
//	Quaternion(const Vector3 &v, float angle) {
//		float _sin = sin(angle / 2.0f);
//		Quaternion(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
//	}
//	Quaternion(const Vector3 &v, Quaternion &q) {
//		Quaternion qq = { 1,1,1,1 };
//		Quaternion qv = { 1,1,1,1 };
//
//	}
//
//};
//
//
////内積を求める
//float dot(const Quaternion &q1, const Quaternion &q2);
//
////ノルムを求める
//float length(const Quaternion &q);
//
////正規化する
//Quaternion normalize(const Quaternion &q);
//
////共役四次元数を求める
//Quaternion conjugate(const Quaternion &q);
//
////単項演算子オーバーロード
//Quaternion operator + (const Quaternion &q);
//Quaternion operator - (const Quaternion &q);
//
////代入演算子オーバーロード
//Quaternion &operator += (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator -= (Quaternion &q1, const Quaternion &q2);
//Quaternion &operator *= (Quaternion &q, float s);
//Quaternion &operator /= (Quaternion &q, float s);
//Quaternion &operator *= (Quaternion &q1, const Quaternion &q2);
//
////2項演算子オーバーロード
//Quaternion operator + (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator - (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q1, const Quaternion &q2);
//Quaternion operator * (const Quaternion &q, float s);
//Quaternion operator * (float s, const Quaternion &q);
//Quaternion operator / (const Quaternion &q, float s);
//
//Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t);
//Quaternion lerp(const Quaternion &q1, const Quaternion &q2, float t);
//Matrix4 rotate(const Quaternion &q);
//Quaternion quaternion(const Matrix4 &m);
//Vector3 getAxis(const Quaternion &q);