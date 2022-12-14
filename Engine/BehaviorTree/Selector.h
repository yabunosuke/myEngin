#pragma once
#include "CompositeNode.h"

namespace behaviorTree
{
    // ��������s����
    class Selector :
        public CompositeNode
    {
    public:
        /// <summary>
        /// �q�m�[�h�̎��s��ɌĂяo��
        /// </summary>
        /// <param name="childStatus"></param>
        void OnChildExecuted(BehaviorStatus childStatus) override;
        
        bool CanExecute() override;
    };
}

