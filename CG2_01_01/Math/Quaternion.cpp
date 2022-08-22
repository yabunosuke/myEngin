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
