#include "EnemyBase.h"
#include <Object3dComponent.h>

EnemyBase::EnemyBase(const std::string &name):
MonoBehaviour(name)
{
}


void EnemyBase::Start()
{
	// ���W�b�h
	rigidbody_ =
		game_object_->GetComponent<Rigidbody>();

	model_data_ =
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
}

void EnemyBase::FixedUpdate()
{
}

void EnemyBase::Update()
{
}

void EnemyBase::Infomation()
{
	ImGui::SliderFloat("HP", &hp_, 0.0f, max_hp_);
}
