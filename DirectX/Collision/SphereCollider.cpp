#include "SphereCollider.h"
#include "TransformComponent.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX &matWorld = XMMatrixIdentity();// = XMLoadFloat4x4(&object->GetComponent<TransformComponent>().at(0)->GetTransform());

	// ���̃����o�ϐ����X�V
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}