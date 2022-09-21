#pragma once

#include "../Math/Vector3.h"

namespace yEngine
{
	struct Plane
	{
		Vector3 normal{ 0.0f,1.0f,0.0f };	// 平面上の法線
		float dot{ 0.0f };					//
	};
	
	/// <summary>
	/// 3頂点から平面を作成
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	static Plane ComputePlane(Vector3 a, Vector3 b, Vector3 c)
	{
		Plane p;
		p.normal = Vector3::Normalize(Vector3::Cross(b - a, c - a));
		p.dot = Vector3::Dot(p.normal, a);

		return p;
	};

	struct AABB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// 中心座標
		float	radius[3]{ 1.0f,1.0f,1.0f };// 半径
	};

	struct Sphere
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// 中心座標
		float	radius{ 0.0f };				// 半径
	};

	struct OBB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// 中心座標
		Vector3	unidirectional[3]			// 各軸ベクトル
		{ 
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f }
		};
		Vector3	extent{ 1.0f,1.0f,1.0f };	// 各軸の半径
	};

	struct Capsule
	{
		Vector3 start{ 0.0f,0.0f,0.0f };	// 開始地点
		Vector3 end{ 0.0f,1.0f,0.0f };		// 終了地点
		float radius{ 1.0f };				// 半径
	};

	struct Triangle
	{
		Vector3 point[3]
		{
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,1.0f,0.0f}
		};
	};

}