#pragma once
#include <functional>

#include "Node.h"

namespace behaviorTree
{
    /// <summary>
    /// �A�N�V�������s���m�[�h
    /// </summary>
    class ActionNode :
        public Node
    {
    public:
        ActionNode(std::function<BehaviorStatus(GameObject*)> action);
        BehaviorStatus OnUpdate() override;

    private:
        // ���s���e
        std::function<BehaviorStatus(GameObject *)> action_;
    };
}
