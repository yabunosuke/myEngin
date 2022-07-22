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
	// �I�u�W�F�N�g�X�V
	game_object_manager_.Update();

	// �����蔻��`�F�b�N
	collision_manager_.CheckBroadCollisions(game_object_manager_.gameObjects);

	// �ŏI�X�V
	game_object_manager_.LastUpdate();
}

void AbstractScene::DrawPostEffect()
{
	post_effect_->DrawPostEffect(DirectXCommon::dev, DirectXCommon::cmdList);
}

