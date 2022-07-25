#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//ウィンドウの表示等
#include "DirectXCommon.h"	//DirectX初期化等
//#include "Object3d.h"		//オブジェクト
#include "2d/Sprite.h"
#include "AudioManager.h"
#include "ModelManager.h"			//モデル
#include "ImGui/imguiManager.h"
#include "Texture.h"
#include  "PrimitiveRenderer.h"
#include "PipelineManager.h"


bool SystemMain::Initialize() const
{
	WinApp::WindowCreate();				//ウィンドウ生成
	DirectXCommon::Initialize();		//DirectX初期化

	//imguiを初期化
	imguiManager::GetIns()->Initialize();

	// テクスチャデスクリプタヒープ生成
	Texture::CreateDescriptorHeap(DirectXCommon::dev.Get());
	
	//Object3d::StaticInitialize(DirectXCommon::dev.Get());//オブジェクト共通データ生成
	//Model::StaticInitialize(DirectXCommon::dev.Get());	//モデル姿勢的初期化

	PipelineManager::GetInstance()->CreateAllPiplines(DirectXCommon::dev);
	PrimitiveRenderer::GetInstance().CreatePrimitivAll(DirectXCommon::dev,DirectXCommon::cmdList);

	Sprite::StaticInitialize(						//スプライト共通データ生成
		DirectXCommon::dev.Get(),
		WinApp::windowWidth, WinApp::windowHeight);

	AudioManager::Initialize();
	//ModelLoader::GetInstance()->CreatePrimitivAll(DirectXCommon::dev.Get());
	ModelManager::GetIns()->AllLoad();			//モデル読み込み

	return true;
}

void SystemMain::Finalize() const
{
	AudioManager::SoundUnLoad(AudioManager::sound);		//音声を開放
	WinApp::WindowClassRelease();	// ウィンドウクラスを登録解除
}

void SystemMain::main() const
{
	Looper looper;
	while ( !!WinApp::WindowsMessage()) {
		//ウィンドウサイズの変更チェック
		if (!looper.Loop()) {
			break;
		}
	}
}
