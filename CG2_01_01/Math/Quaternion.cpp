#include "Quaternion.h"

float Quaternion::Angle(const Quaternion &from, const Quaternion &to)
{
	return acos(Dot(from.Normalized(), to.Normalized())) * 2.0f;
}

float Quaternion::Dot(const Quaternion &lhs, const Quaternion &rhs)
{
	return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w);
}

Quaternion Quaternion::Euler(const float &roll, const float &pitch, const float &yaw)
{
	float cx = cos(0.5f * roll);
	float sx = sin(0.5f * roll);
	float cy = cos(0.5f * pitch);
	float sy = sin(0.5f * pitch);
	float cz = cos(0.5f * yaw);
	float sz = sin(0.5f * yaw);

	Quaternion temp = {
		-cx * sy * sz + sx * cy * cz,
		cx *sy *cz + sx * cy * sz,
		sx *sy *cz + cx * cy * sz,
		-sx * sy * sz + cx * cy * cz
	};
	return temp;
}

Quaternion Quaternion::Euler(const Vector3 &rotation_vector)
{
	return Euler(rotation_vector.x, rotation_vector.y, rotation_vector.z);
}

Quaternion Quaternion::Normalize(Quaternion &quaternion)
{
	quaternion = quaternion.Normalized();
	return quaternion;
}

Quaternion Quaternion::Normalized() const
{
	Quaternion normalized;
	XMStoreFloat4(&normalized,XMQuaternionNormalize(XMLoadFloat4(this)));

	return normalized;
}

Vector3 Quaternion::EulerAngles() const
{

	Quaternion q = *this;
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

	if (Mathf::Approximately(m21, 1.0f))
	{
		tx = XM_PI / 2.0f;
		ty = 0;
		tz = atan2(m10, m00);
	}
	else if (Mathf::Approximately(m21, -1.0f))
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

	return Vector3(tx, ty, tz);
}
