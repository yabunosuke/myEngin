#include "ScriptComponent.h"
#include "Object/GameObject/GameObject.h"


ScriptComponent::ScriptComponent(std::string script_name) :
	Component("Script", ComponentType::Sqript),
	script_name_(script_name)
{
}

void ScriptComponent::ComponentInitialize()
{
	// キーボード取得
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
