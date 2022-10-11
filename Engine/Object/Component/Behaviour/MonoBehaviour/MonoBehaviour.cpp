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
	// ��x�����X�^�[�g�֐����o�R����
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
	// ��x�����X�^�[�g�֐����o�R����
	if(do_start_)
	{
		Start();
		do_start_ = false;
	}
	Update();
}
