#include "SpikeEnemy.h"
#include "Time/Time.h"
#include "Math/Easing.h"

SpikeEnemy::SpikeEnemy():
	EnemyBase("SpikeEnemy")
{
	state_update_[SpikeEnemyState::Idle] = &SpikeEnemy::Idole;
	state_update_[SpikeEnemyState::Damage] = &SpikeEnemy::Damage;
	state_update_[SpikeEnemyState::Death] = &SpikeEnemy::Death;
	state_update_[SpikeEnemyState::Contact] = &SpikeEnemy::Contact;
	state_update_[SpikeEnemyState::HeightJumpAttack] = &SpikeEnemy::HeightJumpAttack;
	state_update_[SpikeEnemyState::RandomJump] = &SpikeEnemy::RandomJump;
}

void SpikeEnemy::OnCollisionEnter(Collision &collision)
{
	std::string tag = collision.gameObject.r_->tag;
	if (tag == "Stage")
	{
		if (Vector3::Dot(collision.contactPoint->normal, Vector3::up) >= 0.8f)
		{
			is_ground_ = true;
		}
	}
}

void SpikeEnemy::OnCollisionStay(Collision &collision)
{
	std::string tag = collision.gameObject.r_->tag;
	if (tag == "Stage")
	{
		if (Vector3::Dot(collision.contactPoint->normal, Vector3::up) >= 0.8f)
		{
			is_ground_ = true;
		}
	}
}

void SpikeEnemy::OnCollisionExit(Collision &collision)
{
	if (collision.gameObject.r_->tag == "Stage")
	{

	}
}

void SpikeEnemy::OnTriggerEnter(Collider &other)
{
	std::string tag = other.game_object_->tag;
	if (tag == "Weapon")
	{
		if (true)
		{
			hp_--;
			if (hp_ <= 0.0f)
			{
				state_ = SpikeEnemyState::Death;
			}
			else
			{
				state_ = SpikeEnemyState::Damage;
			}

		}
	}

}

void SpikeEnemy::Awake()
{
	hp_ = 4;
}

void SpikeEnemy::FixedUpdate()
{
	(this->*state_update_[state_])(true);
	is_ground_ = false;
	if(Vector3::Scale(Vector3{ 1,0,1 }, rigidbody_->velocity) != Vector3::zero)
	{
		transform_->LookAt(transform_->position + Vector3::Scale(Vector3{ 1,0,1 }, rigidbody_->velocity));
	}
}

void SpikeEnemy::Update()
{
	(this->*state_update_[state_])(false);
}

void SpikeEnemy::Infomation()
{
	EnemyBase::Infomation();
	ImGui::BeginDisabled();
	int state_num = static_cast<int>(state_);
	ImGui::DragInt("State Num", &state_num);
	ImGui::EndDisabled();

}

void SpikeEnemy::Idole(bool is_fixed)
{
	model_data_->PlayAnimation(static_cast<int>(SpikeAnimation::Idle));

	if (!is_fixed)
	{
		// プレイヤーが範囲内に入ったなら攻撃に移行
		if (is_target_)
		{
			state_ = SpikeEnemyState::Contact;
		}
	}
	else
	{
		if (idol_move_timer_ >= k_idol_move_cooldown)
		{
			idol_move_timer_ = 0.0f;
			state_ = SpikeEnemyState::RandomJump;
		}
		else
		{
			idol_move_timer_ += Time::GetInstance()->fixedDeltaTime;
		}
	}

}

void SpikeEnemy::RandomJump(bool is_fixed)
{
	if (!is_fixed)
	{

	}
	else
	{
		if (!is_jump_)
		{
			is_jump_ = true;
			// ジャンプ方向決定
			float rand_angle{ static_cast<float>(rand() % 361) };
			Vector3 jump_vec{ 0,5.0f,1.0f };
			jump_vec = Quaternion(Vector3::up, rand_angle) * jump_vec;
			rigidbody_->AddForce(jump_vec, ForceMode::VelocityChange);
			model_data_->PlayAnimation(static_cast<int>(SpikeAnimation::Jump), false, true);
		}
		else
		{
			if (is_ground_ && rigidbody_->velocity->y < 0.0f)
			{
				is_jump_ = false;
				state_ = SpikeEnemyState::Idle;
			}
		}

	}
}

void SpikeEnemy::Contact(bool is_fixed)
{
	if (!is_fixed)
	{
	}
	else
	{
		if (Vector3::Scale(target_position_, Vector3(1, 0, 1)) != Vector3::zero)
		{
			transform_->LookAt(Vector3::Scale(target_position_, Vector3(1, 0, 1)));
		}
		if (contact_move_timer_ >= k_contact_move_cooldown)
		{
			// 攻撃範囲に入っていれば攻撃に移行
			if ((transform_->position - target_position_).Magnitude() <= 3.0f)
			{
				state_ = SpikeEnemyState::HeightJumpAttack;
				return;
			}

			contact_move_timer_ = 0.0f;
			
			// ジャンプ方向決定
			Vector3 jump_vec{ transform_->forward };
			jump_vec *= 2.0f;
			jump_vec.y += 1.0f;
			rigidbody_->AddForce(jump_vec, ForceMode::VelocityChange);
		}
		else
		{
			contact_move_timer_ += Time::GetInstance()->fixedDeltaTime;
		}
	}
}


void SpikeEnemy::HeightJumpAttack(bool is_fixed)
{

	if (!is_fixed)
	{

	}
	else
	{
		if(!is_attack_)
		{
			Vector3 jump_vec{ Vector3::Scale((target_position_ - transform_->position).Normalized(),Vector3{1,0,1})};
			jump_vec *= 4.0f;
			jump_vec.y += 2.0f;
			rigidbody_->AddForce(jump_vec, ForceMode::VelocityChange);
			model_data_->PlayAnimation(static_cast<int>(SpikeAnimation::Bite_Front), false);
			is_attack_ = true;
		}
		else
		{
			if(is_ground_)
			{
				state_ = SpikeEnemyState::Idle;
				is_attack_ = false;
			}
		}

	}
}

void SpikeEnemy::Damage(bool is_fixed)
{
	model_data_->PlayAnimation(static_cast<int>(SpikeAnimation::HitRective),false);

	if (!is_fixed)
	{

	}
	else
	{
		if (invincible_timer_ >= k_invincible_cooldown_)
		{
			invincible_timer_ = 0.0f;
			state_ = SpikeEnemyState::Idle;
		}
		else
		{
			invincible_timer_ += Time::GetInstance()->fixedDeltaTime;
		}

	}
}

void SpikeEnemy::Death(bool is_fixed)
{

	if (!is_play_death_animation_)
	{
		model_data_->PlayAnimation(static_cast<int>(SpikeAnimation::Death), false);
		death_start_scale_ = transform_->scale;
		is_play_death_animation_ = true;
	}
	if (!is_fixed)
	{

	}
	else
	{
		if (death_timer_ >= k_death_counter_)
		{
			GameObject::Destroy(game_object_);
		}
		else
		{
			death_timer_ += Time::GetInstance()->deltaTime;
			if (death_timer_ >= k_death_delay_)
			{
			transform_->scale =
				Ease(Out, Bounce,
					(death_timer_ - k_death_delay_) / (k_death_counter_ - k_death_delay_),
					death_start_scale_, Vector3{ 0.0f,0.0f,0.0f });
			}
		}
	}

}
