#pragma once
#include "Component/Component.h"

#include "KeyboardInput.h"
#include "TransformComponent.h"


class ScriptComponent :
	public Component
{
public:
	ScriptComponent(std::string script_name);

	/// <summary>
	/// ���������u�Ԃ�������
	/// </summary>
	virtual void OnCollisionEnter() {};
	// ���蔲�����Ƃ��p
	virtual void OnTriggerEnter() {};
	// �������Ă���Ƃ��p
	virtual void OnCollisionStay() {};



protected:

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// �X�V
	/// </summary>
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



	// �p����Œ��Ɏg���鐔�l
	Transform *transform_;		// �g�����X�t�H�[��
	KeyboardInput *keybord;		// �L�[�{�[�h����


private:

	/// <summary>
	/// ������
	/// </summary>
	void ComponentInitialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
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

