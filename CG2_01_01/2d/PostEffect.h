#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    //定数バッファ用データ構造体
	struct ConstBufferData {
        XMFLOAT4 color; //色(RGBA)
        XMMATRIX mat;	//3D変換行列
        float time;
    };
    PostEffect();
    void InitializePostEffect(ComPtr<ID3D12Device> dev);

    void PreDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list);
	void DrawPostEffect(ComPtr<ID3D12GraphicsCommandList> cmd_list);
    void PostDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list);


	static const float clear_color[4];

    // 震度バッファ
    ComPtr<ID3D12Resource> depth_buffer_;
    // テクスチャバッファ
    ComPtr<ID3D12Resource> texture_buffer_;
    // RTVデスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_SRV_;
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	std::string shader_name_ = "PostEffect";
};

