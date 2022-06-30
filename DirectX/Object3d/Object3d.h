#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "Model.h"
#include "ModelLoader.h"
#include "Camera.h"



class Object3d
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �萔
//�{�[���̍ő吔
	static const int MAX_BONES = 4;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���́i���W�ϊ��s��p�j
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;    // �r���[�v���W�F�N�V�����s��
		XMMATRIX world; // ���[���h�s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i�X�L�j���O�j
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	struct Animation
	{
		FbxAnimStack *animstack;
		FbxTakeInfo *takeinfo;
	};

public: // �ÓI�����o�֐�
	// �ÓI������
	static void StaticInitialize(ID3D12Device *device);
	// �ÓI�j��
	static void StaticFinalize();
	// �O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();
	// 3D�I�u�W�F�N�g����
	static std::unique_ptr<Object3d> Create(Model *model = nullptr, std::string name = "", bool isAnimation = false);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device *device;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // �����o�֐�
	//�f�X�g���N�^
	~Object3d();
	// ������
	bool Initialize();
	// ���t���[������
	void Update();
	// �`��
	void Draw(ComPtr<ID3D12GraphicsCommandList> cmdList) const;
	// �s��̍X�V
	void UpdateWorldMatrix();
	//���f����ݒ�
	void SetModel(Model *model) { this->model = model; }
	//���f���̃Q�b�^�[
	Model *GetModel() { return model; }
	//�A�j���[�V�����̃��[�h
	void LoadAnimation();
	//�A�j���[�V�����J�n
	void PlayAnimation(int animationNumber = 0, bool isLoop = true);

	// ���W�̎擾
	const XMFLOAT3 &GetPos() { return position; }
	// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	// X,Y,Z�����̎擾
	const XMFLOAT3 &GetRotation() { return rotation; }
	// X,Y,Z�����̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	// �X�P�[���̎擾
	const XMFLOAT3 &GetScale() { return scale; }
	// �X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	void SetScale(float scale) { this->scale = { scale,scale,scale }; }

	XMMATRIX GetMatWorld() { return matWorld; }


protected: // �����o�ϐ�
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	// �萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	// ���[�J���X�P�[��
	Vector3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	Vector3 rotation = { 0,0,0 };
	// ���[�J�����W
	Vector3 position = { 0,0,0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// ���f��
	Model *model = nullptr;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	//���[�v�t���O
	bool isLoop = false;
	//�A�j���[�V�����̕ۑ�
	std::vector<Animation> animationData;

	//�N���X��(�f�o�b�N�p)
	const std::string *name = nullptr;

};