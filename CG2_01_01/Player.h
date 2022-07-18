#pragma once
#include "Component.h"
#include "Vector3.h"

class Player :
    public Component
{
public:
    Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// 描画
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// 終了処理
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// 情報
	/// </summary>
	void Infomation() override;

private:

	Vector3 velosity;
};

