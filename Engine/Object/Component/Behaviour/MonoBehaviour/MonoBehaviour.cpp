#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour(const std::string &name):
	Behaviour(name, ComponentType::MonoBehaviour)
{
}

void MonoBehaviour::ComponentInitialize()
{
	game_object_->AddMonoBehaviour(this);
}

void MonoBehaviour::ComponentFixedUpdate()
{
	if (!enabled) return;
	// 一度だけスタート関数を経由する
	if (do_start_)
	{
		Start();
		do_start_ = false;
	}
	FixedUpdate();
}

void MonoBehaviour::ComponentUpdate()
{
	if (!enabled) return;
	// 一度だけスタート関数を経由する
	if(do_start_)
	{
		Start();
		do_start_ = false;
	}
	Update();
}
