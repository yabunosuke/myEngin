#include "MonoBehaviour.h"

MonoBehaviour::MonoBehaviour(const std::string &name):
	Behaviour(name, ComponentType::MonoBehaviour)
{
}

void MonoBehaviour::ComponentFixedUpdate()
{
	FixedUpdate();
}

void MonoBehaviour::ComponentUpdate()
{
	// ��x�����X�^�[�g�֐����o�R����
	if(do_start_)
	{
		Start();
		do_start_ = false;
	}
	Update();
}
