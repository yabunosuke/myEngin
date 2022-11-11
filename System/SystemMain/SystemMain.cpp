#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//�E�B���h�E�̕\����
#include "DirectXCommon.h"	//DirectX��������
//#include "Object3d.h"		//�I�u�W�F�N�g
#include "2d/Sprite.h"
#include "AudioManager.h"
#include "ModelManager.h"			//���f��
#include "ImGui/imguiManager.h"
#include "Texture.h"
#include  "PrimitiveRenderer.h"
#include "PipelineManager.h"

#include "Time/Time.h"

#include "../Engine/Object/Object.h"


bool SystemMain::Initialize()
{
	WinApp::WindowCreate();				//�E�B���h�E����
	DirectXCommon::Initialize();		//DirectX������

	//imgui��������
	imguiManager::GetIns()->Initialize();

	// �e�N�X�`���f�X�N���v�^�q�[�v����
	TextureManager::GetInstance()->CreateDescriptorHeap(DirectXCommon::dev.Get());
	// �p���m�}�X�J�C�{�b�N�X���[�h
	TextureManager::LoadTexture(DirectXCommon::dev.Get(), L"Assets/2d/Panorama-V01.png");

	//Object3d::StaticInitialize(DirectXCommon::dev.Get());//�I�u�W�F�N�g���ʃf�[�^����
	//Model::StaticInitialize(DirectXCommon::dev.Get());	//���f���p���I������

	PipelineManager::GetInstance()->CreateAllPiplines(DirectXCommon::dev);
	PrimitiveRenderer::GetInstance().CreatePrimitivAll(DirectXCommon::dev,DirectXCommon::cmdList);

	Sprite::StaticInitialize(						//�X�v���C�g���ʃf�[�^����
		DirectXCommon::dev.Get(),
		WinApp::windowWidth, WinApp::windowHeight);

	AudioManager::Initialize();
	//ModelLoader::GetInstance()->CreatePrimitivAll(DirectXCommon::dev.Get());
	//ModelManager::GetIns()->AllLoad();			//���f���ǂݍ���

	// �o�b�t�@�}�l�[�W������
	constant_buffer_manager_ = new ConstantBufferManager(DirectXCommon::dev);

	// Time�N���X�̃V���O���g������
	Time::Create();

	return true;
}

void SystemMain::Finalize() const
{
	// Time�N���X�̃V���O���g�����
	Time::Destroy();
	AudioManager::SoundUnLoad(AudioManager::sound);		//�������J��
	WinApp::WindowClassRelease();	// �E�B���h�E�N���X��o�^����
}

void SystemMain::main() const
{
	Looper looper;
	while ( !!WinApp::WindowsMessage()) {
		//�E�B���h�E�T�C�Y�̕ύX�`�F�b�N
		if (!looper.Loop()) {
			break;
		}
	}
}
