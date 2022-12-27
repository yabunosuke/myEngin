#include "Boss.h"
#include <Object3dComponent.h>

Boss::Boss():
	EnemyBase("Boss Test")
{
}

void Boss::Start()
{
	player_ = GameObject::Find("Player");
	model_data_ =
		game_object_->GetComponent<Object3dComponent>()->GetObjectData();
	rg_ = game_object_->GetComponent<Rigidbody>();
	CreateTree();
}

void Boss::Update()
{
	behavior_tree_->Update();
}

void Boss::Infomation()
{
	std::stack<int> test = behavior_tree_->ActiveStack;
	int size = test.size();
	int a = 0;
	while (test.size())
	{
		
		ImGui::Text(behavior_tree_->AllNode.r_[test.top()]->Name->c_str());
		test.pop();

	}
	/*for (auto &node : behavior_tree_->AllNode.r_)
	{
		ImGui::Text(node->Name->c_str());
	}*/
}

void Boss::CreateTree()
{
	// ツリー作成
	behavior_tree_ = std::make_unique<SimpleBehaviorTree>(game_object_);

	// 登場フラグ
	ConditionalNode *spawn_check = new ConditionalNode
	(
		[&](GameObject *owner)
		{
			return state_flag_ == StateFlag::Spawn ?
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	spawn_check->Name = "Condition : Spawn Check";

	// スポーン時にPlayer方向を向く
	ActionNode *spawn_rotate_player = new ActionNode
	(
		[&](GameObject *owner)
		{
			owner->transform_->LookAt({ player_->transform_->position->x,owner->transform_->position->y,player_->transform_->position->z});
			return BehaviorStatus::Sucess;
		}
	);
	spawn_rotate_player->Name = "Action : Spawn Rotate Player";

	// 登場時アニメーションを行う
	ActionNode *spawn_animation = new ActionNode
	(
		[&](GameObject *owner)
		{
			// 一度だけアニメーションする
			if(!spawn_animation_)
			{
				spawn_animation_ = true;
				model_data_->PlayAnimation(0, false);
			}
			// アニメーションが終了したらフラグ切り替え
			if (model_data_->IsPlayAnimation(0) == false)
			{
				state_flag_ = StateFlag::Move;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	spawn_animation->Name = "Action : Spown Animation";

	// シーケンサ
	Sequencer *seq1 = new Sequencer();
	seq1->Name = "Sequencer1";
	seq1->NeedsConditionalAbort = true;
	seq1->AddNode(spawn_check);			//
	seq1->AddNode(spawn_rotate_player);	//
	seq1->AddNode(spawn_animation);		//

	// 歩行フラグ
	ConditionalNode *move_check = new ConditionalNode
	(
		[&](GameObject *owner)
		{
			return state_flag_ == StateFlag::Move ?
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	move_check->Name = "Condition : Move Check";

	// Playerに進む
	ActionNode *move_to_player = new ActionNode
	(
		[&](GameObject *owner)
		{
			model_data_->PlayAnimation(6);
			// 回転
			owner->transform_->LookAt({ player_->transform_->position->x,owner->transform_->position->y,player_->transform_->position->z });

			Vector3 dir
			{
				(player_->transform_->position - owner->transform_->position).Normalized()
			};
			owner->transform_->position += dir * 0.01f;

			const float ep{ 1.0f };
			Vector3 dv
			{
				player_->transform_->position - owner->transform_->position
			};
			// 誤差範囲内になったら終了
			if (fabsf(dv.x) <= ep &&
				fabsf(dv.y) <= ep &&
				fabsf(dv.z) <= ep)
			{
				state_flag_ = StateFlag::Attack;
				attack_ = true;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move_to_player->Name = "Action : Move To Player";

	Sequencer *seq2 = new Sequencer();
	seq2->Name = "Sequencer2";
	seq2->NeedsConditionalAbort = true;
	seq2->AddNode(move_check);
	seq2->AddNode(move_to_player);


	ConditionalNode *Attack_check = new ConditionalNode
	(
		[&](GameObject *owner)
		{
			return state_flag_ == StateFlag::Attack ?
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	Attack_check->Name = "Condition : Attack Check";
	

	// 攻撃アニメーションを行う
	ActionNode *attack1 = new ActionNode
	(
		[&](GameObject *owner)
		{
			if (attack_) {
				model_data_->PlayAnimation(1,false);
				attack_ = false;
			}
			// アニメーションが終了したらフラグ切り替え
			else if (model_data_->IsPlayAnimation(1) == false)
			{
				state_flag_ = StateFlag::Move;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	attack1->Name = "Action : Attack 1";

	ActionNode *attack2 = new ActionNode
	(
		[&](GameObject *owner)
		{
			// 一度だけアニメーションする
			if (attack2->Status != BehaviorStatus::Runnning)
			{
				model_data_->PlayAnimation(2, false);
			}
			// アニメーションが終了したらフラグ切り替え
			if (model_data_->IsPlayAnimation(2) == false)
			{
				state_flag_ = StateFlag::Move;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	attack2->Name = "Action : Attack 2";

	ActionNode *attack3 = new ActionNode
	(
		[&](GameObject *owner)
		{
			// 一度だけアニメーションする
			if (attack3->Status != BehaviorStatus::Runnning)
			{
				model_data_->PlayAnimation(3, false);
			}
			// アニメーションが終了したらフラグ切り替え
			if (model_data_->IsPlayAnimation(3) == false)
			{
				state_flag_ = StateFlag::Move;
				return BehaviorStatus::Sucess;
			}

			return BehaviorStatus::Runnning;
		}
	);
	attack3->Name = "Action : Attack 3";


	Selector *attack_selector = new Selector();
	attack_selector->Name = "AttackSelector";
	attack_selector->AddNode(attack1);
	attack_selector->AddNode(attack2);
	attack_selector->AddNode(attack3);


	Sequencer *seq3 = new Sequencer();
	seq3->NeedsConditionalAbort = true;
	seq3->Name = "Sequencer3";
	seq3->AddNode(Attack_check);
	seq3->AddNode(attack_selector);


	Selector *selector = new Selector();
	selector->Name = "Selector";
	selector->AddNode(seq1);
	selector->AddNode(seq2);
	selector->AddNode(seq3);

	Repeater *repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(selector);
	behavior_tree_->SetRootNode(repeater);

	behavior_tree_->Start();
}
