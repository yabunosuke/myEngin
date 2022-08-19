#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Vector3 : public XMFLOAT3 {
	Vector3() = default;
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3(XMFLOAT3 num) {
		this->x = num.x;
		this->y = num.y;
		this->z = num.z;
	}
	Vector3(const XMVECTOR &other) :XMFLOAT3() {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}


	//���Z
	inline bool operator == (const Vector3 &r) const { return x == r.x && y == r.y && z == r.z; }
	inline bool operator != (const Vector3 &r) const { return !(*this == r); }
	inline XMVECTOR operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
	inline XMVECTOR operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

	//�񍀉��Z
	Vector3 operator+=(const Vector3 &r) {
		*this = *this + r;
		return *this;
	}
	Vector3 operator-=(const Vector3 &r) {
		*this = *this + -r;
		return *this;
	}

	Vector3 operator*=(const Vector3 &r) {
		*this = *this * r;
		return *this;
	}

	Vector3 operator*=(const float &r) {
		*this = *this * r;
		return *this;
	}

	Vector3 operator=(const float &r) {
		*this = Vector3(r,r,r);
		return *this;
	}
	Vector3 operator=(const XMFLOAT3 &num) {
		*this = Vector3(num.x, num.y, num.z);
		return *this;
	}
	// �x�N�g���̓���
	float VDot(Vector3 In) const { return x * In.x + y * In.y + z * In.z; }
	// �x�N�g���̊O��
	Vector3 VCross(Vector3 In) const { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }
	// �x�N�g���̃X�P�[�����O
	Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

	//���
	Vector3 &operator=(const XMVECTOR &other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}
	//�L���X�g
	operator XMVECTOR() const {
		return XMLoadFloat3(this);
	}
	//����
	float Length() const {
		return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
	}
	//�������
	float LengthSq() const {
		return powf(this->Length(),2);
	}
	//���K��(���������Ȃ�)
	Vector3 Normal() {
		return XMVector3Normalize(XMVECTOR(*this));
	}
	//���K��(������������)
	Vector3 Normalize() {

		*this = XMVector3Normalize(XMVECTOR(*this));
		return *this;
	}
	void RotationX(float angle) {
	
	}
	//Y������]
	void AddRotationY(float angle) {
		*this = {
			this->x * cosf(angle)	+0			+this->z * sinf(angle),
			0						+this->y	+0,	
			this->x * -sinf(angle)	+0			+this->z * cosf(angle)
		};
	}
	void RotationZ(float angle) {

	}
};

static const Vector3 foward{ 0,0,1 };
static const Vector3 right{ 1,0,0 };
static const Vector3 up{ 0,1,0 };