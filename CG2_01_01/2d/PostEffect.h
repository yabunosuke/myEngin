#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    //�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
        XMFLOAT4 color; //�F(RGBA)
        XMMATRIX mat;	//3D�ϊ��s��
        float time;
    };
    PostEffect();
    void InitializePostEffect(ComPtr<ID3D12Device> dev);

    void PreDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list);
	void DrawPostEffect(ComPtr<ID3D12GraphicsCommandList> cmd_list);
    void PostDrawScene(ComPtr<ID3D12GraphicsCommandList> cmd_list);


	static const float clear_color[4];

    // �k�x�o�b�t�@
    ComPtr<ID3D12Resource> depth_buffer_;
    // �e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texture_buffer_;
    // RTV�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_SRV_;
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_RTV_;
    ComPtr<ID3D12DescriptorHeap> descriputor_heap_DSV_;
	std::string shader_name_ = "PostEffect";
};

