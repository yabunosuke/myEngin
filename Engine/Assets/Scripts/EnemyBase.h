#pragma once
#include "3d/Fbx/Fbx.h"
#include "Object/Component/Rigidbody.h"
#include "Object/Component/Behaviour/MonoBehaviour/MonoBehaviour.h"

class EnemyBase :
    public MonoBehaviour
{
public:
	EnemyBase();

    void OnCollisionEnter(Collision &collision) override;

	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void Infomation() override;

private:

	Rigidbody *regidbody_{ nullptr };
	// ‘Ì—Í
	float max_hp_{10.0f};
	float hp_{max_hp_};
};

