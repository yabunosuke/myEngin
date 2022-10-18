#include "Object.h"
#include "GameObject/GameObject.h"
int Object::static_id_ = 0;
std::vector<std::unique_ptr<Object>> Object::objects_;



Object::Object():
instance_id_(++static_id_)
{
}

Object::~Object()
{
}


void Object::Destroy(Object *destroy_object, float t)
{
	for(auto &object : objects_)
	{
		if(destroy_object->instance_id_ == object->instance_id_)
		{
			object.reset();
			return;
		}
	}
}

