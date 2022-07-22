#pragma once
#include <string>
#include "IoChangedListener.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "2d/PostEffect.h"
#include <wrl.h>

class AbstractScene
{
protected:

	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	virtual ~AbstractScene() = default;
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();
	/// <summary>
	/// ���Ԋu�ōX�V
	/// </summary>
	//virtual void FixUpdate() = 0;
	/// <summary>
	/// �`��R�}���h���s
	/// </summary>
	void PreDrawScene(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list) { post_effect_->PreDrawScene(cmd_list); }
	virtual void Draw() const = 0;
	void PostDrawScene(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list) { post_effect_->PostDrawScene(cmd_list); }

	virtual void DrawPostEffect();
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;


	// �Q�b�^�[
	GameObjectManager *GetObjectManager() { return &game_object_manager_; }	// �I�u�W�F�N�g�}�l�[�W���[
	CollisionManager *GetCollisionManager() { return &collision_manager_; }			// �q���W�����}�l�[�W���[
	std::string GetName() { return name; }

protected:
	// �I�u�W�F�N�g�}�l�[�W���[
	GameObjectManager game_object_manager_;
	// �R���W�����}�l�[�W���[
	CollisionManager collision_manager_;
	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> post_effect_;

private:
	// �V�[����
	std::string name;

};

