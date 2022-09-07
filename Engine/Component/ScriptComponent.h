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
	/// ���������u�Ԃ�������
	/// </summary>
	virtual void OnCollisionEnter() {};
	/// <summary>
	/// ���������u�Ԃ�������
	/// </summary>
	virtual void OnTriggerEnter() {};
	/// <summary>
	/// ���������u�Ԃ�������
	/// </summary>
	virtual void OnCollisionStay() {};



protected:

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void LustUpdate() {};

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() {};

	
private:

	/// <summary>
	/// ������
	/// </summary>
	void ComponentInitialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void ComponentFixedUpdate() override;
	void ComponentUpdate() override;
	void ComponentLustUpdate() override;

	/// <summary>
	/// �`��
	/// </summary>
	void ComponentDraw() override;

	/// <summary>
	/// �I������
	/// </summary>
	void ComponentFinalize() override;

	std::string script_name_;

};

