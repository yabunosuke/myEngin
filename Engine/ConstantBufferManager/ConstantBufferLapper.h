#pragma once
#include <wrl.h>
#include <d3d12.h>

class ConstantBufferLapper
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// �o�b�t�@�̐���
	bool CreateConstantBuffer(size_t size, ComPtr<ID3D12Device> dev);
	// �Q�b�b�^�[
	ID3D12Resource *GetConstantBuffer() const { return constant_buffer_.Get(); }

private:
	ComPtr<ID3D12Resource> constant_buffer_;

};

