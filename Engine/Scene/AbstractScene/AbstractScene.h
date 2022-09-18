#pragma once
#include <string>
#include "IoChangedListener.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "2d/PostEffect.h"
#include "MultiRenderTarget/MulutiRenderTarget.h"


#include "Component/Manager/LightManager.h"
#include <wrl.h>

class AbstractScene
{
protected:

	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl,std::string sceneName);
	virtual ~AbstractScene() {};
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// ���Ԋu�ōX�V
	/// </summary>
	virtual void FixedUpdate();
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�g�`��O����
	/// </summary>
	void PreDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// �`��R�}���h���s
	/// </summary>
	virtual void Draw() const;

	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�g�`��㏈��
	/// </summary>
	void PostDrawMultiRenderTarget(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// �|�X�g�G�t�F�N�g�`��O����
	/// </summary>
	void PreDrawPostEffect(Microsoft::WRL::ComPtr<ID3D12Device>dev,Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);
	
	/// <summary>
	/// �}���`�����_�[�^�[�Q�b�g�`��
	/// </summary>
	virtual void DrawMulutiRenderTarget(Microsoft::WRL::ComPtr<ID3D12Device>dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	void PostDrawPoseEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);

	virtual void DrawPostEffect(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();


	// �Q�b�^�[
	std::weak_ptr<GameObjectManager> GetObjectManager() { return game_object_manager_; }	// �I�u�W�F�N�g�}�l�[�W���[
	//CollisionManager *GetCollisionManager() { return &collision_manager_; }			// �q���W�����}�l�[�W���[
	std::string GetName() { return name; }
	std::unique_ptr<MulutiRenderTarget> *GetMulutiRenderTarget() { return &muluti_render_target_; }

protected:
	// �I�u�W�F�N�g�}�l�[�W���[
	std::shared_ptr<GameObjectManager> game_object_manager_;
	// �R���W�����}�l�[�W���[
	//CollisionManager collision_manager_;
	// ���C�g�}�l�[�W���[
	std::shared_ptr<LightManager> light_manager_;
	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> post_effect_;
	// �}���`�����_�[�^�[�Q�b�g
	std::unique_ptr<MulutiRenderTarget> muluti_render_target_;

private:
	// �V�[����
	std::string name;

};

