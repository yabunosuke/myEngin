#include "ConditionalNode.h"

behaviorTree::ConditionalNode::ConditionalNode(std::function<BehaviorStatus(GameObject *)> condition)
{
	condition_ = condition;
}

behaviorTree::BehaviorStatus behaviorTree::ConditionalNode::OnUpdate()
{
	Node::OnUpdate();
	status_ = condition_(owner_.lock().get());
	return status_;
}
