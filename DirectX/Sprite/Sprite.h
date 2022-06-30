#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")

#include "Vector3.h"


class Sprite
{
private:	//�L�q�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:		//�T�u���X
	//���_�f�[�^�\����
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z���W
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat;	//3D�ϊ��s��
	};


public:		//�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInitialize(ID3D12Device *dev, int window_width, int window_height);	//�X�v���C�g�p�p�C�v���C������	
	//�e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev);
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList *cmdList);
	//�`��㏈��
	static void PostDraw();
	//�X�v���C�g����
	static Sprite *Create(UINT texNumber,XMFLOAT2 anchorpoint = { 0.5f,0.5f },bool isFlipX = false, bool isFlipY = false);

public:		//�����o�֐�
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	//������
	bool Initialize();
	//��]
	void SetRotation(float rotation);
	//���W
	XMFLOAT2 GetPosition() { return position; }
	void SetPosition(XMFLOAT2 position);

	/// <summary>
	/// �F�ύX
	/// </summary>
	/// <param name="color"> {R,G,B,A}</param>
	void SetColor(XMFLOAT4 color);
	/// <summary>
	/// �F�ύX
	/// </summary>
	/// <param name="r">Red</param>
	/// <param name="g">Green</param>
	/// <param name="b">Blue</param>
	/// <param name="a">Alpha</param>
	void SetColor(float r = 1, float g = 1, float b = 1, float a = 1);
	/// <summary>
	/// �F�ύX
	/// </summary>
	/// <param name="color">�J���[�z��</param>
	void SetColor(float color[4]);


	//�A���J�[�|�C���g�ύX
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	//���E���]
	void SetIsFlipX(bool isFlipX);
	//�㉺���]
	void SetIsFlipY(bool isFlipY);
	//�e�N�X�`���͈͎w��
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	//�`��
	void Draw();


	//�C���\��
	//�c���̃T�C�Y
	void SetSize(XMFLOAT2 size);
	//�{��
	void SetScale(float scale);

private:	//�ÓI�����o�ϐ�
	static const int SRVCount = 512;				//�e�N�X�`���̍ő吔
	static ID3D12Device *dev;					//�f�o�C�X
	static ID3D12GraphicsCommandList *cmdList;		//�R�}���h���X�g
	static ComPtr<ID3D12RootSignature> rootSignature;	//���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState;	//�p�C�v���C���X�e�[�g
	static XMMATRIX matProjection;						//�ˉe�s��
	static ComPtr<ID3D12DescriptorHeap> descHeap;		//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12Resource> texBuff[SRVCount];	//�e�N�X�`���o�b�t�@


private:
	ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[
	UINT texNumber = 0;						//�e�N�X�`���i���o�[
	float rotation = 0.0f;					//z���̉�]�p
	XMFLOAT2 position;		//���W
	XMFLOAT2 size;			//�傫��
	XMFLOAT2 anchorpoint;	//�A���J�[�|�C���g
	XMMATRIX matWorld;						//���[���h�s��
	XMFLOAT4 color;			//�F
	bool isFlipX;					//���E���]
	bool isFlipY;					//�㉺���]
	XMFLOAT2 texLeftTop = { 0,0 };			//�e�N�X�`��������W
	XMFLOAT2 texSize = { 100,100 };		//�؂���͈�
	bool isInvisible = false;

private:
	void TransferVertices();
};