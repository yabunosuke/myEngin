#include "Object.h"

int Object::static_id_ = 0;
std::vector<std::shared_ptr<Object>> Object::objects_;



Object::Object():
instance_id_(++static_id_)
{
}

Object::~Object()
{
}


//Object::~Object()
//{
//
//}

void Object::Destroy(std::weak_ptr<Object> object, float t)
{
	object.lock().reset();
}
