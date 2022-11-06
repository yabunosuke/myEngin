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

void FlyEnemy::OnTriggerEnter(Collider &other)
{
	if ((other.game_object_->tag == "Weapon"))
	{
		transform_->scale = transform_->scale - Vector3{ 20.0f,20.0f ,20.0f };
		--hp_;
		if (hp_ < 0.0f)
		{
			Destroy(game_object_);
		}
	}

}


void FlyEnemy::Start()
{
	regidbody_ =
		game_object_->GetComponent<Rigidbody>();
	transform_->scale = { 100.0f,100.0f ,100.0f };
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
