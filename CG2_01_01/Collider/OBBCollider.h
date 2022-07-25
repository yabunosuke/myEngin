#pragma once
#include "BaseCollider.h"
class OBBCollider :
    public BaseCollider
{
public:
	OBBCollider(std::string name = "OBB", XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f)
	{
		// ���`����Z�b�g
		shapeType = SHAPE_OBB;
		this->collisionName = name;
	}

};

