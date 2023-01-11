#include "Object.h"
#include "GameObject/GameObject.h"
#include "Component/Component.h"
#include "Time/Time.h"
#include <iostream>

int Object::id_counter_ = 0;
std::vector<std::shared_ptr<Object>> Object::objects_;

Object::Object(const std::string &name) :
	instance_id_(id_counter_++),
	name_(name),
	is_destroy_(false),
	destroy_timer_(0.0f)
{
}

Object::~Object()
{
}


void Object::Destroy(Object *destroy_object, float t)
{
	destroy_object->destroy_timer_ = t;
	destroy_object->is_destroy_ = true;

}

void Object::Destroyer()
{

	ImGui::Begin("Object Count");
	ImGui::Text("Object Count -> %d", objects_.size());
	ImGui::End();

	// タイマー処理
	for (int index = 0; index < objects_.size(); ++index)
	{
		if ((*(objects_.begin() + index))->is_destroy_)
		{
			(*(objects_.begin() + index))->destroy_timer_ -= Time::GetInstance()->time;
			
			if ((*(objects_.begin() + index))->destroy_timer_ <= 0.0f)
			{
				(*(objects_.begin() + index))->DestoryRelated();
			}

		}
	}

	// 削除処理
	auto it = std::remove_if(objects_.begin(), objects_.end(),
		[=](std::shared_ptr<Object> &object)
		{
			if (object == nullptr) return false;
			bool is_remove = object->destroy_timer_ <= 0.0f && object->is_destroy_;

			return is_remove;
		}
	);
	objects_.erase(it, objects_.end());
}



