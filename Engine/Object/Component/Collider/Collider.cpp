#include "Collider.h"
#include "Scene/AbstractScene/AbstractScene.h"
#include "CollisionManager.h"

// コライダー
#include "SphereCollider.h"
#include "Collider/OBBCollider.h"

Collider::Collider(AbstractScene *scene, CollisionShapeType type):
	Component("Collider", ComponentType::None),
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
}
//
//ColliderComponent::~ColliderComponent()
//{
//	game_object_.lock()->RemoveCollider(collider);
//
//}

void Collider::ComponentInitialize()
{
	// コライダーにオブジェクトを紐づけ
	collider->SetObject(game_object_.lock());
	// オブジェクトにコライダーwaek_ptr渡す
	game_object_.lock()->AddCollider(collider);
	collider->Update();

}

void Collider::ComponentUpdate()
{
	collider->Update();
}

void Collider::ComponentDraw()
{
	collider->Draw();
}

void Collider::ComponentFinalize()
{
}

void Collider::Infomation()
{
	collider->Infomation();
}
