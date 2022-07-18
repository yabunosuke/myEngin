#pragma once
#include "Component.h"
#include "Vector3.h"

class Player :
    public Component
{
public:
    Player();

	/// <summary>
	/// ������
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;

private:

	Vector3 velosity;
};

