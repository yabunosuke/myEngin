#pragma once
#include "2d/Sprite.h"
#include "Object/GameObject/GameObject.h"

#include "Scene/AbstractScene/AbstractScene.h"
#include "CollisionPrimitive.h"
#include <vector>
#include <string>

class TitleScene final
	:public AbstractScene
{
public:
	explicit TitleScene(IoChangedListener *impl);
	void Finalize() override;
	void Update() override;
private:

};

