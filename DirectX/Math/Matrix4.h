//#pragma once
//#include <DirectXMath.h>
//#include "Vector3.h"
//
//using namespace DirectX;
//
//struct Matrix4 :public XMMATRIX {
//	Matrix4() {
//		*this = XMMatrixIdentity();
//	}
//
//	//‘ã“ü
//	XMFLOAT4X4 &operator=(const XMMATRIX &other) {
//		XMFLOAT4X4 f4_4;
//		XMStoreFloat4x4(&f4_4, other);
//		return f4_4;
//	}
//};
//
////Šg‘å
//Matrix4 scale(const Vector3 &s);
//Matrix4 identity();
//
//Matrix4 rotateX(float angle);
//Matrix4 rotateY(float angle);
//Matrix4 rotateZ(float angle);
//Matrix4 rotate(float x, float y, float z);
//
//Matrix4 translate(const Vector3 &t);
//
//Vector3 transform(const Vector3 &v, const Matrix4 &m);
//Vector3 &operator * (const Vector3 &v, const Matrix4 &m);
//
//
