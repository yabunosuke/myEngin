#pragma once
#include <vector>

#include "Fbx/Fbx.h"

class Renderer
{

private:	//シングルトンパターン
	Renderer() = default;
	~Renderer() = default;
	Renderer(const Renderer & r) = default;
	Renderer &operator= (const Renderer & r) = default;

public:		//インスタンスの取得
	static Renderer *GetIns();

	void SetDrawResource(std::weak_ptr<FbxResource> resorces , std::vector<Fbx::Node> nodes);
	void DrawDeferred(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);
	void DrawForward(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);


private:
	
	static std::vector<std::shared_ptr<FbxResource>>	deferred_shading_datas_;
	static std::vector<std::shared_ptr<FbxResource>>	forward_shading_datas_;
	static std::vector<std::vector<Fbx::Node>>			nodes_;
};

