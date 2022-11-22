#include "Image.h"

#include "DirectXCommon.h"
#include "ConstantBufferManager/ConstantBufferManager.h"


Image::Image():
MonoBehaviour("Image")
{
	HRESULT result = S_FALSE;
	// 頂点バッファ生成
	result = DirectXCommon::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	mat_projection_ = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WinApp::windowWidth, (float)WinApp::windowHeight, 0.0f, 0.0f, 1.0f);


	// 定数バッファの生成
	result = DirectXCommon::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteDate) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// 定数バッファにデータ転送
	SpriteDate *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = mat_projection_;
		constBuff->Unmap(0, nullptr);
	}
	return;
}

void Image::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - pivot_.x) * width_;
	float right = (1.0f - pivot_.x) * width_;
	float top = (0.0f - pivot_.y) * height_;
	float bottom = (1.0f - pivot_.y) * height_;

	// 頂点データ
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	//if (TextureManager::texture_buffer_[texNumber]) {
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTexture(texture_name_)->resource->GetDesc();

	float tex_left = /*texLeftTop.x /*/ resDesc.Width;
	float tex_right = (/*texLeftTop.x +*/ width_) / resDesc.Width;
	float tex_top = /*texLeftTop.y /*/ resDesc.Height;
	float tex_bottom = (/*texLeftTop.y +*/ height_) / resDesc.Height;

	vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
	vertices[LT].uv = { tex_left,	tex_top }; // 左上
	vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
	vertices[RT].uv = { tex_right,	tex_top }; // 右上
	//}

	// 頂点バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}
