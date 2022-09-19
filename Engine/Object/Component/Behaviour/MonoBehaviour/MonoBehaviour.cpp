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
	// ��x�����X�^�[�g�֐����o�R����
	if(do_start_)
	{
		Start();
		do_start_ = false;
	}
	Update();
}
