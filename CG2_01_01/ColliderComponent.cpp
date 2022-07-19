#include "ColliderComponent.h"
#include "AbstractScene.h"

// コライダー
#include "SphereCollider.h"

ColliderComponent::ColliderComponent(CollisionShapeType type, AbstractScene &scene):
	Component("Collider")
{
	switch (type)
	{
	case SHAPE_UNKNOWN:
		break;
	case SHAPE_POINT:
		break;
	case SHAPE_RAY:
		break;
	case SHAPE_LINE:
		break;
	case SHAPE_TRIANGLE:
		break;
	case SHAPE_SPHERE:
		collider = std::make_shared<SphereCollider>();
		break;
	case SHAPE_CYLINDER:
		break;
	case SHAPE_AABB:
		break;
	case SHAPE_OBB:
		break;
	case SHAPE_CAPSULE:
		break;
	case SHAPE_MESH:
		break;
	case SHAPE_PLANE:
		break;
	case BOX_2D:
		break;
	default:
		break;
	}
	collider->SetObject(parent);
	scene.GetCollisionManager()->AddCollider(collider);
}

void ColliderComponent::VirtualInitialize()
{
}

void ColliderComponent::VirtualUpdate()
{
	collider->Update();
}

void ColliderComponent::VirtualDraw()
{
}

void ColliderComponent::VirtualFinalize()
{
}

void ColliderComponent::Infomation()
{
}
