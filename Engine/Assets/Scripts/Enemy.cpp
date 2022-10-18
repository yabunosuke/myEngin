#include "Enemy.h"

#include "Object3dComponent.h"
#include "Time/Time.h"

Enemy::Enemy() :
MonoBehaviour("Enemy")
{
}

void Enemy::OnCollisionEnter(Collision &collision)
{
	if (collision.gameObject.r_->name.r_ == "Weapon")
	{
		if(state != AnimationState::DEAD)
		{
			if(hp_ <= 0)
			{
				state = AnimationState::DEAD;
				model_data_->PlayAnimation(static_cast<int>(state), false);
			}
			else
			{
				--hp_;
				state = AnimationState::DAMAGE;
				model_data_->PlayAnimation(static_cast<int>(state), false,true);
			}
		}
	}
}

void Enemy::Start()
{
	// ƒŠƒWƒbƒh
	regidbody_ =
		game_object_->GetComponent<Rigidbody>();
	model_data_ =
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
	model_data_->PlayAnimation(static_cast<int>(AnimationState::FIGHT_IDOLE),true,false ,0.0f);
}

void Enemy::FixedUpdate()
{
	if (hp_ > 0)
	{
		if(!model_data_->IsPlayAnimation())
		{
			state = AnimationState::FIGHT_IDOLE;
			model_data_->PlayAnimation(static_cast<int>(state));
		}
	}


	if (hp_<=0)
	{
		destroy_timer_test_ += Time::GetInstance()->fixedDeltaTime;
	}
}

void Enemy::Update()
{
	if(destroy_timer_test_ >= 1.0f)
	{
		Destroy(this);
	}
}

void Enemy::Infomation()
{
	ImGui::DragInt("HP", &hp_);
}
