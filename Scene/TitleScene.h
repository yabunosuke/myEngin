#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "GameObjCommon.h"
#include "AbstractScene.h"
#include "CollisionPrimitive.h"
#include "DebugText.h"
#include "Camera.h"
#include <vector>
//
//#include "SkinnedMesh.h"
//
//#include "Model.h"
//#include "ModelLoader.h"
//#include "Object3d.h"

#include "Fbx.h"
#include "FbxResource.h"

class TitleScene
	:public AbstractScene
{
public:
	TitleScene(IoChangedListener *impl);
	virtual ~TitleScene() = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;
private:

	Camera *cam;

	std::shared_ptr<Fbx> test[2];
};

