#pragma once
#include "../../Engine/Object/Component/Camera.h"

class CameraManager
{
public:

	Camera *main_camera_;
	//オブジェクト
	std::vector<Camera *> cameras_;
};

