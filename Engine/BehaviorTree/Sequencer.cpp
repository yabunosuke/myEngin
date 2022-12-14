#include "Sequencer.h"

void behaviorTree::Sequencer::OnChildExecuted(BehaviorStatus childStatus)
{
	current_child_index_++;
	CompositeNode::OnChildExecuted(childStatus);
}

bool behaviorTree::Sequencer::CanExecute()
{
	bool execute
	{
		current_child_index_ < children_.size() &&
		status_ != BehaviorStatus::Failure
	};
	return execute;
}
