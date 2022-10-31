#include "Sprite.h"
#include "WinApp.h"
#include <DirectXTex.h>
#include "PipelineManager.h"
#include "oldTexture.h"
using namespace Microsoft::WRL;

//静的メンバの実態
XMMATRIX Sprite::matProjection;						//射影行列

bool Sprite::StaticInitialize(ID3D12Device *dev,int window_width, int window_height)
{
	//射影変換行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	return true;
}


Sprite *Sprite::Create(ComPtr<ID3D12Device> dev,int texuer_num,XMFLOAT2 anchorpoint, bool isFlipX,bool isFlipY)
{
	HRESULT result = S_FALSE;

	// 仮サイズ
	XMFLOAT2 size = { 100.0f, 100.0f };
	
	//指定番号の画像が読み込み済みなら
	//if (TextureManager::texture_buffer_[texuer_num]) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResoureDesc(texuer_num);
		//スプライトの大きさを画像の解像度に合わせる
		size = { (float)resDesc.Width,(float)resDesc.Height };
	//}
	
	//新しいスプライトを作る
	Sprite *sprite = new Sprite(texuer_num, {0,0}, size, { 1, 1, 1, 1 }, anchorpoint, isFlipX, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize(dev)) {
		
		
		sprite;
		assert(0);
		return nullptr;
	}

	return sprite;
}

Sprite::Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY) {
	this->position = position;
	this->size = size;
	this->anchorpoint = anchorpoint;
	this->matWorld = XMMatrixIdentity();
	this->color = color;
	this->texNumber = texNumber;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;
	this->texSize = size;
	this->isInvisible = false;
}

bool Sprite::Initialize(ComPtr<ID3D12Device> dev)
{
	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}
	return true;
}

void Sprite::SetRotation(float rotation) {
	this->rotation = rotation;

}

void Sprite::SetPosition(XMFLOAT2 position)
{
	this->position = position;

}

void Sprite::SetSize(XMFLOAT2 size)
{
	this->size = size;

}

void Sprite::SetScale(float scale)
{
	size.x *= scale;
	size.y *= scale;

}

void Sprite::SetColor(XMFLOAT4 color)
{
	this->color = color;
}

void Sprite::SetColor(float r, float g, float b, float a)
{
	this->color = { r, g, b, a };
}

void Sprite::SetColor(float color[4])
{
	this->color = { color[0],color[1],color[2],color[3] };
}

void Sprite::SetAnchorPoint(XMFLOAT2 anchorpoint) {
	this->anchorpoint = anchorpoint;

}

void Sprite::SetIsFlipX(bool isFlipX) {
	this->isFlipX = isFlipX;

}

void Sprite::SetIsFlipY(bool isFlipY) {
	this->isFlipY = isFlipY;

}

void Sprite::SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize) {
	this->texLeftTop = texBase;
	this->texSize = texSize;

}

void Sprite::Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipeline_name)
{


	//頂点バッファへのデータ転送
	TransferVertices();
	//透明なら描画しない
	if (isInvisible) {
		return;
	}
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData *constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void **)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// 行列の合成	
		this->constBuff->Unmap(0, nullptr);
	}

	// パイプラインセット
	PipelineManager::GetInstance()->SetPipline(cmd_list, pipeline_name);
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファの設定
	cmd_list->IASetVertexBuffers(0, 1, &this->vbView);


	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(cmd_list.Get(), 1, texNumber);
	// 定数バッファビューをセット
	cmd_list->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmd_list->DrawInstanced(4, 1, 0, 0);
}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX) {// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY) {// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[4];
	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	//if (TextureManager::texture_buffer_[texNumber]) {
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetResoureDesc(texNumber);

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

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
