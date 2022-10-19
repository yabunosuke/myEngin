#include "Collider.h"
#include "Scene/AbstractScene/AbstractScene.h"


Collider::Collider():
	Component("Collider", ComponentType::Collider)
{
}

Collider::~Collider()
{
	if (&game_object_ != nullptr)
	{
		return;
	}
	game_object_->RemoveCollider(this);
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
