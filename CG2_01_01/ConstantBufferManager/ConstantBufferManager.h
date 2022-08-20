#pragma once
#include "ConstantBufferManager/ConstantBufferLapper.h"
#include "ConstantBufferManager/ConstantBuffer.h"
#include <wrl.h>
#include <vector>

enum class BufferName
{
	Test,
	Light,
	Camera,
	MAX,
};

class ConstantBufferManager
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// �V���O���g���p�^�[������
	ConstantBufferManager(ComPtr<ID3D12Device> dev);

	// �C���X�^���X�̎擾
	static ConstantBufferManager *GetInstance() { return instance; };

	// �o�b�t�@�Ƀf�[�^�]��
	template<class T>
	bool BufferTransfer(
		ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index, 
		BufferName buffername,
		T *buffer_data)
	{
		// �]��
		HRESULT result = S_OK;
		T *constMap = nullptr;
		result = constant_buffers_[static_cast<int>(buffername)].
			GetConstantBuffer()->Map(subresource, nullptr, (void **)&constMap);
		if (SUCCEEDED(result)) {
			memcpy(constMap, buffer_data, sizeof(*constMap));
			constant_buffers_[static_cast<int>(buffername)].
				GetConstantBuffer()->Unmap(subresource, nullptr);
		}
		// �Z�b�g
		cmd_list->SetGraphicsRootConstantBufferView(rootparameta_index, constant_buffers_[static_cast<int>(buffername)].GetConstantBuffer()->GetGPUVirtualAddress());


		return true;
	}


	
private:

	// �R���X�^���g�o�b�t�@�̐����ƒǉ�
	bool AddConstantBuffer(size_t size, BufferName buffername, ComPtr<ID3D12Device> dev);
	// �R���X�^���g�o�b�t�@�[�R���e�i
	std::vector<ConstantBufferLapper> constant_buffers_;
	// �����̃|�C���^
	static ConstantBufferManager *instance;
};

