#pragma once
#include "Texture.h"
#include "Math/Vector2.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"

class Image :
    public MonoBehaviour
{
public:
	//���_�f�[�^�\����
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z���W
		XMFLOAT2 uv;	//uv���W
	};


    Image();


private:
	void TransferVertices();

	Vector2 pivot_{ 0.5f,0.5f };
	float width_{100.0f};
	float height_{100.0f};
	XMFLOAT4 color{1.0f,1.0f,1.0f,1.0f};			//�F
	std::wstring texture_name_;

	XMMATRIX mat_projection_;									//�ˉe�s��
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[
};

