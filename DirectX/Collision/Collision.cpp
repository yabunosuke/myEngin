#include "Collision.h"
#include "Vector3.h"
using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
	DirectX::XMVECTOR* inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}
	//�[����_���v�Z
	if (inter)
	{
		//���ʏ�̍ŋߐړ_���[����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
	const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
	DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	XMVECTOR p;
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius)
	{
		return false;
	}
	//�[����_���v�Z
	if (inter)
	{
		//�O�p�`��̍ŋߐړ_p���[����_�Ƃ���
		*inter = p;
	}
	// �����o���x�N�g�����v�Z
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}

	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane,
	float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; //�덷�z���p�̔����Ȓl
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon)
	{
		return false;
	}
	//�n�_�ƌ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)
	{
		return false;
	}
	//��������������
	if (distance)
	{
		*distance = t;
	}
	//��_���v�Z
	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
	float* distance, DirectX::XMVECTOR* inter)
{
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlane))
	{
		return false;
	}
	//���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f; //�덷�z���p�̔����Ȓl
	XMVECTOR m;
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}
	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon)
	{
		return false;
	}

	//�����Ȃ̂ŁA�������Ă���
	if (inter)
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
	float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă��������������Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	//���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)
	{
		return false;
	}

	//���C�͋��ƌ������Ă���
	//��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	//t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0)
	{
		t = 0.0f;
	}

	if (distance)
	{
		*distance = t;
	}

	if (inter)
	{
		*inter = ray.start + t * ray.dir;
	}

	return true;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
	DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	//���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2)
	{
		if (inter)
		{
			//A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		// �����o���x�N�g�����v�Z
		if (reject)
		{
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = XMVector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule)
{
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	Vector3 n = vStartToEnd;
	n.Normalize();

	Vector3 sPos = { sphere.center.m128_f32[0], sphere.center.m128_f32[1], sphere.center.m128_f32[2] };

	float t = n.VDot(Vector3(sPos - capsule.startPosition));

	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	float lengthRate = t / vStartToEnd.Length();

	float distance;
	if (lengthRate < 0.0f)
	{
		distance = Vector3(sPos + capsule.startPosition).Length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = Vector3(posPn - sPos).Length() - capsule.radius;
	}
	else
	{
		distance = Vector3(sPos + capsule.endPosition).Length() - capsule.radius;
	}

	return distance < sphere.radius;
}

bool Collision::CheckSphere2Box(const Sphere& sphere, const Box& box)
{
	float sqDistance = 0.0f;
	float pos;

	pos = sphere.center.m128_f32[0];
	if (pos < box.center.m128_f32[0] - box.scale.x)
	{
		sqDistance += (pos - box.center.m128_f32[0] - box.scale.x) * (pos - box.center.m128_f32[0] - box.scale.x);
	}
	else if (pos > box.center.m128_f32[0] + box.scale.x)
	{
		sqDistance += (pos - box.center.m128_f32[0] + box.scale.x) * (pos - box.center.m128_f32[0] + box.scale.x);
	}

	pos = sphere.center.m128_f32[1];
	if (pos < box.center.m128_f32[1] - box.scale.y)
	{
		sqDistance += (pos - box.center.m128_f32[1] - box.scale.y) * (pos - box.center.m128_f32[1] - box.scale.y);
	}
	else if (pos > box.center.m128_f32[1] + box.scale.y)
	{
		sqDistance += (pos - box.center.m128_f32[1] + box.scale.y) * (pos - box.center.m128_f32[1] + box.scale.y);
	}

	pos = sphere.center.m128_f32[2];
	if (pos < box.center.m128_f32[2] - box.scale.z)
	{
		sqDistance += (pos - box.center.m128_f32[2] - box.scale.z) * (pos - box.center.m128_f32[2] - box.scale.z);
	}
	else if (pos > box.center.m128_f32[2] + box.scale.z)
	{
		sqDistance += (pos - box.center.m128_f32[2] + box.scale.z) * (pos - box.center.m128_f32[2] + box.scale.z);
	}

	return sqDistance < sphere.radius * sphere.radius;
}

bool Collision::CheckSphere2AABB(const Sphere &sphere, const AABB &aabb, DirectX::XMVECTOR *inter)
{
	//�����ׂ̂���
	float sqLen = 0;
	Vector3 spherePos = sphere.center;
	if (spherePos.x < aabb.center.x) {
		sqLen += (spherePos.x - aabb.center.x) * (spherePos.x - aabb.center.x);
	}
	else if (spherePos.x > aabb.center.x + aabb.length.x) {
		sqLen += (spherePos.x - (aabb.center.x + aabb.length.x)) * (spherePos.x- (aabb.center.x + aabb.length.x));
	}
	if (spherePos.y < aabb.center.y) {
		sqLen += (spherePos.y - aabb.center.y) * (spherePos.y - aabb.center.y);
	}
	else if (spherePos.y > aabb.center.y + aabb.length.y) {
		sqLen += (spherePos.y - (aabb.center.y + aabb.length.y)) * (spherePos.y - (aabb.center.y + aabb.length.y));
	}
	if (spherePos.z < aabb.center.z) {
		sqLen += (spherePos.z - aabb.center.z) * (spherePos.z - aabb.center.z);
	}
	else if (spherePos.z > aabb.center.z + aabb.length.z) {
		sqLen += (spherePos.z - (aabb.center.z + aabb.length.z)) * (spherePos.z - (aabb.center.z + aabb.length.z));
	}

	//�Փ�
	if (sqLen < sphere.radius * sphere.radius) {
		if (inter) {
			Vector3 checkPos = spherePos - aabb.center;
			//�Փˈʒu
			if (checkPos.x < aabb.length.x) {
				inter->m128_f32[0] = aabb.length.x;
			}
			else {
				inter->m128_f32[0] = spherePos.x;
			}

			if (checkPos.y < aabb.length.y) {
				inter->m128_f32[1] = aabb.length.y;
			}
			else {
				inter->m128_f32[1] = spherePos.y;
			}

			if (checkPos.z < aabb.length.z) {
				inter->m128_f32[2] = aabb.length.z;
			}
			else {
				inter->m128_f32[2] = spherePos.z;
			}
		}
		return true;
	}
	return false;
}

bool Collision::CheckCapsule2Box(const Capsule& capsule, const Box& box)
{
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	Vector3 n = vStartToEnd;
	n.Normalize();

	Vector3 boxMinPos = { box.center.m128_f32[0] - box.scale.x, box.center.m128_f32[1] - box.scale.y, box.center.m128_f32[2] - box.scale.z };
	Vector3 boxMaxPos = { box.center.m128_f32[0] + box.scale.x, box.center.m128_f32[1] + box.scale.y, box.center.m128_f32[2] + box.scale.z };

	float t = n.VDot(Vector3(boxMaxPos - capsule.startPosition));

	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	float lengthRate = t / vStartToEnd.Length();

	float sqDistance = 0.0f;
	float pos;
	if (lengthRate < 0.0f)
	{
		pos = capsule.startPosition.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = capsule.startPosition.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = capsule.startPosition.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}
	else if (lengthRate <= 1.0f)
	{
		pos = posPn.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = posPn.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = posPn.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}
	else
	{
		pos = capsule.endPosition.x;
		if (pos < boxMinPos.x)
		{
			sqDistance += (pos - boxMinPos.x) * (pos - boxMinPos.x);
		}
		else if (pos > boxMaxPos.x)
		{
			sqDistance += (pos - boxMaxPos.x) * (pos - boxMaxPos.x);
		}

		pos = capsule.endPosition.y;
		if (pos < boxMinPos.y)
		{
			sqDistance += (pos - boxMinPos.y) * (pos - boxMinPos.y);
		}
		else if (pos > boxMaxPos.y)
		{
			sqDistance += (pos - boxMaxPos.y) * (pos - boxMaxPos.y);
		}

		pos = capsule.endPosition.z;
		if (pos < boxMinPos.z)
		{
			sqDistance += (pos - boxMinPos.z) * (pos - boxMinPos.z);
		}
		else if (pos > boxMaxPos.z)
		{
			sqDistance += (pos - boxMaxPos.z) * (pos - boxMaxPos.z);
		}
	}

	return sqDistance < capsule.radius* capsule.radius;
}