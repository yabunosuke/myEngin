#include "ColliderComponent.h"
#include "Scene/AbstractScene/AbstractScene.h"

// コライダー
#include "SphereCollider.h"
#include "Collider/OBBCollider.h"

ColliderComponent::ColliderComponent(AbstractScene *scene, CollisionShapeType type):
	Component("Collider", ComponentID::None),
	type_(type)
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
		collider = std::make_shared<OBBCollider>();
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
	collision_manager_ = std::make_shared<CollisionManager>(*scene->GetCollisionManager());
	collision_manager_->AddCollider(collider);
}

ColliderComponent::~ColliderComponent()
{
	parent->RemoveCollider(collider);
}

void ColliderComponent::ComponentInitialize()
{
	// コライダーにオブジェクトを紐づけ
	collider->SetObject(parent);
	// オブジェクトにコライダーwaek_ptr渡す
	parent->AddCollider(collider);
	collider->Update();

}

void ColliderComponent::ComponentUpdate()
{
	collider->Update();
}

void ColliderComponent::ComponentDraw()
{
	collider->Draw();
}

void ColliderComponent::ComponentFinalize()
{
}

void ColliderComponent::Infomation()
{
	collider->Infomation();
}
