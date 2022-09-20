#pragma once

#include "../Math/Vector3.h"

namespace yEngine
{
	struct AABB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };		// ���S���W
		float	radius[3]{ 1.0f,1.0f,1.0f };		// ���a
	};

	struct Sphere
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// ���S���W
		float	radius{ 0.0f };				// ���a
	};


	struct OBB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// ���S���W
		Vector3	u[3]						// �e���x�N�g��
		{ 
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f }
		};
		Vector3	euler{ 1.0f,1.0f,1.0f };	// �e���̔��a
	};
}