#include "TitleManager.h"
#include "Time/Time.h"

TitleManager::TitleManager(GameObject *title, GameObject *start_text, GameObject *end_text):
	MonoBehaviour("Title Manager"),
	title_(title),
	start_text_(start_text),
	end_text_(end_text)
{
	title_->transform_->localQuaternion = Quaternion::Euler(0, 11.0f * Mathf::deg_to_rad, 0);
	title_->transform_->scale = {1.4f,1.4f,1.4f};
}

void TitleManager::FixedUpdate()
{
	title_->transform_->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f + 2.0f,
		-1.0f,
	};


	start_text_->transform_->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f - 0.5f,
		-1.0f,
	};


	end_text_->transform_->localPosition =
	{
		-3.0f,
		sinf(Time::GetInstance()->frameCount / 100.0f) * 0.2f - 1.8f,
		-1.0f,
	};

}
