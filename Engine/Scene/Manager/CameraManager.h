#pragma once
#include "../../Engine/Object/Component/Camera.h"

class CameraManager
{
public:

	Camera *main_camera_;
	//�I�u�W�F�N�g
	std::vector<Camera *> cameras_;
};

