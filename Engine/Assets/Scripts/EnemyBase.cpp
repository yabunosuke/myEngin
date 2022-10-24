#include "EnemyBase.h"

EnemyBase::EnemyBase(const std::string &name):
MonoBehaviour(name)
{
}


void EnemyBase::Start()
{
	// ƒŠƒWƒbƒh
	regidbody_ =
		game_object_->GetComponent<Rigidbody>();
	transform_->scale = { 40,40,40 };
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
