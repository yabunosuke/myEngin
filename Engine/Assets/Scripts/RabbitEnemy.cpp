#include "RabbitEnemy.h"
#include "Time/Time.h"

RabbitEnemy::RabbitEnemy():
	EnemyBase("RabbitEnemy")
{
	state_update_[RabbitEnemyState::Idole] = &RabbitEnemy::Idole;
}

void RabbitEnemy::OnTriggerEnter(Collider &other)
{
	if (other.game_object_->tag == "Player")
	{
		Vector3 player_2_enemy{ other.game_object_->transform_->position - transform_->position };
		// 一定距離以下なら攻撃フラグを立てる
		if (player_2_enemy.Magnitude() <= 3.0f)
		{
			is_attack_ = true;
		}

		state_ = RabbitEnemyState::Contact;
	}
}

void RabbitEnemy::OnTriggerExit(Collider &other)
{
	if (other.game_object_->tag == "Player")
	{
		state_ = RabbitEnemyState::Idole;
	}
}

void RabbitEnemy::Awake()
{
}

void RabbitEnemy::FixedUpdate()
{
	(this->*state_update_[state_])(true);
}

void RabbitEnemy::Update()
{
	(this->*state_update_[state_])(false);
}

void RabbitEnemy::Idole(bool is_fixed)
{
	if (!is_fixed)
	{
		// プレイヤーが範囲内に入ったなら攻撃に移行
		if (is_target_)
		{
			state_ = RabbitEnemyState::Contact;
		}
	}
	else
	{
		if (idol_move_timer_ >= k_idol_move_cooldown)
		{
			idol_move_timer_ = 0.0f;
			// ジャンプ方向決定
			float rand_angle { static_cast<float>(rand() % 361)};
			Vector3 jump_vec{ 0,2.0f,1.0f };
			jump_vec = Quaternion(Vector3::up, rand_angle) * jump_vec;
			rigidbody_->AddForce(jump_vec, ForceMode::VelocityChange);
		}
		else
		{
			idol_move_timer_ += Time::GetInstance()->fixedDeltaTime;
		}
	}

}

void RabbitEnemy::Contact(bool is_fixed)
{
	if (!is_fixed)
	{

	}
	else
	{
		if (contact_move_timer_ >= k_contact_move_cooldown)
		{
			// 攻撃範囲に入っていれば攻撃に移行
			if (is_attack_)
			{
				state_ = RabbitEnemyState::HeightJumpAttack;
			}


			contact_move_timer_ = 0.0f;
			transform_->LookAt(Vector3::Scale(target_position_, Vector3(1, 0, 1)));
			
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


void RabbitEnemy::HeightJumpAttack(bool is_fixed)
{
}
