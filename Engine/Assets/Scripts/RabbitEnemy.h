#pragma once
#include "EnemyBase.h"

enum class RabbitEnemyState
{
    Idole,

    Contact,
    HeightJumpAttack,

};

class RabbitEnemy :
    public EnemyBase
{
public:
    RabbitEnemy();

    void OnTriggerEnter(Collider &other) override;
    void OnTriggerExit(Collider &other) override;

    void Awake() override;
    void FixedUpdate() override;
    void Update() override;
private:

    std::map<RabbitEnemyState, void(RabbitEnemy:: *)(bool)> state_update_;

    void Idole(bool is_fixed);
    float k_idol_move_cooldown{ 2.0f };
    float idol_move_timer_{ 0.0f };

    void Contact(bool is_fixed);
    float k_contact_move_cooldown{ 1.0f };
    float contact_move_timer_{ 0.0f };

    void HeightJumpAttack(bool is_fixed);
    bool is_attack_{ false };

    RabbitEnemyState state_ = RabbitEnemyState::Idole;

    Rigidbody *rigidbody_;
    Fbx *model_data_{ nullptr };

    bool is_target_{ false };
    Vector3 target_position_{};

};

