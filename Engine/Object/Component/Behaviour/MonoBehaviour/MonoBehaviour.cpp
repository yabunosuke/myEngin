#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour(const std::string &name):
	Behaviour(name, ComponentType::MonoBehaviour)
{
}

void MonoBehaviour::ComponentFixedUpdate()
{
	if (!enabled) return;
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
