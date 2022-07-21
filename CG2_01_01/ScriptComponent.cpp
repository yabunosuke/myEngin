#include "ScriptComponent.h"
#include "GameObject.h"


ScriptComponent::ScriptComponent(std::string script_name) :
	Component("Script", true),
	script_name_(script_name)
{
	isDontRemove = false;
}

void ScriptComponent::ComponentInitialize()
{
	// �g�����X�t�H�[���擾
	transform_ = parent->GetComponent<TransformComponent>()->GetTransform();
	// �L�[�{�[�h�擾
	keybord = KeyboardInput::GetIns();


	Initialize();
}

void ScriptComponent::ComponentUpdate()
{
	Update();
}

void ScriptComponent::ComponentLustUpdate()
{
}

void ScriptComponent::ComponentDraw()
{
}

void ScriptComponent::ComponentFinalize()
{
}
