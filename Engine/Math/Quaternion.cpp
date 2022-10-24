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
	float cx = cosf(0.5f * roll);
	float sx = sinf(0.5f * roll);
	float cy = cosf(0.5f * pitch);
	float sy = sinf(0.5f * pitch);
	float cz = cosf(0.5f * yaw);
	float sz = sinf(0.5f * yaw);

	Quaternion quaternion
	{
		cx * sy * sz + cy * cz * sx,
		cx * cz * sy - cy * sx * sz,
		cx * cy * sz - cz * sx * sy,
		sx * sy * sz + cx * cy * cz
	};
	return quaternion;
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
    Quaternion r = *this;
    float x = r.x;
    float y = r.y;
    float z = r.z;
    float w = r.w;

    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;

    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    // 1 - 2y^2 - 2z^2
    float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);

    // 2xy + 2wz
    float m01 = (2.0f * xy) + (2.0f * wz);

    // 2xy - 2wz
    float m10 = (2.0f * xy) - (2.0f * wz);

    // 1 - 2x^2 - 2z^2
    float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);

    // 2xz + 2wy
    float m20 = (2.0f * xz) + (2.0f * wy);

    // 2yz+2wx
    float m21 = (2.0f * yz) - (2.0f * wx);

    // 1 - 2x^2 - 2y^2
    float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);


    float tx, ty, tz;

    if (Mathf::Approximately(m21, 1.0f))
    {
        tx = -Mathf::pi/ 2.0f;
        ty = 0;
        tz = atan2f(m10, m00);
    }
    else if (Mathf::Approximately(m21, -1.0f))
    {
        tx = Mathf::pi / 2.0f;
        ty = 0;
        tz = atan2f(m10, m00);
    }
    else
    {
		tx = asinf(-m21);
		ty = atan2f(m20, m22);
		tz = atan2f(m01, m11);
    }


	Vector3 euler
	{
		tx,ty,tz
	};
    return euler;

}
