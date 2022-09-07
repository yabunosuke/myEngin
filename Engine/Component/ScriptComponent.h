#pragma once
#include "Object/Component/Component.h"

#include "Input.h"

#include "Object/Component/Transform.h"

class ScriptComponent :
	public Component
{
public:
	ScriptComponent(std::string script_name);

	/// <summary>
	/// “–‚½‚Á‚½uŠÔ‚¾‚¯ˆ—
	/// </summary>
	virtual void OnCollisionEnter() {};
	/// <summary>
	/// “–‚½‚Á‚½uŠÔ‚¾‚¯ˆ—
	/// </summary>
	virtual void OnTriggerEnter() {};
	/// <summary>
	/// “–‚½‚Á‚½uŠÔ‚¾‚¯ˆ—
	/// </summary>
	virtual void OnCollisionStay() {};



protected:

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// XV
	/// </summary>
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void LustUpdate() {};

	/// <summary>
	/// •`‰æ
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	virtual void Finalize() {};

	
private:

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void ComponentInitialize() override;

	/// <summary>
	/// XV
	/// </summary>
	void ComponentFixedUpdate() override;
	void ComponentUpdate() override;
	void ComponentLustUpdate() override;

	/// <summary>
	/// •`‰æ
	/// </summary>
	void ComponentDraw() override;

	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void ComponentFinalize() override;

	std::string script_name_;

};

