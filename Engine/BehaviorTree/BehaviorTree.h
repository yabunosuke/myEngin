#pragma once
#include <vector>
#include <string>
#include <stack>
#include "Object/GameObject/GameObject.h"
#include "Object/Component/Component.h"

#include "Property.h"

#include "Node.h"
#include "ActionNode.h"
#include "ConditionalNode.h"
#include "CompositeNode.h"
#include "Repeater.h"
#include "Selector.h"
#include "Sequencer.h"

namespace behaviorTree
{
	class SimpleBehaviorTree
	{
	public:
		SimpleBehaviorTree(GameObject *owner);

		/// <summary>
		/// ���[�g�m�[�h�ݒ�
		/// </summary>
		/// <param name="node"></param>
		void SetRootNode(Node *node);

		void Start();
		void Update();


		/// <summary>
		///	�ĕ]���p�̏��
		/// </summary>
		class ConditionalReevaluate
		{
		public:
			ConditionalReevaluate() {};
			void Initialize(int i, BehaviorStatus status, int stack, int composite);
			yEngine::Property<int> Index
			{
				&Index,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<BehaviorStatus> Status
			{
				&status_,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<int> CompositeIndex
			{
				&composite_index_,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<int> StackIndex
			{
				&stack_index_,
				yEngine::AccessorType::AllAccess
			};
		private:
			int index_{ -1 };
			BehaviorStatus status_;
			int composite_index_;
			int stack_index_;
		};

	private:
		/// <summary>
		/// �A�N�e�B�u�ȃm�[�h�Ƃ��ăX�^�b�N�ɒǉ�
		/// </summary>
		/// <param name="node"></param>
		void PushNode(Node *node);
		/// <summary>
		/// �m�[�h���X�^�b�N�����菜��
		/// </summary>
		void PopNode();
		/// <summary>
		/// ��̃m�[�h�̋��ʂ̑c���������
		/// </summary>
		/// <param name="node1">1�ڂ̃m�[�hID</param>
		/// <param name="node2">2�ڂ̃m�[�hID</param>
		/// <returns></returns>
		int CommonAncestorNode(int node1, int node2);
		/// <summary>
		/// �Ώۃm�[�h�̋O��
		/// </summary>
		/// <param name="node"></param>
		void CallOnAwake(Node *node);

		int ReevaluateConditionalTasks();

		BehaviorStatus Excute(Node *node);

		GameObject *owner_;					// �c���[��ێ����Ă���I�u�W�F�N�g
		Node *root_node_;	// ��ԏ�ɂ���m�[�h

		std::vector<Node*> node_container_;	//�S�m�[�h
		std::vector<std::unique_ptr<ConditionalReevaluate>> reevaluate_container_;	// �ĕ]���p�R���e�i

		std::stack<int> active_stack_;		// ���s���m�[�h��ێ�����X�^�b�N
		
		bool is_completed_{ false };
		
		int active_node_index_{ -1 };

	};
};

