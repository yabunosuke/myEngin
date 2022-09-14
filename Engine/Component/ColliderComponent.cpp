#include "ColliderComponent.h"
#include "Scene/AbstractScene/AbstractScene.h"
#include "CollisionManager.h"

// �R���C�_�[
#include "SphereCollider.h"
#include "Collider/OBBCollider.h"

ColliderComponent::ColliderComponent(AbstractScene *scene, CollisionShapeType type):
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

void ColliderComponent::ComponentInitialize()
{
	// �R���C�_�[�ɃI�u�W�F�N�g��R�Â�
	collider->SetObject(game_object_.lock());
	// �I�u�W�F�N�g�ɃR���C�_�[waek_ptr�n��
	game_object_.lock()->AddCollider(collider);
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
