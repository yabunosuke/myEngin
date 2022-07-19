#include "SphereCollider.h"
#include "TransformComponent.h"

using namespace DirectX;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX &matWorld = XMMatrixIdentity();// = XMLoadFloat4x4(&object->GetComponent<TransformComponent>().at(0)->GetTransform());

	// 球のメンバ変数を更新
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}