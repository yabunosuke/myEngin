#include "FlyEnemyEye.h"

FlyEnemyEye::FlyEnemyEye():
	MonoBehaviour("FlyEnemyEye")
{
}

void FlyEnemyEye::OnTriggerStay(Collider &other)
{
	if (other.game_object_.lock()->name.r_ == "Player")
	{
		game_object_.lock()->GetPearent().lock()->transform->lock()->LookAt(other.game_object_.lock()->transform->lock()->position);
	}
}
