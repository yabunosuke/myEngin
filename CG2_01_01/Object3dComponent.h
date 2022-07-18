#pragma once
#include "Component.h"
#include "Fbx.h"

class Object3dComponent :
    public Component
{
public:
	Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path);

	/// <summary>
	/// ������
	/// </summary>
	void VirtualInitialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void VirtualUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void VirtualDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void VirtualFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;

private:

	// FBX�I�u�W�F�N�g
	std::shared_ptr<Fbx> object_fbx;

	
	ComPtr<ID3D12Device> dev;					// �f�o�C�X
	ComPtr<ID3D12GraphicsCommandList> cmd_list;	// �R�}���h���X�g

};

