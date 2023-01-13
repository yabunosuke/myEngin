#include "Object.h"
#include "GameObject/GameObject.h"
#include "Component/Component.h"
#include "Time/Time.h"
#include <iostream>

int Object::id_counter_ = 0;


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
	ImGui::Text("Object Count -> %d", Singleton<ObjectManager>::GetInstance().objects_.size());
	ImGui::End();

	// タイマー処理
	for (int index = 0; index < Singleton<ObjectManager>::GetInstance().objects_.size(); ++index)
	{
		if ((*(Singleton<ObjectManager>::GetInstance().objects_.begin() + index))->is_destroy_)
		{
			(*(Singleton<ObjectManager>::GetInstance().objects_.begin() + index))->destroy_timer_ -= Singleton<Time>::GetInstance().time;
			
			if ((*(Singleton<ObjectManager>::GetInstance().objects_.begin() + index))->destroy_timer_ <= 0.0f)
			{
				(*(Singleton<ObjectManager>::GetInstance().objects_.begin() + index))->DestoryRelated();
			}

		}
	}

	// 削除処理
	auto it = std::remove_if(Singleton<ObjectManager>::GetInstance().objects_.begin(), Singleton<ObjectManager>::GetInstance().objects_.end(),
		[=](std::shared_ptr<Object> &object)
		{
			if (object == nullptr) return false;
			bool is_remove = object->destroy_timer_ <= 0.0f && object->is_destroy_;

			return is_remove;
		}
	);
	Singleton<ObjectManager>::GetInstance().objects_.erase(it, Singleton<ObjectManager>::GetInstance().objects_.end());
}

void ObjectManager::Save()
{
	{

		// バイナリ書き出し
		std::ofstream ofs("test.json", std::ios::binary);
		cereal::JSONOutputArchive serealization(ofs);
		serealization(cereal::make_nvp("root", *this));
	}

}

