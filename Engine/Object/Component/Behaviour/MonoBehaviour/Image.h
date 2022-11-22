#pragma once
#include "Texture.h"
#include "Math/Vector2.h"
#include "Object/Component/Behaviour/MonoBehaviour.h"

class Image :
    public MonoBehaviour
{
public:
	//頂点データ構造体
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z座標
		XMFLOAT2 uv;	//uv座標
	};


    Image();


private:
	void TransferVertices();

	Vector2 pivot_{ 0.5f,0.5f };
	float width_{100.0f};
	float height_{100.0f};
	XMFLOAT4 color{1.0f,1.0f,1.0f,1.0f};			//色
	std::wstring texture_name_;

	XMMATRIX mat_projection_;									//射影行列
    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;		//頂点バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;		//定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//頂点バッファビュー
};

