#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>




class PrimitiveRenderer
{
private:	//�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

	// �V���O���g���p�^�[��
	PrimitiveRenderer() = default;
	~PrimitiveRenderer() = default;
	PrimitiveRenderer(const PrimitiveRenderer & r) = default;
	PrimitiveRenderer &operator= (const PrimitiveRenderer & r) = default;

public:
	struct ConstantBuffer
	{
		XMFLOAT4X4 world;	// ���[���h�s��
		XMFLOAT4 color;		// �J���[
	};

	struct Line
	{
		DirectX::XMFLOAT3 start_positon;
		DirectX::XMFLOAT3 end_positon;
	};
	struct Box
	{
		DirectX::XMFLOAT3 positon;

	};

	enum class PrimitiveType
	{
		LINE,
		BOX,
		SPHERE,
		CAPSULE,

		MAX,

	};

	static PrimitiveRenderer &GetInstance();

	// �S�v���~�e�B�u����
	void Initialize(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list);


	void DrawLine(ComPtr<ID3D12GraphicsCommandList> cmd_list,Line line);
	// ���C���v���~�e�B�u����
private:
	void CreateLine(ComPtr<ID3D12Device> dev);


	// �R�}���h���X�g

	ComPtr<ID3D12Resource> vertex_buffer_[static_cast<int>(PrimitiveType::MAX)];			// ���_�o�b�t�@
	ComPtr<ID3D12Resource> constant_buffer_;		// �萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView[static_cast<int>(PrimitiveType::MAX)] = {};			// ���_�r���[
};

