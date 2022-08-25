#include "Object.h"

int Object::static_id_ = 0;

Object::Object():
instance_id_(++static_id_)
{
}

void Object::Destroy(Object *object, float t)
{
}
