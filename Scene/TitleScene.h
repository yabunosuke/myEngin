#pragma once
#include "Scene/AbstractScene/AbstractScene.h"
#include <vector>
#include <string>

class TitleScene final
	:public AbstractScene
{
public:
	explicit TitleScene(IoChangedListener *impl);
	void Initialize() override;
	void Draw() const override;

};

