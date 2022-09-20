#pragma once

#include "../Math/Vector3.h"

namespace yEngine
{
	struct AABB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };		// 中心座標
		float	radius[3]{ 1.0f,1.0f,1.0f };		// 半径
	};

	struct Sphere
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// 中心座標
		float	radius{ 0.0f };				// 半径
	};


	struct OBB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// 中心座標
		Vector3	u[3]						// 各軸ベクトル
		{ 
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f }
		};
		Vector3	euler{ 1.0f,1.0f,1.0f };	// 各軸の半径
	};
}