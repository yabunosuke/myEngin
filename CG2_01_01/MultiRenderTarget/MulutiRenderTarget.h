#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>

// �T�u�N���X
enum class RenderName
{
	color,	//	0.�F���
	normal,	//	1.�@�����
	metal_roughness,		//	2.���^���b�N�E���t�l�X���
	depth,	//	3.�[�x���
	world_position,	//	4.���[���h���
	id,		//	5.���������p��ID
	MAX
};

class MulutiRenderTarget
{

private:	//�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:
	

	MulutiRenderTarget();

	void InitializeMulutiRenderTarget(ComPtr<ID3D12Device> dev);
	void PreDrawScene(ComPtr<ID3D12Device> dev,ComPtr<ID3D12GraphicsCommandList> cmd_list);
	void DrawRenderTarget(ComPtr<ID3D12GraphicsCommandList> cmd_list, ComPtr<ID3D12Device> dev);
	void PostDraw(ComPtr<ID3D12GraphicsCommandList> cmd_list);

	/// <summary>
	/// �}���`�����_�[�̐�
	/// </summary>
	static const int buffer_count_ = 6;


private:
	// �T�u�N���X
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z���W
		XMFLOAT2 uv;	//uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat;	//3D�ϊ��s��
	};




	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constant_buffer_;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertex_buffer_;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_{};
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depth_buffer_;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texture_buffer_[buffer_count_];
	// SRV�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_SRV_;
	// RTV�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
	// DSV�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	// ��ʃN���A����F
	const float clear_color_[4] = { 0.0f,0.0f,1.0f,1.0f };


};

