#include "Matrix4x4.h"
const Matrix4x4 Matrix4x4::identity		// 恒等行列
{
		1.0f,0.0f,0.0f,0.0,
		0.0f,1.0f,0.0f,0.0,
		0.0f,0.0f,1.0f,0.0,
		0.0f,0.0f,0.0f,1.0
};
const Matrix4x4 Matrix4x4::zero			// 全ての要素を0にした行列
{
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0
};

Matrix4x4 Matrix4x4::Translation(const Vector3 &translation)
{
	Matrix4x4 t
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translation.x, translation.y, translation.z, 1.0f
	};
	return t;
}

Matrix4x4 Matrix4x4::Scale(const Vector3 &scale)
{
	Matrix4x4 s
	{
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return s;
}

Matrix4x4 Matrix4x4::Rotate(const Quaternion &quaternion)
{
	Matrix4x4 r
	{
		1.0f - 2.0f * quaternion.y * quaternion.y - 2.0f * quaternion.z * quaternion.z,
		2.0f * quaternion.x * quaternion.y + 2.0f * quaternion.w * quaternion.z,
		2.0f * quaternion.x * quaternion.z - 2.0f * quaternion.w * quaternion.y,
		0.0f,

		2.0f * quaternion.x * quaternion.y - 2.0f * quaternion.w * quaternion.z,
		1.0f - 2.0f * quaternion.x * quaternion.x - 2.0f * quaternion.z * quaternion.z,
		2.0f * quaternion.y * quaternion.z + 2.0f * quaternion.w * quaternion.x,
		0.0f,

		2.0f * quaternion.x * quaternion.z + 2.0f * quaternion.w * quaternion.y,
		2.0f * quaternion.y * quaternion.z - 2.0f * quaternion.w * quaternion.x,
		1.0f - 2.0f * quaternion.x * quaternion.x - 2.0f * quaternion.y * quaternion.y,
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f

	};

	return r;
}

Quaternion Matrix4x4::GetRotation(const Matrix4x4 &rotate_matrix)
{
	float element[4];
	element[0] = rotate_matrix.m[0][0] - rotate_matrix.m[1][1] - rotate_matrix.m[2][2] + 1.0f;
	element[1] = -rotate_matrix.m[0][0] + rotate_matrix.m[1][1] - rotate_matrix.m[2][2] + 1.0f;
	element[2] = -rotate_matrix.m[0][0] - rotate_matrix.m[1][1] + rotate_matrix.m[2][2] + 1.0f;
	element[3] = rotate_matrix.m[0][0] + rotate_matrix.m[1][1] + rotate_matrix.m[2][2] + 1.0f;

	int index = 0;
	for (int i = 1; i < 4; ++i)
	{
		if (element[i] > element[index])
		{
			index = i;
		}
	}

	if (element[index] < 0.0f)
	{
		assert(0);
	}
	Quaternion result;
	float v = sqrtf(element[index]) * 0.5f;
	result[index] = v;
	float mult = 0.25f / v;

	switch (index)
	{
	case 0:
		result[1] = (rotate_matrix.m[0][1] + rotate_matrix.m[1][0]) * mult;
		result[2] = (rotate_matrix.m[2][0] + rotate_matrix.m[0][2]) * mult;
		result[3] = (rotate_matrix.m[1][2] - rotate_matrix.m[2][1]) * mult;
		break;

	case 1:
		result[0] = (rotate_matrix.m[0][1] + rotate_matrix.m[1][0]) * mult;
		result[2] = (rotate_matrix.m[1][2] + rotate_matrix.m[2][1]) * mult;
		result[3] = (rotate_matrix.m[2][0] - rotate_matrix.m[0][2]) * mult;
		break;

	case 2:
		result[0] = (rotate_matrix.m[2][0] + rotate_matrix.m[0][2]) * mult;
		result[1] = (rotate_matrix.m[1][2] + rotate_matrix.m[2][1]) * mult;
		result[3] = (rotate_matrix.m[0][1] - rotate_matrix.m[1][0]) * mult;
		break;

	case 3:
		result[0] = (rotate_matrix.m[1][2] - rotate_matrix.m[2][1]) * mult;
		result[1] = (rotate_matrix.m[2][0] - rotate_matrix.m[0][2]) * mult;
		result[2] = (rotate_matrix.m[0][1] - rotate_matrix.m[1][0]) * mult;
		break;

	default:
		break;
	}

	return result;
}

Matrix4x4 Matrix4x4::Inverse(const Quaternion &quaternion)
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::ExtractTranslation()
{
	Matrix4x4 t = Translation({ this->m[3][0],this->m[3][1], this->m[3][2] });
	return t;
}

Matrix4x4 Matrix4x4::ExtractScale()
{
	Matrix4x4 s = Scale(
		{
			Vector3{this->m[0][0],this->m[0][1],this->m[0][2]}.Magnitude(),
			Vector3{this->m[1][0],this->m[1][1],this->m[1][2]}.Magnitude(),
			Vector3{this->m[2][0],this->m[2][1],this->m[2][2]}.Magnitude()
		}
	);
	return s;
}

Matrix4x4 Matrix4x4::ExtractRotation()
{
	return Matrix4x4();
}


Matrix4x4 &operator*=(Matrix4x4 &rhs, const Matrix4x4 &lhs)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				result.m[i][j] += rhs.m[i][k] * lhs.m[k][j];
			}

		}
	}
	rhs = result;
	return result;
}

Matrix4x4 &operator*(const Matrix4x4 &rhs, const Matrix4x4 &lhs)
{
	Matrix4x4 result = rhs;
	return result *= lhs;
}
