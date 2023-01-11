#include "Boss.h"
#include <Object3dComponent.h>

Boss::Boss():
	EnemyBase("Boss Test")
{
}

void Boss::Start()
{
	player_ = GameObject::FindObject("Player");
	model_data_ =
		game_object_.lock()->GetComponent<Object3dComponent>().lock()->GetObjectData();
	rigidbody_ = game_object_.lock()->GetComponent<Rigidbody>().lock().get();
	CreateTree();

	for (const auto &obj : game_object_.lock()->GetChildren())
	{
		if (obj.lock()->name == "AttackSphereR")
		{
			right_hand_ = obj.lock().get();
		}
		else if (obj.lock()->name == "AttackSphereL")
		{
			left_hand_ = obj.lock().get();
		}
	}
}

void Boss::FixedUpdate()
{
	rigidbody_->velocity = Vector3::Scale(rigidbody_->velocity, { 0.9,1.0f,0.9f });
	rigidbody_->AddForce(move - Vector3::Scale({ 1,0,1 }, rigidbody_->velocity));
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
			owner->transform->lock()->LookAt({player_.lock()->transform->lock()->position->x,owner->transform->lock()->position->y,player_.lock()->transform->lock()->position->z});
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
			owner->transform->lock()->LookAt({ player_.lock()->transform->lock()->position->x,owner->transform->lock()->position->y,player_.lock()->transform->lock()->position->z });

			Vector3 dir
			{
				(player_.lock()->transform->lock()->position - owner->transform->lock()->position).Normalized()
			};

			move = dir* move_speed_;
			move.y = 0;

			const float ep{ 2.0f };
			Vector3 dv
			{
				player_.lock()->transform->lock()->position - owner->transform->lock()->position
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
			if (attack_num_ != 1)
			{
				return BehaviorStatus::Failure;
			}

			rigidbody_->velocity->x = 0.0f;
			rigidbody_->velocity->z = 0.0f;
			if (attack_) {
				model_data_->PlayAnimation(1,false);
				left_hand_->activeSelf = true;
				right_hand_->activeSelf = true;
				attack_ = false;
			}
			// アニメーションが終了したらフラグ切り替え
			else if (model_data_->IsPlayAnimation(1) == false)
			{
				attack_ = true;
				left_hand_->activeSelf = false;
				right_hand_->activeSelf = false;

				const float ep{ 2.0f };
				Vector3 dv
				{
					player_.lock()->transform->lock()->position - owner->transform->lock()->position
				};
				// 誤差範囲内になったら終了
				if (fabsf(dv.x) <= ep &&
					fabsf(dv.y) <= ep &&
					fabsf(dv.z) <= ep)
				{
					
					attack_num_ = 2;
					return BehaviorStatus::Sucess;
				}
				else
				{
					attack_num_ = 1;
					state_flag_ = StateFlag::Move;
					return BehaviorStatus::Sucess;

				}

			}

			return BehaviorStatus::Runnning;
		}
	);
	attack1->Name = "Action : Attack 1";

	ActionNode *attack2 = new ActionNode
	(
		[&](GameObject *owner)
		{

			if (attack_num_ != 2)
			{
				return BehaviorStatus::Failure;
			}

			rigidbody_->velocity->x = 0.0f;
			rigidbody_->velocity->z = 0.0f;
			if (attack_) {
				model_data_->PlayAnimation(2, false);
				left_hand_->activeSelf = true;
				right_hand_->activeSelf = true;
				attack_ = false;
			}
			// アニメーションが終了したらフラグ切り替え
			else if (model_data_->IsPlayAnimation(2) == false)
			{
				attack_ = true;
				left_hand_->activeSelf = false;
				right_hand_->activeSelf = false;

				const float ep{ 2.0f };
				Vector3 dv
				{
					player_.lock()->transform->lock()->position - owner->transform->lock()->position
				};
				// 誤差範囲内になったら終了
				if (fabsf(dv.x) <= ep &&
					fabsf(dv.y) <= ep &&
					fabsf(dv.z) <= ep)
				{

					attack_num_ = 3;
					return BehaviorStatus::Sucess;
				}
				else
				{
					attack_num_ = 1;
					state_flag_ = StateFlag::Move;
					return BehaviorStatus::Sucess;

				}
			}

			return BehaviorStatus::Runnning;
		}
	);
	attack2->Name = "Action : Attack 2";

	ActionNode *attack3 = new ActionNode
	(
		[&](GameObject *owner)
		{

			if (attack_num_ != 3)
			{
				return BehaviorStatus::Failure;
			}
			rigidbody_->velocity->x = 0.0f;
			rigidbody_->velocity->z = 0.0f;

			if (attack_) {
				model_data_->PlayAnimation(3, false);
				left_hand_->activeSelf = true;
				right_hand_->activeSelf = true;
				attack_ = false;
			}

			// アニメーションが終了したらフラグ切り替え
			else if (model_data_->IsPlayAnimation(3) == false)
			{

				attack_ = false;
				left_hand_->activeSelf = false;
				right_hand_->activeSelf = false;
				attack_num_ = 1;
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
