#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include "Input.h"
class GameScene :
	public AbstractScene
{
public:
	explicit GameScene(IoChangedListener *impl);
	void Initialize() override;
	void Update() override;
};

