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
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
	void LustUpdate() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;





};

