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
	if ((other.game_object_->tag == "Weapon"))
	{
		fbx_->SetColor({ 1,0,0,1 });
		--hp_;
		is_invincible_ = true;
	}

}


void FlyEnemy::Start()
{
	regidbody_ =
		game_object_->GetComponent<Rigidbody>();
	transform_->scale = { 50.0f,50.0f ,50.0f };
	fbx_ = game_object_->GetComponent<Object3dComponent>()->GetObjectData();
}

void FlyEnemy::FixedUpdate()
{
	if(is_invincible_)
	{
		if(invincible_timer_ >= k_invincible_time_)
		{
			invincible_timer_ = 0.0f;
			fbx_->SetColor({ 1,1,1,1 });
			is_invincible_ = false;
		}
		invincible_timer_ += Time::GetInstance()->time;

	}

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
