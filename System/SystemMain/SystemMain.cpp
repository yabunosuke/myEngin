#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//�E�B���h�E�̕\����
#include "DirectXCommon.h"	//DirectX��������
#include "Object3d.h"		//�I�u�W�F�N�g
#include "Sprite.h"
#include "ImageManager.h"
#include "AudioManager.h"
#include "ModelManager.h"			//���f��
#include "imguiManager.h"

//imgui
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

bool SystemMain::Initialize() const
{
	WinApp::WindowCreate();				//�E�B���h�E����
	DirectXCommon::Initialize();		//DirectX������

	//imgui��������
	imguiManager::GetIns()->Initialize();

	
	Object3d::StaticInitialize(DirectXCommon::dev.Get());//�I�u�W�F�N�g���ʃf�[�^����
	//Model::StaticInitialize(DirectXCommon::dev.Get());	//���f���p���I������


	Sprite::StaticInitialize(						//�X�v���C�g���ʃf�[�^����
		DirectXCommon::dev.Get(),
		WinApp::windowWidth, WinApp::windowHeight);
	ImageManager::GetIns()->LoadImagALL(DirectXCommon::dev.Get());				//�摜�ǂݍ���

	AudioManager::Initialize();
	ModelLoader::GetInstance()->Initialize(DirectXCommon::dev.Get());
	ModelManager::GetIns()->AllLoad();			//���f���ǂݍ���

	return true;
}

void SystemMain::Finalize() const
{
	AudioManager::SoundUnLoad(AudioManager::sound);		//�������J��
	WinApp::WindowClassRelease();	// �E�B���h�E�N���X��o�^����
}

void SystemMain::main() const
{
	Looper looper;
	while (DirectXCommon::ResourceBarrierWriting() && DirectXCommon::ScreenClear() && !!WinApp::WindowsMessage()) {
		//�E�B���h�E�T�C�Y�̕ύX�`�F�b�N
		if (!looper.Loop()) {
			break;
		}
	}
}
