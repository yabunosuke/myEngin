#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"

class EnemyBase :
    public MonoBehaviour
{
public:
	EnemyBase(const std::string &name);
	

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;

protected:
	Fbx *model_data_;
	Rigidbody *rigidbody_{ nullptr };
	// ‘Ì—Í
	float max_hp_{10.0f};
	float hp_{max_hp_};
};

