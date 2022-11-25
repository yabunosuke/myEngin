#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"
#include <IoChangedListener.h>
class TitleManager final : 
	public MonoBehaviour
{
public:
	TitleManager(
		GameObject *title,
		GameObject *start_text,
		GameObject *end_text,
		IoChangedListener *impl_scene_changed
	);

	void Update() override;
	void FixedUpdate() override;

private:
	GameObject *title_;
	GameObject *start_text_;
	GameObject *end_text_;
	IoChangedListener *impl_scene_changed_;

	int select_num_{ -1 };
};

