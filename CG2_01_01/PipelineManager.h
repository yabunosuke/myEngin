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

public:

	void CreatePipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	
	void SetPipline(ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipline_name);

private:
	// �p�C�v���C���R���e�i�i��X�V���A���C�Y����j
	std::map<std::string,
		std::pair<ComPtr<ID3D12PipelineState>, ComPtr<ID3D12RootSignature>>> piplines_;
};

