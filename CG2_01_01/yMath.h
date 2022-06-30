#pragma once
#define _USE_MATH_DIFINES
#include <cmath>
#include "Vector3.h"
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
static float RadToDeg(float rad) {
	return rad * (180.0f / XM_PI);
}
//度→ラジアン
static float DegToRad(float deg) {
	return deg * (XM_PI / 180.0f);
}

//向きベクトルから壁ずりベクトルへ変換
static Vector3 CalcWallScratchVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *scratchNormalVec = nullptr) {
	Vector3 nextVec = moveVec - moveVec.VDot(normal) * normal;
	if (scratchNormalVec != nullptr) {
		*scratchNormalVec = nextVec.Normal();
	}
	return nextVec;
}

//向きベクトルから反射ベクトル
static Vector3 CalcReflectVector(const Vector3 &moveVec, const Vector3 &normal, Vector3 *reflectNormalVec = nullptr) {
	Vector3 nextVec = moveVec - 2.0f * moveVec.VDot(normal) * normal;
	if (reflectNormalVec != nullptr) {
		*reflectNormalVec = nextVec.Normal();
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



