#include "Collider.h"
#include "Scene/AbstractScene/AbstractScene.h"


Collider::Collider():
	Component("Collider", ComponentType::Collider)
{
	
	//collision_manager_ = std::make_shared<CollisionManager>(*scene->GetCollisionManager());
}


void Collider::ComponentUpdate()
{
	//collider->Update();
}

void Collider::ComponentDraw()
{
	//collider->Draw();
}

void Collider::ComponentFinalize()
{
}

void Collider::Infomation()
{
	//collider->Infomation();
}
