#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <map>
#include <string>
class PipelineManager
{
private:	// �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:	// �V���O���g��
	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager &r) = default;
	PipelineManager &operator= (const PipelineManager &r) = default;

public:
	static PipelineManager *GetInstance();		//�擾�p
	

	void CreateAllPiplines(ComPtr<ID3D12Device> dev);
	// �I�u�W�F�N�g�p�p�C�v���C��
	void CreateGBufferPipline(ComPtr<ID3D12Device> dev);
	// �X�v���C�g�p�p�C�v���C��
	void CreateSpriutePipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// �X�J�C�{�b�N�X�p�p�C�v���C��
	void CreateSkyBoxPipline(ComPtr<ID3D12Device> dev);
	// �|�X�g�G�t�F�N�g�p�p�C�v���C��
	void CreatePostEffectPipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// �}���`�����_�[�^�[�Q�b�g�p�p�C�v���C��
	void CreateMultiRenderTargetPipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// �f�B�t�@�[�h�p�p�C�v���C��
	void CreateDeferredPpline(ComPtr<ID3D12Device> dev);


	// �R���C�_�[�p�̃p�C�v���C��
	void CreatePrimitivePipeline(ComPtr<ID3D12Device> dev);

	//�p�C�v���C���Z�b�g
	void SetPipline(ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipline_name);


	const std::string posteffect_shader_list_[9] = {
		"PostEffect",
		"Blur",
		"GrayScale",
		"Sepia",
		"UvScroll",
		"Tiling",
		"ScanLine",
		"RGBShift",
		"InverseColor"
	};

private:
	// �p�C�v���C���R���e�i�i��X�V���A���C�Y����j
	std::map<std::string,
		std::pair<ComPtr<ID3D12PipelineState>, ComPtr<ID3D12RootSignature>>> piplines_;
};

