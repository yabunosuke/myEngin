#pragma once

#include "../Math/Vector3.h"

namespace yEngine
{
	struct Plane
	{
		Vector3 normal{ 0.0f,1.0f,0.0f };	// ���ʏ�̖@��
		float dot{ 0.0f };					//
	};
	
	/// <summary>
	/// 3���_���畽�ʂ��쐬
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
		Vector3	center{ 0.0f,0.0f,0.0f };	// ���S���W
		float	radius[3]{ 1.0f,1.0f,1.0f };// ���a
	};

	struct Sphere
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// ���S���W
		float	radius{ 0.0f };				// ���a
	};

	struct OBB
	{
		Vector3	center{ 0.0f,0.0f,0.0f };	// ���S���W
		Vector3	unidirectional[3]			// �e���x�N�g��
		{ 
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f }
		};
		Vector3	extent{ 1.0f,1.0f,1.0f };	// �e���̔��a
	};

	struct Capsule
	{
		Vector3 start{ 0.0f,0.0f,0.0f };	// �J�n�n�_
		Vector3 end{ 0.0f,1.0f,0.0f };		// �I���n�_
		float radius{ 1.0f };				// ���a
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