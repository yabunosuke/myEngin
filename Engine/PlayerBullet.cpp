#include "PlayerBullet.h"

#include "Time/Time.h"

PlayerBullet::PlayerBullet(const Vector3 &start_position,const Quaternion &q) :
	MonoBehaviour("Bullet")
{
	start_position_= start_position;
	start_q_ = q;
}

void PlayerBullet::Start()
{
	transform_->position = start_position_;
	transform_->quaternion = start_q_;
	rigidbody_ = game_object_->GetComponent<Rigidbody>();
}

void PlayerBullet::FixedUpdate()
{
	rigidbody_->AddForce(transform_->GetFront() * 10.0f, ForceMode::VelocityChange);
	live_timer_ += Time::GetInstance()->deltaTime;

	if(live_timer_ >= 4.0f)
	{
		Destroy(game_object_);
	}
}
