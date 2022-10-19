#pragma once
#include "Math/Vector3.h"
#include "Object/Component/Collider/Collider.h"
#include <vector>
struct ContactPoint
{
	Vector3 normal;							// 接触点の法線
	std::vector<Collider> other_collider;	// 接触している他のコライダー
	Vector3 point;							// 接触地点
	Collider this_collider;					// 最初に接触したコライダー情報
};