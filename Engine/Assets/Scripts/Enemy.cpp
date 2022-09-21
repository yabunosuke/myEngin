#include "Enemy.h"

#include "Object3dComponent.h"

Enemy::Enemy() :
MonoBehaviour("Enemy")
{
}

void Enemy::OnCollisionEnter(Collision &collision)
{
	if(collision.gameObject->lock()->name.r_ == "Weapon")
	{
		--hp_;
		if(hp_ <= 0)
		{
			state = AnimationState::DEAD;
			game_object->PlayAnimation(static_cast<int>(state), false);
		}
		else
		{
			state = AnimationState::DAMAGE;
			game_object->PlayAnimation(static_cast<int>(state), false);
		}
	}
}

void Enemy::Start()
{
	// ƒŠƒWƒbƒh
	regidbody_ =
		game_object_.lock()->GetComponent<Rigidbody>();
	game_object =
		game_object_.lock()->GetComponent<Object3dComponent>().lock()->GetObjectData();
	game_object->PlayAnimation(static_cast<int>(AnimationState::FIGHT_IDOLE),true,0.0f);
}

void Enemy::FixedUpdate()
{
	if (hp_ > 0)
	{
		if(!game_object->IsPlayAnimation())
		{
			state = AnimationState::FIGHT_IDOLE;
			game_object->PlayAnimation(static_cast<int>(state));
		}
	}
}

void Enemy::Update()
{
}

void Enemy::Infomation()
{
	ImGui::DragInt("HP", &hp_);
}
