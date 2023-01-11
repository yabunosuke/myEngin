#include "TitleManager.h"
#include "Time/Time.h"
#include "Input.h"

TitleManager::TitleManager(GameObject *title, GameObject *start_text, GameObject *end_text,IoChangedListener *impl_scene_changed):
	MonoBehaviour("Title Manager"),
	title_(title),
	start_text_(start_text),
	end_text_(end_text),
	impl_scene_changed_(impl_scene_changed)
{
	title_->transform->lock()->localQuaternion = Quaternion::Euler(0, 11.0f * Mathf::deg_to_rad, 0);
	title_->transform->lock()->scale = {1.4f,1.4f,1.4f};
}

void TitleManager::Update()
{
	float stick_y = Input::GetAxis(GamePadAxis::AXIS_LY);
	if (stick_y > 0)
	{
		select_num_ = 1;
		end_text_->transform->lock()->scale = { 1.2f,1.2f,1.2f };
		start_text_->transform->lock()->scale = { 1.0f,1.0f,1.0f };
	}
	else if (stick_y < 0)
	{
		select_num_ = 0;
		start_text_->transform->lock()->scale = { 1.2f,1.2f,1.2f };
		end_text_->transform->lock()->scale = { 1.0f,1.0f,1.0f };
	}

	if (Input::GetButtonPressTrigger(GamePadButton::INPUT_A))
	{
		if (select_num_ == 0)
		{
			impl_scene_changed_->OnSceneChanged(Scenes::Game);
		}
		else
		{
			impl_scene_changed_->ExitWindow();
		}
	}
}
void TitleManager::FixedUpdate()
{
	title_->transform->lock()->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f + 2.0f,
		-1.0f,
	};


	start_text_->transform->lock()->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f - 0.5f,
		-1.0f,
	};
	

	end_text_->transform->lock()->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f - 1.8f,
		-1.0f,
	};



}
