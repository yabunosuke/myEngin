#pragma once
#include <vector>

#include "Fbx/Fbx.h"

class DrawFbx
{

private:	//シングルトンパターン
	DrawFbx() = default;
	~DrawFbx() = default;
	DrawFbx(const DrawFbx & r) = default;
	DrawFbx &operator= (const DrawFbx & r) = default;

public:		//インスタンスの取得
	static DrawFbx *GetIns();

	void SetDrawResource(std::weak_ptr<FbxResource> resorces , std::vector<Fbx::Node> nodes);
	void AllDraw(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list);


private:
	
	static std::vector<std::shared_ptr<FbxResource>>	resouces_;
	static std::vector<std::vector<Fbx::Node>>			nodes_;
};

