#include "FlyEnemy.h"

#include "Object3dComponent.h"
#include "Time/Time.h"
FlyEnemy::FlyEnemy():
	EnemyBase("FlyEnemy")
{
	max_hp_ = 10.0f;
	hp_ = max_hp_;
}

void FlyEnemy::OnCollisionEnter(Collision &collision)
{

}

void FlyEnemy::OnTriggerEnter(Collider &other)
{
	if (is_invincible_) return;
	if ((other.game_object_.lock()->tag == "Weapon"))
	{
		--hp_;
		is_invincible_ = true;
	}

}


void FlyEnemy::Start()
{
	rigidbody_ =
		game_object_.lock()->GetComponent<Rigidbody>().lock().get();
	transform->lock()->scale = {50.0f,50.0f ,50.0f};
	model_data_ = game_object_.lock()->GetComponent<Object3dComponent>().lock()->GetObjectData();
}

void FlyEnemy::FixedUpdate()
{
	if(is_invincible_)
	{
		if(invincible_timer_ >= k_invincible_time_)
		{
			invincible_timer_ = 0.0f;
			is_invincible_ = false;
		}
		invincible_timer_ += Singleton<Time>::GetInstance().time;

	}

	float wave_angle = 2 * Mathf::pi * fmod(static_cast<float>(Singleton<Time>::GetInstance().timeAsDouble), 1.0f);
	float wave_offset = sin(wave_angle);
	Vector3 wave_vec
	{
		wave_offset,
		wave_offset,
		wave_offset
	};
	rigidbody_->AddForce(wave_vec);

}

void FlyEnemy::Update()
{
}

void FlyEnemy::Infomation()
{
}
