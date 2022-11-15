#include "Vector3.h"

const Vector3 Vector3::forward{ 0.0f, 0.0f, 1.0f };	// (  0,  0,  1 )
const Vector3 Vector3::back{ 0.0f, 0.0f, -1.0f };	// (  0,  0, -1 )
const Vector3 Vector3::up{ 0.0f, 1.0f, 0.0f };		// (  0,  1,  0 )
const Vector3 Vector3::down{ 0.0f, -1, 0.0f };		// (  0, -1,  0 )
const Vector3 Vector3::right{ 1.0f, 0.0f, 0.0f };	// (  1,  0,  0 )
const Vector3 Vector3::left{ -1.0f, 0.0f, 0.0f };	// ( -1,  0,  0 )
const Vector3 Vector3::zero{ 0.0f, 0.0f, 0.0f };	// (  0,  0,  0 )
const Vector3 Vector3::one{ 1.0f, 1.0f, 1.0f };		// (  1,  1,  1 )
const Vector3 Vector3::positive_infinity{ Mathf::infinity, Mathf::infinity, Mathf::infinity };		// (  inf,  inf,  inf )
const Vector3 Vector3::negative_infinity{ Mathf::negative_infinity,Mathf::negative_infinity,Mathf::negative_infinity };// ( -inf, -inf, -inf )


float Vector3::Magnitude() const
{
	float magnitude = sqrtf(x * x + y * y + z * z);
	//XMVector3Length(XMLoadFloat3(this)).m128_f32[0];

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
	float length = this->Magnitude();
	Vector3 normalized = 
	{
		this->x /length,
		this->y /length,
		this->z /length,
	};
	return normalized;
}

float Vector3::Angle(const Vector3 &from, const Vector3 &to)
{
	float cos_sita = Dot(from, to) / (from.Magnitude() * to.Magnitude());
	float angle = acos(cos_sita) * Mathf::rad_to_deg;

	return angle;
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

Vector3 Vector3::Normalize(Vector3& valuse)
{
	valuse = valuse.Normalized();

	return valuse;
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

Vector3 Vector3::MoveTowards(const Vector3 &current, const Vector3 &target, float max_distance_delta)
{
	Vector3 normal{ Vector3(target - current) };
	float distance = Vector3::Distance(current, target);
	if (distance - max_distance_delta > 0.0f)
	{
		return current + normal.Normalized() * max_distance_delta;
	}
	return target;
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

Vector3 Vector3::Project(const Vector3 &vector, const Vector3 &onNormal)
{
	Vector3 project
	{
		onNormal * vector.Magnitude()
	};

	return project;
}

Vector3 Vector3::Scale(const Vector3 &lhs, const Vector3 &rhs)
{
	Vector3 scale
	{
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z
	};
	return scale;
}
