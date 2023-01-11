#include "EnemySearchPlayer.h"

EnemySearchPlayer::EnemySearchPlayer():
MonoBehaviour("EnemySearchPlayer")
{
}

void EnemySearchPlayer::OnTriggerExit(Collider &other)
{
	if (other.game_object_.lock()->tag == "Player")
	{
		spike_enemy_->is_target_ = false;

	}
}

void EnemySearchPlayer::OnTriggerStay(Collider &other)
{
	if(other.game_object_.lock()->tag == "Player")
	{

		spike_enemy_->is_target_ = true;
		spike_enemy_->target_position_ = other.game_object_.lock()->transform->lock()->position;
	}
}

void EnemySearchPlayer::Start()
{
	spike_enemy_ = game_object_.lock()->GetPearent().lock()->GetComponent<SpikeEnemy>().lock().get();
}
