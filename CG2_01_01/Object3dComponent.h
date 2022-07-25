#pragma once
#include "Component/Component.h"
#include "3d/Fbx/Fbx.h"

class Object3dComponent :
    public Component
{
public:
	Object3dComponent(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, const char *file_path);

	/// <summary>
	/// ������
	/// </summary>
	void ComponentInitialize() override;
	/// <summary>
	/// �X�V
	/// </summary>
	void ComponentUpdate() override;
	/// <summary>
	/// �`��
	/// </summary>
	void ComponentDraw() override;
	/// <summary>
	/// �I������
	/// </summary>
	void ComponentFinalize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Infomation() override;

	// �I�u�W�F�N�g�̎擾
	Fbx *GetObjectData() const { return object_fbx.get(); }
private:

	// FBX�I�u�W�F�N�g
	std::shared_ptr<Fbx> object_fbx;

	
	ComPtr<ID3D12Device> dev;					// �f�o�C�X
	ComPtr<ID3D12GraphicsCommandList> cmd_list;	// �R�}���h���X�g

};

