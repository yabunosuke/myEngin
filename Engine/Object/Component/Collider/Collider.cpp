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

void Collider::ComponentInitialize()
{
	// オブジェクトにコライダーwaek_ptr渡す
	game_object_->AddCollider(this);
	ComponentUpdate();
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
