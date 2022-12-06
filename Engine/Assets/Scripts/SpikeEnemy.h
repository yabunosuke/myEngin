#pragma once
#include "EnemyBase.h"

enum class SpikeEnemyState
{
    Idle,
    RandomJump,
    Contact,
    HeightJumpAttack,
    Damage,
    Death,


};

enum class SpikeAnimation
{
    Bite_Front,
    Dance,
    Death,
    HitRective,
    Idle,
    Jump,
    No,
    Walk,
    Yes,
};

class SpikeEnemy :
    public EnemyBase
{
public:
    SpikeEnemy();

    void OnCollisionEnter(Collision &collision) override;
    void OnCollisionStay(Collision &collision) override;
    void OnCollisionExit(Collision &collision) override;

    void OnTriggerEnter(Collider &other) override;

    void Awake() override;
    void FixedUpdate() override;
    void Update() override;

    void Infomation() override;



    bool is_target_{ false };
    Vector3 target_position_{};
private:

    std::map<SpikeEnemyState, void(SpikeEnemy:: *)(bool)> state_update_;

    // ê⁄ínÉtÉâÉO
    bool is_ground_{ false };

    void Idole(bool is_fixed);
    float k_idol_move_cooldown{ 2.0f };
    float idol_move_timer_{ 0.0f };

    void RandomJump(bool is_fixed);
    bool is_jump_{ false };

    void Contact(bool is_fixed);
    const float k_contact_move_cooldown{ 2.0f };
    float contact_move_timer_{ 0.0f };

    void HeightJumpAttack(bool is_fixed);
    bool is_attack_{ false};

    void Damage(bool is_fixed);
    const float k_invincible_cooldown_{ 0.5f };
    float invincible_timer_{ 0.0f };

    void Death(bool is_fixed);
    const float k_death_counter_{ 2.0f };
    float death_timer_{ 0.0f };
    bool is_play_death_animation_{ false };
    Vector3 death_start_scale_{};

    SpikeEnemyState state_ = SpikeEnemyState::Idle;



};

