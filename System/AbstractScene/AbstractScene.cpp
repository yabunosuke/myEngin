#include "AbstractScene.h"
#include  "DirectXCommon.h"
AbstractScene::AbstractScene(IoChangedListener *impl,std::string sceneName)
	:implSceneChanged(impl),
	name(sceneName)
{
	post_effect_ = std::make_unique<PostEffect>();
	post_effect_->InitializePostEffect(DirectXCommon::dev);
}

void AbstractScene::Update()
{
	// オブジェクト更新
	game_object_manager_.Update();

	// 当たり判定チェック
	collision_manager_.CheckBroadCollisions(game_object_manager_.gameObjects);

	// 最終更新
	game_object_manager_.LastUpdate();
}

void AbstractScene::DrawPostEffect()
{
	post_effect_->DrawPostEffect(DirectXCommon::dev, DirectXCommon::cmdList);
}

