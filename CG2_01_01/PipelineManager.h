#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <map>
#include <string>
class PipelineManager
{
private:	// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:	// シングルトン
	PipelineManager() = default;
	~PipelineManager() = default;
	PipelineManager(const PipelineManager &r) = default;
	PipelineManager &operator= (const PipelineManager &r) = default;

public:
	static PipelineManager *GetInstance();		//取得用

public:

	void CreatePipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	
	void SetPipline(ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipline_name);

private:
	// パイプラインコンテナ（後々シリアライズする）
	std::map<std::string,
		std::pair<ComPtr<ID3D12PipelineState>, ComPtr<ID3D12RootSignature>>> piplines_;
};

