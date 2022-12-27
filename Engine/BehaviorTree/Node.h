#pragma once

#include "Object/GameObject/GameObject.h"
#include "Property.h"

namespace behaviorTree
{
	enum class BehaviorStatus
	{
		Inactive,		// ��A�N�e�B�u
		Sucess,		// ����
		Failure,		// ���s
		Runnning,		// ���s��
		Completed		// ����
	};

	/// <summary>
	/// �m�[�h�̊��N���X
	/// </summary>
	class Node
	{
	public:
		Node();

		virtual void OnAwake() {};
		virtual void OnStart();
		virtual BehaviorStatus OnUpdate();
		virtual void OnEnd();
		virtual void OnAbort();

		/// <summary>
		/// �q�m�[�h��ǉ�
		/// </summary>
		/// <param name="chuld">�ǉ�����q�m�[�h</param>
		virtual void AddNode(Node *child) {};
		/// <summary>
		/// �q�m�[�h�𕡐��ǉ�
		/// </summary>
		/// <param name="chuld">�ǉ�����q�m�[�h</param>
		/// 
		/// 

		// �A�N�Z�b�T
		yEngine::Property<GameObject*> Owner
		{
			&owner_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<int> Index
		{
			&index_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<Node*> ParentNode
		{
			&parent_node_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<std::string> Name
		{
			&name_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<BehaviorStatus> Status
		{
			&status_,
			yEngine::AccessorType::ReadOnly
		};

	protected:
		GameObject *owner_{ nullptr };
		Node *parent_node_{ nullptr };
		std::string name_;
		int index_{ -1 };
		BehaviorStatus status_{BehaviorStatus::Inactive};
	};
}
