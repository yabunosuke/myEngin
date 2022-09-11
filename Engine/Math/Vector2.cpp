#include "Vector2.h"
const Vector2 Vector2::up{ 0.0f,1.0f };			// (  0,  1 )
const Vector2 Vector2::down{ 0.0f,-1.0f };		// (  0, -1 )
const Vector2 Vector2::right{ 1.0f,0.0f };		// (  1,  0 )
const Vector2 Vector2::left{ -1.0f,0.0f };		// ( -1,  0 )
const Vector2 Vector2::zero{ 0.0f,0.0f };		// (  0,  0 )
const Vector2 Vector2::one{ 1.0f,1.0f };		// (  1,  1 )
const Vector2 Vector2::positiveInfinity{ Mathf::infinity ,Mathf::infinity };		// (  inf,  inf )
const Vector2 Vector2::negativeInfinity{ Mathf::negative_infinity ,Mathf::negative_infinity };		// ( -inf, -inf )

float Vector2::Angle(const Vector2 &from, const Vector2 &to)
{
	float cos_sita = Dot(from, to) / (from.Magnitude() * to.Magnitude());
	float angle = acos(cos_sita);

	return angle;
}

Vector2 Vector2::ClampMagnitude(const Vector2 &vector, const float &max_length)
{
	Vector2 clamp = {
		vector.x < max_length ? vector.x : max_length,
		vector.y < max_length ? vector.y : max_length,
	};

	return clamp;
}

float Vector2::Distance(const Vector2 &lhs, const Vector2 &rhs)
{
	float distance = (lhs - rhs).Magnitude();

	return distance;

}

float Vector2::Dot(const Vector2 &lhs, const Vector2 &rhs)
{
	float dot = lhs.x * rhs.x + lhs.y * rhs.y;

	return dot;
}

Vector2 Vector2::Max(const Vector2 &lhs, const Vector2 &rhs)
{
	Vector2 max = {
		lhs.x > rhs.x ? lhs.x : rhs.x,
		lhs.y > rhs.y ? lhs.y : rhs.y,
	};

	return max;
}

Vector2 Vector2::Min(const Vector2 &lhs, const Vector2 &rhs)
{
	Vector2 min = {
		lhs.x < rhs.x ? lhs.x : rhs.x,
		lhs.y < rhs.y ? lhs.y : rhs.y,
	};

	return min;
}

Vector2 Vector2::Reflect(const Vector2 &inDirection, const Vector2 &inNormal)
{
	Vector2 reflect = inDirection - 2.0f * Dot(inDirection, inNormal) * inNormal;

	return reflect;
}


float Vector2::Magnitude() const
{
	float magnitude = XMVector2Length(XMLoadFloat2(this)).m128_f32[0];

	return magnitude;
}

Vector2 Vector2::Normalized() const
{
	Vector2 normalized = XMVector2Normalize(XMLoadFloat2(this));

	return normalized;
}
