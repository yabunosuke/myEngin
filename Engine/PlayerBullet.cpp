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
	transform->lock()->position = start_position_;
	transform->lock()->quaternion = start_q_;
	rigidbody_ = game_object_.lock()->GetComponent<Rigidbody>().lock().get();
}

void PlayerBullet::FixedUpdate()
{
	rigidbody_->AddForce(transform->lock()->GetFront() * 1.0f, ForceMode::VelocityChange);
	transform->lock()->LookAt(transform->lock()->position + rigidbody_->velocity);
	live_timer_ += Time::GetInstance()->deltaTime;

	if(live_timer_ >= 4.0f)
	{
		Destroy(game_object_.lock().get());
	}
}
