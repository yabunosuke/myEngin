#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")

#include "Vector3.h"


class Sprite
{
protected:	//記述省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:		//サブラス
	//頂点データ構造体
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z座標
		XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color; //色(RGBA)
		XMMATRIX mat;	//3D変換行列
	};


public:		//静的メンバ関数
	//静的初期化
	static bool StaticInitialize(ID3D12Device *dev, int window_width, int window_height);	//スプライト用パイプライン生成	
	//テクスチャ読み込み
	//static bool LoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev);

	//スプライト生成
	static Sprite *Create(ComPtr<ID3D12Device> dev,int texture_num,XMFLOAT2 anchorpoint = { 0.5f,0.5f },bool isFlipX = false, bool isFlipY = false);

public:		//メンバ関数
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	//初期化
	bool Initialize(ComPtr<ID3D12Device> dev);
	//回転
	void SetRotation(float rotation);
	//座標
	XMFLOAT2 GetPosition() { return position; }
	void SetPosition(XMFLOAT2 position);

	/// <summary>
	/// 色変更
	/// </summary>
	/// <param name="color"> {R,G,B,A}</param>
	void SetColor(XMFLOAT4 color);
	/// <summary>
	/// 色変更
	/// </summary>
	/// <param name="r">Red</param>
	/// <param name="g">Green</param>
	/// <param name="b">Blue</param>
	/// <param name="a">Alpha</param>
	void SetColor(float r = 1, float g = 1, float b = 1, float a = 1);
	/// <summary>
	/// 色変更
	/// </summary>
	/// <param name="color">カラー配列</param>
	void SetColor(float color[4]);


	//アンカーポイント変更
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	//左右反転
	void SetIsFlipX(bool isFlipX);
	//上下反転
	void SetIsFlipY(bool isFlipY);
	//テクスチャ範囲指定
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	//描画
	void Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list, std::string pipeline_name);


	//修正予定
	//縦横のサイズ
	void SetSize(XMFLOAT2 size);
	//倍率
	void SetScale(float scale);

protected:
	void TransferVertices();
protected:	//静的メンバ変数
	static XMMATRIX matProjection;						//射影行列


protected:
	ComPtr<ID3D12Resource> vertBuff;		//頂点バッファ
	ComPtr<ID3D12Resource> constBuff;		//定数バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//頂点バッファビュー
	UINT texNumber = 0;						//テクスチャナンバー
	float rotation = 0.0f;					//z軸の回転角
	XMFLOAT2 position;		//座標
	XMFLOAT2 size;			//大きさ
	XMFLOAT2 anchorpoint;	//アンカーポイント
	XMMATRIX matWorld;						//ワールド行列
	XMFLOAT4 color;			//色
	bool isFlipX;					//左右反転
	bool isFlipY;					//上下反転
	XMFLOAT2 texLeftTop = { 0,0 };			//テクスチャ左上座標
	XMFLOAT2 texSize = { 100,100 };		//切り取り範囲
	bool isInvisible = false;

};