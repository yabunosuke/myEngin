#pragma once

#include <d3d12.h>
#include <d3dx12.h>
#include <memory>
#include "FbxResource.h"

class Fbx
{
private:	//�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:

	// �m�[�h
	struct Node	{
		const char *name;
		Node *parent;
		DirectX::XMFLOAT3	scale;				// �g�嗦
		DirectX::XMFLOAT4	rotate;				// ��]
		DirectX::XMFLOAT3	translate;			// �ړ�
		DirectX::XMFLOAT4X4	local_transform;	// ���[�J��
		DirectX::XMFLOAT4X4	world_transform;	// ���[���h

		std::vector<Node *>	children;			// �q�̃m�[�h
	};


public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="file_path">�g���������f���̃p�X</param>
	Fbx(ID3D12Device *dev, const char *file_path);

	/// <summary>
	/// �g�����X�t�H�[���X�V
	/// </summary>
	/// <param name="transform"></param>
	void UpdateTransform(const XMFLOAT4X4 &transform);

	/// <summary>
	/// �`��R�}���h���s
	/// </summary>
	/// <param name="dev"></param>
	void Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// �m�[�h�̌���
	/// </summary>
	/// <param name="name">�����������m�[�h��</param>
	/// <returns>�m�[�h</returns>
	Node *FindNode(const char *name);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const FbxResource *GetResource() const { return resource_.get(); }

	void UpdateAnimation(float elapsed_time = 0.016f);
	void PlayAnimation(uint64_t index, bool loop = true, bool reset = false, float blend_seconds = 1.0f);
	float GetCurrentAnimationSeconds() const { return current_animation_seconds_; }

	// �A�j���[�V������������
	bool IsPlayAnimation() const;

	// �F��ݒ�
	void SetColor(XMFLOAT4 color) { color_ = color; }

private:
	// ���\�[�X
	std::shared_ptr<FbxResource>	resource_;	// ���f���f�[�^
	std::vector<Node>				nodes_;		// �R�s�[�����m�[�h

	// �A�j���[�V�����Ǘ�
	int		current_animation_index_ = -1;		// �A�j���[�V�����ԍ�
	float	current_animation_seconds_ = 0.0f;	// ���ݎ���
	bool	animation_loop_flag_ = false;		// ���[�v�t���O
	bool	animation_end_flag_ = false;		// �A�j���[�V�����I���t���O
	float	animation_blend_time_ = 0.0f;		// �⊮�ɂ�����b��
	float	animation_blend_seconds_ = 0.0f;	// �⊮�̌��ݎ���


	XMFLOAT4 color_ = { 1,1,1,1 };	//�J���[
	
};

