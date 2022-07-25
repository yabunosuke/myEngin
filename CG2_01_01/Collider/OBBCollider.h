#pragma once
#include "BaseCollider.h"
class OBBCollider :
    public BaseCollider
{
public:
	OBBCollider(std::string name = "OBB", XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f)
	{
		// 球形状をセット
		shapeType = SHAPE_OBB;
		this->collisionName = name;
	}

};

