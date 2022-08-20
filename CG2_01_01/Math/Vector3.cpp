#include "Vector3.h"

float Vector3::Magnitude() const
{
	float magnitude = XMVector3Length(XMLoadFloat3(this)).m128_f32[0];

	return magnitude;
}

float Vector3::SqrMagnitude() const
{
	float magnitude = this->Magnitude();
	float sqr_magnitude = magnitude * magnitude;

	return sqr_magnitude;
}

Vector3 Vector3::Normalized() const
{
	Vector3 normalized = XMVector3Normalize(XMLoadFloat3(this));
	Vector3 a = { 1,1,1 };
	Vector3 b = { 1,1,1 };
	Vector3 c = (a - b);

	return normalized;
}

float Vector3::Dot(const Vector3& lhs, const Vector3& rhs)
{
	float dot = lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;

	return dot;
}

Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs)
{
	Vector3 cross = {
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	};

	return cross;
}

Vector3 Vector3::Normalize(const Vector3& valuse)
{
	Vector3 normalize = valuse.Normalized();

	return normalize;
}

Vector3 Vector3::Max(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 max = {
		lhs.x > rhs.x ? lhs.x : rhs.x,
		lhs.y > rhs.y ? lhs.y : rhs.y,
		lhs.z > rhs.z ? lhs.z : rhs.z
	};

	return max;
}

Vector3 Vector3::Min(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 min = {
		lhs.x < rhs.x ? lhs.x : rhs.x,
		lhs.y < rhs.y ? lhs.y : rhs.y,
		lhs.z < rhs.z ? lhs.z : rhs.z
	};

	return min;
}

Vector3 Vector3::ClampMagnitude(const Vector3 &vector, const float &max_length)
{
	Vector3 clamp = {
		vector.x < max_length ? vector.x : max_length,
		vector.y < max_length ? vector.y : max_length,
		vector.z < max_length ? vector.z : max_length
	};

	return clamp;
}

Vector3 Vector3::Reflect(const Vector3 &inDirection, const Vector3 &inNormal)
{
	Vector3 reflect = inDirection - 2.0f * Dot(inDirection, inNormal) * inNormal;

	return reflect;
}

float Vector3::Distance(const Vector3 &lhs, const Vector3 &rhs)
{
	float distance = (lhs - rhs).Magnitude();

	return distance;
}

float Vector3::Angle(const Vector3 &from, const Vector3 &to)
{
	float cos_sita = Dot(from, to) / (from.Magnitude() * to.Magnitude());
	float angle = acos(cos_sita);

	return angle;
}
