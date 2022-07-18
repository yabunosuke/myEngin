#pragma once
#include "Component.h"
#include "Vector3.h"

class Player :
    public Component
{
public:
    Player();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// XV
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// •`‰æ
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// I—¹ˆ—
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// î•ñ
	/// </summary>
	void Infomation() override;

private:

	Vector3 velosity;
};

