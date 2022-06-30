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
//#include "FbxObject3d.h"

#include "Model.h"
#include "ModelLoader.h"
#include "Object3d.h"

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

	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> obj[3];
	//std::shared_ptr<SkinnedMesh> skinnedMeshes[8];
	//std::unique_ptr<FbxObject3d> obj;
};

