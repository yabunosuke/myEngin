#pragma once
#include "Component.h"
#include "Vector3.h"
#include "TransformComponent.h"
#include "Fbx.h"

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

	void VirtualLustUpdate() override;
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
	TransformComponent::Transform transform_;
	void Move();

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
	bool isRifle = false;
	bool isDead = false;

	Fbx *game_object;
};

