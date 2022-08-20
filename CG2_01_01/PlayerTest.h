#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Transform.h"

#include "3d/Fbx/Fbx.h"

class PlayerTest :
	public ScriptComponent
{
public:
	PlayerTest();
	
	void OnCollisionEnter() override;


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


	/// <summary>
	/// ImGui用
	/// </summary>
	void Infomation() override;

private:

	// 移動速度
	float flont_move_speed	= 1.2f;
	float side_move_speed	= 1.0f;
	float back_move_speed	= 0.8f;

	// 座標
	Transform *transform_;



	// アニメーションステート
	enum class AnimationState {
		NONE = -1,
		DETH,
		IDLE,
		IDLE_LIFRE,
		PULL_RIFLE,
		PUT_RIFLE,
		WALK_BACK,
		RIFLE_WALK_BACK,
		WALK,
		RIFLE_WALK,
		WALK_RIGHT,
		WALK_LEFT,
		RIFLE_WALK_LEFT,
		RIFLE_WALK_RIGHT,
		MAX
	};
	AnimationState state = AnimationState::IDLE;
	bool isRifle{ false };
	bool isDead{ false };

	Fbx *game_object{ nullptr };
};

