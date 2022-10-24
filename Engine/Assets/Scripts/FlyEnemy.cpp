#include "FlyEnemy.h"
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

void FlyEnemy::OnTriggerEnter(Collision &collision)
{

	if ((collision.gameObject.r_->name.r_ == "Weapon") && collision.collider.r_->game_object_->name.r_ == "FlyEnemy")
	{
		transform_->scale = transform_->scale - Vector3{ 2,2,2 };
		--hp_;
		if (hp_ < 0.0f)
		{
			Destroy(game_object_);
		}
	}

	if ((collision.gameObject.r_->name.r_ == "Bullet") && collision.collider.r_->game_object_->name.r_ == "FlyEnemy")
	{
		transform_->scale = transform_->scale - Vector3{ 2,2,2 };
		--hp_;
		if (hp_ < 0.0f)
		{
			Destroy(game_object_);
		}
	}



}

void FlyEnemy::OnTriggerStay(Collision &collision)
{
	if (collision.gameObject.r_->name.r_ == "Player")
	{
		transform_->LookAt(collision.gameObject.r_->transform_->position);
	}
}

void FlyEnemy::Start()
{
	regidbody_ =
		game_object_->GetComponent<Rigidbody>();
	transform_->scale = { 20,20,20 };
}

void FlyEnemy::FixedUpdate()
{

	float wave_angle = 2 * Mathf::pi * fmod(static_cast<float>(Time::GetInstance()->timeAsDouble), 1.0f);
	float wave_offset = sin(wave_angle);
	Vector3 wave_vec
	{
		wave_offset,
		wave_offset,
		wave_offset
	};
	regidbody_->AddForce(wave_vec);

}

void FlyEnemy::Update()
{
}

void FlyEnemy::Infomation()
{
}
