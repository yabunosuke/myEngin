#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"
class TitleManager final : 
	public MonoBehaviour
{
public:
	TitleManager(
		GameObject *title,
		GameObject *start_text,
		GameObject *end_text
	);

	void FixedUpdate() override;

private:
	GameObject *title_;
	GameObject *start_text_;
	GameObject *end_text_;
	
	int select_num_{ 0 };
};

