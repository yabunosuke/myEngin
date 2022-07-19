#pragma once

#include <string>
#include "imguiManager.h"
#include "IoChangedListener.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"

class AbstractScene
{
protected:
	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	virtual ~AbstractScene() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 一定間隔で更新
	/// </summary>
	//virtual void FixUpdate() = 0;
	/// <summary>
	/// 描画コマンド発行
	/// </summary>
	virtual void Draw() const = 0;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;

	//シーン名
	std::string name;


	GameObjectManager *GetObjectManager() { return &game_object_manager_; }
	CollisionManager *GetCollisionManager() { return &collision_manager_; }


protected:
	// オブジェクトマネージャー
	GameObjectManager game_object_manager_;
	// コリジョンマネージャー
	CollisionManager collision_manager_;

private:

};

