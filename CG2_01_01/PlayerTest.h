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
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	void LustUpdate() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;





};

