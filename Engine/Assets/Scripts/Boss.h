#pragma once
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
private:
	std::unique_ptr<SimpleBehaviorTree> behavior_tree_;

    bool for_point1_{ true };
    Vector3 point1_{ 10,0,0 };
    Vector3 point2_{ -10,0,0 };
};

