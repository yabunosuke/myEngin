#pragma once
#include "AbstractScene.h"

#include <vector>
#include "Sprite.h"

#include "GameObjCommon.h"

class TestScene
	:public AbstractScene
{
public:

	TestScene(IoChangedListener *impl);
	virtual ~TestScene() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	
};