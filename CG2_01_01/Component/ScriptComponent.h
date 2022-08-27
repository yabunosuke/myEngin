#pragma once
#include "Component/Component.h"

#include "KeyboardInput.h"
#include "Transform.h"


class ScriptComponent :
	public Component
{
public:
	ScriptComponent(std::string script_name);

	/// <summary>
	/// 当たった瞬間だけ処理
	/// </summary>
	virtual void OnCollisionEnter() {};
	/// <summary>
	/// 当たった瞬間だけ処理
	/// </summary>
	virtual void OnTriggerEnter() {};
	/// <summary>
	/// 当たった瞬間だけ処理
	/// </summary>
	virtual void OnCollisionStay() {};



protected:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() {};

	/// <summary>
	/// 更新
	/// </summary>
	virtual void FixedUpdate() {};
	virtual void Update() {};
	virtual void LustUpdate() {};

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() {};



	// 継承先で直に使える数値
	KeyboardInput *keybord;		// キーボード入力


private:

	/// <summary>
	/// 初期化
	/// </summary>
	void ComponentInitialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void ComponentFixedUpdate() override;
	void ComponentUpdate() override;
	void ComponentLustUpdate() override;

	/// <summary>
	/// 描画
	/// </summary>
	void ComponentDraw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void ComponentFinalize() override;

	std::string script_name_;

};

