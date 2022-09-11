#include "Object.h"

int Object::static_id_ = 0;
std::vector<Object*> Object::objects_;


Object::Object():
instance_id_(++static_id_)
{
	objects_.emplace_back(this);
}

void Object::Destroy(Object *object, float t)
{
}
