#include "FlyEnemyEye.h"

FlyEnemyEye::FlyEnemyEye():
	MonoBehaviour("FlyEnemyEye")
{
}

void FlyEnemyEye::OnTriggerStay(Collider &other)
{
	if (other.game_object_->name.r_ == "Player")
	{
		game_object_->GetPearent()->transform_->LookAt(other.game_object_->transform_->position);
	}
}
