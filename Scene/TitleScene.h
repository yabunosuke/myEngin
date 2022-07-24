#pragma once
#include "2d/Sprite.h"
#include "GameObject.h"
#include "GameObjCommon.h"
#include "AbstractScene.h"
#include "CollisionPrimitive.h"
#include "Camera.h"
#include <vector>
#include <string>

class TitleScene final
	:public AbstractScene
{
public:
	explicit TitleScene(IoChangedListener *impl);
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;
private:

	Camera *cam_;
	Sprite *test_sprite;
	
};

