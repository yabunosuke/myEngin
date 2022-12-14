#pragma once
#include "Node.h"

namespace behaviorTree
{
	/// <summary>
	/// ��������p�m�[�h
	/// </summary>
	class CompositeNode :
		public Node
	{
	public:
		CompositeNode() {};

		virtual bool CanExecute();
		/// <summary>
		/// ���f���Ăяo��
		/// </summary>
		/// <param name="childNodeIndex">���f���Ăяo�����q�m�[�h�̑S�̂ł̃C���f�b�N�X</param>
		virtual void OnConditionalAbort(int childNodeIndex);
		/// <summary>
		/// �q�m�[�h�̎��s���I������ۂɌĂяo��
		/// ���ׂẴm�[�h�����s�����������̏ꍇ�̓R���v���[�g�ɂ���
		/// </summary>
		/// <param name="childStatus"></param>
		virtual void OnChildExecuted(BehaviorStatus childStatus);

		/// <summary>
		/// �S�Ă̎q�m�[�h��OnAwake�Ăяo��
		/// </summary>
		void OnAwake() override;
		/// <summary>
		/// �X�^�[�g�Ăяo��
		/// </summary>
		void OnStart() override;
		void OnAbort() override;
		BehaviorStatus OnUpdate() override;

		void AddNode(Node *child) override;

		// �A�N�Z�b�T
		yEngine::Property<bool> NeedsConditionalAbort
		{
			&node_conditional_abort_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<int> CurrentChildIndex
		{
			&current_child_index_,
			yEngine::AccessorType::ReadOnly
		};
		yEngine::Property<std::vector<std::unique_ptr<Node>>> Children
		{
			&children_,
			yEngine::AccessorType::ReadOnly

		};


	protected:
		// ���݂̃C���f�b�N�X
		int current_child_index_{ 0 };
		// �����̎q�m�[�h
		std::vector<std::unique_ptr<Node>> children_;

	private:
		bool node_conditional_abort_{ false };
		bool has_conditional_node_{ false };


	};

}
