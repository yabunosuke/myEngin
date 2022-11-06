#include "Drone.h"

#include "Input.h"
#include "Object3dComponent.h"
#include "PlayerBullet.h"
#include "Object/Component/Camera.h"
#include "Object/Component/Collider/SphereCollider/SphereCollider.h"
#include "Object/Component/Light.h"
#include "Time/Time.h"

Drone::Drone(Vector3 *axis_pos):
	MonoBehaviour("Drone"),
axis_pos_(axis_pos)
{
}

void Drone::Start()
{
	transform_->scale = { 0.5f,0.5f,0.5f };
}

void Drone::FixedUpdate()

{
	Vector3 camera_right = Vector3::Scale(Camera::main.r_->transform_->GetRight(), Vector3(1.0f, 1.0f, 1.0f)).Normalized();
	transform_->position = *axis_pos_ + camera_right * 1.0f + Vector3::up * 1.0f;
	transform_->quaternion = Camera::main.r_->transform_->quaternion.r_;
}

void Drone::Update()
{
	static float shot_interval = 0;

	if(Input::GetButtonPress(GamePadButton::INPUT_RB))
	{
		if(shot_interval >= 0.4f || Input::GetButtonPressTrigger(GamePadButton::INPUT_RB))
		{
			auto bullet = GameObject::CreateObject("Bullet");
			bullet->tag = "Weapon";
			bullet->AddComponent<Object3dComponent>(
				DirectXCommon::dev.Get(), DirectXCommon::cmdList.Get(),
				"Assets/3d/webtrcc.fbx");
			bullet->transform_->position = transform_->position.r_ + transform_->GetFront() * 0.2f;
			bullet->transform_->scale= {0.2f,0.2f,0.2f};
			bullet->AddComponent<Rigidbody>();
			auto c = bullet->AddComponent<SphereCollider>();
			c->isTrigger = true;
			bullet->AddComponent<PlayerBullet>(transform_->position.r_, transform_->quaternion.r_);
			auto light = bullet->AddComponent<Light>();
			light->color = {0,0,1,1};
			light->intensity = 2.0f;
			light->range = 3.0f;
			shot_interval = 0.0f;
		}
		else
		{
			shot_interval += Time::GetInstance()->time;
		}
	}
	else
	{
		shot_interval = 0.0f;
	}

}
