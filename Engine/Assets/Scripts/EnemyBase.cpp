#include "EnemyBase.h"
#include <Object3dComponent.h>

EnemyBase::EnemyBase(const std::string &name):
MonoBehaviour(name)
{
}


void EnemyBase::Start()
{
	// ƒŠƒWƒbƒh
	rigidbody_ =
		game_object_.lock()->GetComponent<Rigidbody>().lock().get();

	model_data_ =
		game_object_.lock()->GetComponent<Object3dComponent>().lock()->GetObjectData();
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
