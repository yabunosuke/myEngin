#pragma once
#include "CompositeNode.h"
namespace behaviorTree
{
    // ��A�̗������������
    class Sequencer :
        public CompositeNode
    {
    public:
        /// <summary>
        /// �q�m�[�h���s�I�����ɌĂяo��
        /// </summary>
        /// <param name="childStatus"></param>
        void OnChildExecuted(BehaviorStatus childStatus) override;

		bool CanExecute() override;
    };

}
