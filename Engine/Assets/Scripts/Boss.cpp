#include "Boss.h"

Boss::Boss():
	EnemyBase("Boss Test")
{
}

void Boss::Start()
{
	// ツリー作成
	behavior_tree_ = std::make_unique<SimpleBehaviorTree>(game_object_);
	
	ConditionalNode *condition = new ConditionalNode
	(
		[&](GameObject *owner)
		{
			return for_point1_ ? BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);

	// ターゲットを向く
	ActionNode *rotate_point1 = new ActionNode
	(
		[&](GameObject *owner)
		{
			owner->transform_->LookAt(point1_);
			return BehaviorStatus::Sucess;
		}
	);
	rotate_point1->Name = "RotateToPoint1";

	// 正面に進む
	ActionNode *move_point1 = new ActionNode
	(
		[&](GameObject *owner)
		{
			Vector3 dir
			{
				(point1_ - owner->transform_->position).Normalized()
			};
			owner->transform_->position += dir * 0.01f;

			const float ep{ 0.1f };
			Vector3 dv
			{
				point1_ - owner->transform_->position
			};
			// 誤差範囲内になったら終了
			if (fabsf(dv.x) <= ep &&
				fabsf(dv.y) <= ep &&
				fabsf(dv.z) <= ep)
			{
				for_point1_ = false;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move_point1->Name = "MoveToPoint1";

	// シーケンサ
	Sequencer *seq1 = new Sequencer();
	seq1->Name = "Sequencer1";
	seq1->AddNode(condition);
	seq1->AddNode(rotate_point1);
	seq1->AddNode(move_point1);


	// ターゲットを向く
	ActionNode *rotate_point2 = new ActionNode
	(
		[&](GameObject *owner)
		{
			owner->transform_->LookAt(point2_);
			return BehaviorStatus::Sucess;
		}
	);
	rotate_point2->Name = "RotateToPoint2";

	// 正面に進む
	ActionNode *move_point2 = new ActionNode
	(
		[&](GameObject *owner)
		{
			Vector3 dir
			{
				(point2_ - owner->transform_->position).Normalized()
			};
			owner->transform_->position += dir * 0.01f;

			const float ep{ 0.1f };
			Vector3 dv
			{
				point2_ - owner->transform_->position
			};
			// 誤差範囲内になったら終了
			if (fabsf(dv.x) <= ep &&
				fabsf(dv.y) <= ep &&
				fabsf(dv.z) <= ep)
			{
				for_point1_ = false;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move_point2->Name = "MoveToPoint2";

	ActionNode *move_up = new ActionNode
	(

		[&](GameObject *owner)
		{
			owner->transform_->position += Vector3::up * 0.01f;
			if (owner->transform_->position->y >= 3.0f)
			{
				for_point1_ = true;
				return BehaviorStatus::Sucess;
			}
			return BehaviorStatus::Runnning;
		}
	);
	move_up->Name = "MoveUp";

	Sequencer *seq3 = new Sequencer();
	seq3->Name = "Sequencer3";
	seq3->AddNode(move_point2);
	seq3->AddNode(move_up);

	Sequencer *seq2 = new Sequencer();
	seq2->Name = "Sequencer2";
	seq2->AddNode(rotate_point2);
	seq2->AddNode(seq3);

	Selector *selector = new Selector();
	selector->Name = "Selector";
	selector->AddNode(seq1);
	selector->AddNode(seq2);

	Repeater *repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(selector);
	behavior_tree_->SetRootNode(repeater);

	behavior_tree_->Start();
}

void Boss::Update()
{
	behavior_tree_->Update();
}
