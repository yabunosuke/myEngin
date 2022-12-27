#include "EnemyBase.h"
#include "BehaviorTree/BehaviorTree.h"
using namespace behaviorTree;

class Boss :
    public EnemyBase
{
public:
    Boss();
    void Start() override;
    void Update() override;

    void Infomation() override;
private:
    Rigidbody *rg_;

    GameObject *player_;
    std::unique_ptr<SimpleBehaviorTree> behavior_tree_;

    void CreateTree();
    enum class StateFlag {
        Spawn,
        Attack,
        Stay,
        Move,
    }state_flag_{ StateFlag::Spawn };

    int attack_num_{ 0 };

	bool spawn_animation_{ false };


    bool attack_{ false };
    bool for_point1_{ true };
    Vector3 point2_{ -10,0,0 };
};

