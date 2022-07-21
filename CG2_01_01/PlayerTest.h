#pragma once
#include "ScriptComponent.h"
class PlayerTest :
	public ScriptComponent
{
public:
	PlayerTest();

	void OnCollisionEnter() override;
private:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// XV
	/// </summary>
	void Update() override;
	void LustUpdate() override;

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;

	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void Finalize() override;





};

