#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
using namespace behaviorTree;

class Boss :
    public EnemyBase
{
public:
    Boss();
    void Start() override;
    void FixedUpdate() override;
    void Update() override;

    void Infomation() override;
private:

    GameObject *player_;
    std::unique_ptr<SimpleBehaviorTree> behavior_tree_;

    void CreateTree();
    enum class StateFlag {
        Spawn,
        Damage,
        Attack,
        Stay,
        Move,
    }state_flag_{ StateFlag::Spawn };

    int attack_num_ {1};

    // ï‡çsë¨ìx
    float move_speed_{4.0f};

    GameObject *left_hand_, *right_hand_;

	bool spawn_animation_{ false };

    Vector3 move;

    bool attack_{ false };
    bool for_point1_{ true };
    Vector3 point2_{ -10,0,0 };
};

