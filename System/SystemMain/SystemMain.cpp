#include "SystemMain.h"
#include "Looper.h"

#include "WinApp.h"			//ウィンドウの表示等
#include "DirectXCommon.h"	//DirectX初期化等
#include "2d/Sprite.h"
#include "AudioManager.h"
#include "ModelManager.h"			//モデル
#include "ImGui/imguiManager.h"
#include "Texture.h"
#include  "PrimitiveRenderer.h"
#include "PipelineManager.h"


#include "Time/Time.h"
#include "D3D12Base/D3D12Base.h"



bool SystemMain::Initialize()
{
	WinApp::WindowCreate();				//ウィンドウ生成
	DirectXCommon::Initialize();		//DirectX初期化
	
	//// テスト
	//D3D12Base d3d12base;
	//d3d12base.Initialize(WinApp::GetHwnd());
	//// テスト



	//imguiを初期化
	imguiManager::GetIns()->Initialize();

	// テクスチャデスクリプタヒープ生成
	TextureManager::GetInstance()->CreateDescriptorHeap(DirectXCommon::dev.Get());
	// パラノマスカイボックスロード
	TextureManager::LoadTexture(DirectXCommon::dev.Get(), L"Assets/2d/Panorama-V01.png");

	PipelineManager::GetInstance()->CreateAllPiplines(DirectXCommon::dev);
	PrimitiveRenderer::GetInstance().CreatePrimitivAll(DirectXCommon::dev,DirectXCommon::cmdList);

	Sprite::StaticInitialize(						//スプライト共通データ生成
		DirectXCommon::dev.Get(),
		WinApp::windowWidth, WinApp::windowHeight);

	AudioManager::Initialize();
	// バッファマネージャ生成
	constant_buffer_manager_ = new ConstantBufferManager(DirectXCommon::dev);

	// Timeクラスのシングルトン生成
	Time::Create();

	return true;
}

void SystemMain::Finalize() const
{
	// Timeクラスのシングルトン解放
	Time::Destroy();
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
