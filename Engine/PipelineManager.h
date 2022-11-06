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
	

	void CreateAllPiplines(ComPtr<ID3D12Device> dev);
	// オブジェクト用パイプライン
	void CreateGBufferPipline(ComPtr<ID3D12Device> dev);
	// スプライト用パイプライン
	void CreateSpriutePipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// スカイボックス用パイプライン
	void CreateSkyBoxPipline(ComPtr<ID3D12Device> dev);
	// ポストエフェクト用パイプライン
	void CreatePostEffectPipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// マルチレンダーターゲット用パイプライン
	void CreateMultiRenderTargetPipline(ComPtr<ID3D12Device> dev, const std::string &pipline_name);
	// ディファード用パイプライン
	void CreateDeferredPpline(ComPtr<ID3D12Device> dev);


	// コライダー用のパイプライン
	void CreatePrimitivePipeline(ComPtr<ID3D12Device> dev);

	//パイプラインセット
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
	// パイプラインコンテナ（後々シリアライズする）
	std::map<std::string,
		std::pair<ComPtr<ID3D12PipelineState>, ComPtr<ID3D12RootSignature>>> piplines_;
};

