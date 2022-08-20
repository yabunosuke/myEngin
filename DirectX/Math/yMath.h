#pragma once
#define _USE_MATH_DIFINES
#include <cmath>
#include "Math/Vector3.h"
#include <DirectXMath.h>

//0~360度の範囲に収める
static float ChangeDegree360(float deg) {
	deg = fmodf(deg, 360.0f);
	if (0.0f > deg) {
		deg += 360.0f;
	}

	return deg;
}

//ラジアン→度
static XMFLOAT3 RadToDeg(const DirectX::XMFLOAT3 &rad) {
	XMFLOAT3 deg = {
		rad.x * (180.0f / XM_PI),
		rad.y * (180.0f / XM_PI),
		rad.z * (180.0f / XM_PI),
	};

	return deg;
}

//度→ラジアン
static XMFLOAT3 DegToRad(const DirectX::XMFLOAT3 &deg) {
	XMFLOAT3 rad = {
		deg.x * (XM_PI / 180.0f),
		deg.y * (XM_PI / 180.0f),
		deg.z * (XM_PI / 180.0f)
	};

	return rad;
}

// 近似かどうかチェック
static bool Approximately(float a, float b, float tolerance = 0.01f)
{
	float difference = a - b;
	if (fabsf(difference) < tolerance)
	{
		return true;
	}
	return false;
}

// クオータニオン→オイラー
static XMFLOAT3 QuaternionToEuler(const XMFLOAT4 &quaternion)
{
	XMFLOAT4 q = quaternion;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float w = q.w;

	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;

	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);
	float m01 = (2.0f * xy) + (2.0f * wz);
	float m10 = (2.0f * xy) - (2.0f * wz);
	float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);
	float m20 = (2.0f * xz) + (2.0f * wy);
	float m21 = (2.0f * yz) - (2.0f * wx);
	float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

	float tx, ty, tz;

	if (Approximately(m21, 1.0f))
	{
		tx = XM_PI / 2.0f;
		ty = 0;
		tz = atan2(m10, m00);
	}
	else if (Approximately(m21, -1.0f))
	{
		tx = -XM_PI / 2.0f;
		ty = 0;
		tz = atan2(m10, m00);
	}
	else
	{
		tx = asin(-m21);
		ty = atan2(m20, m22);
		tz = atan2(m01, m11);
	}

	tx *= 180.0f / DirectX::XM_PI;
	ty *= 180.0f / DirectX::XM_PI;
	tz *= 180.0f / DirectX::XM_PI;

	return XMFLOAT3(tx, ty, tz);
}



//向きベクトルから壁ずりベクトルへ変換
static Vector3 CalcWallScratchVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *scratchNormalVec = nullptr) {
	Vector3 nextVec = moveVec - Vector3::Dot(moveVec, normal) * normal;
	if (scratchNormalVec != nullptr) {
		*scratchNormalVec = nextVec.Normalized();
	}
	return nextVec;
}


//線の補間
static Vector3 sprinePosition(const std::vector<Vector3> &points, size_t &startIndex, float time, bool isLoop)
{
	//補間すべき点の数
	if (!isLoop) {
		if (startIndex >= points.size() - 1) {
			startIndex = points.size() - 1;
			return points[points.size() - 1];
		}
	}

	//終点に到達したらスタートをリセット
	if (startIndex == points.size()) {
		startIndex = 0;
	}
	//P0～P3の制御点を取得する
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	if (startIndex == 0 && isLoop) {
		p0 = points[points.size() - 1];
	}
	else if (startIndex == 0 && !isLoop) {
		p0 = points[0];
	}
	else {
		p0 = points[startIndex - 1];
	}

	p1 = points[startIndex];
	//今のインデックス+1にデータがなければ0に戻す
	if (startIndex >= points.size() - 1) {
		p2 = points[0];
	}
	else {
		p2 = points[startIndex + 1];
	}
	//今のインデックス+1にデータがなければ0に戻す
	if (startIndex >= points.size() - 2) {
		p3 = points[startIndex - points.size() - 2];
	}
	else {
		p3 = points[startIndex + 2];
	}
	//Catmull-Rom の式による補間
	Vector3 a = (-p0 + p2) * time;
	Vector3 b = (2 * p0 - 5 * p1 + 4 * p2 - p3) * time * time;
	Vector3 c = (-p0 + 3 * p1 - 3 * p2 + p3) * time * time * time;
	Vector3 position = 0.5f * (2 * p1 + a + b + c);

	return position;
}



