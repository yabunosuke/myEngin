#include "ScriptComponent.h"
#include "GameObject.h"


ScriptComponent::ScriptComponent(std::string script_name) :
	Component("Script", ComponentID::Sqript),
	script_name_(script_name)
{
}

void ScriptComponent::ComponentInitialize()
{
	// トランスフォーム取得
	transform_ = parent->GetComponent<TransformComponent>()->GetTransform();
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
