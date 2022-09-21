#include "Looper.h"
#include "DirectXCommon.h"
#include "AudioManager.h"	//音声管理
//#include "KeyboardInput.h"
#include "Input.h"
#include "PrimitiveRenderer.h"
#include "Time/Time.h"
#include "Object/GameObject/GameObject.h"
#include "Collision/CheckCollision.h"
//シーン
#include "TitleScene.h"

Looper::Looper() {
	//最初のシーン
	OnSceneChanged(Scenes::Title, false);
	editor.Initialize(sceneStack.top());
	//KeyboardInput::GetIns()->Initialize();
	Input::Initialize();
}

bool Looper::Loop()
{
	// 計測開始
	Time::GetInstance()->InstrumentationStart();
	// ImGui描画前処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// エディタ描画
	editor.Draw();


	//キーボード更新
	//KeyboardInput::GetIns()->Update();
	Input::Update();

	// 物理挙動アップデート
	{
		for (int i = 0; Time::GetInstance()->CheckFixedUpdate(); ++i)
		{
			sceneStack.top()->FixedUpdate();

			// 経過時間を減少させる
			Time::GetInstance()->SubFixedTimer();

			// 当たり判定
			CheckCollision::CheckColliders(sceneStack.top()->GetObjectManager().lock()->game_objects_);

			// 5回処理して改善しなければ強制的に離脱
			if(i >=10)
			{
				break;
			}
		}
	}
	// 各種初期化
	PrimitiveRenderer::GetInstance().FrameInitialize();	// プリミティブのバッファインデックス初期化


	//シーンの更新
	sceneStack.top()->Update();


	//シーンの描画コマンドを発行
	sceneStack.top()->PreDrawMultiRenderTarget(DirectXCommon::dev,DirectXCommon::cmdList);	// マルチレンダーターゲットの設定
	sceneStack.top()->Draw();																	// 描画コマンド発行
	sceneStack.top()->PostDrawMultiRenderTarget(DirectXCommon::cmdList);						// 設定終了
	// ここまでの描画はマルチレンダーターゲットの対象

	sceneStack.top()->PreDrawPostEffect(DirectXCommon::dev,DirectXCommon::cmdList);				// ポストエフェクトの設定
	sceneStack.top()->DrawMulutiRenderTarget(DirectXCommon::dev, DirectXCommon::cmdList);		// ディファ―レンダリング描画

	sceneStack.top()->PostDrawPoseEffect(DirectXCommon::cmdList);								// 設定終了

	//バッファクリア
	DirectXCommon::ResourceBarrierWriting();
	DirectXCommon::ScreenClear();

	// ポストエフェクトの描画
	sceneStack.top()->DrawPostEffect(DirectXCommon::cmdList);


	imguiManager::GetIns()->Draw();
	// 全コマンド実行
	DirectXCommon::PlayCommandList();

	// 計測開始
	Time::GetInstance()->InstrumentationEnd();

	//ESCが押されたらゲームを終了
	if (Input::GetKeyPressTrigger(DIK_ESCAPE)) {
		return false;
	}
	return true;
}

void Looper::OnSceneChanged(const Scenes scene, const bool stackClear)
{
	//現在のシーンを終了
	if(sceneStack.size() != 0)
	{
		sceneStack.top()->Finalize();
	}
	if (stackClear == true) {				//スタックをクリアする設定なら
		while (!sceneStack.empty()) {	//スタックがからになるまでポップする
			sceneStack.pop();
		}
	}
	switch (scene) {
	case Scenes::Title:
		sceneStack.push(make_shared<TitleScene>(this));
		break;
	
	default:
		//存在しないシーンがスタックトップにある際のエラー処理
		
		break;
	}
	// ゲームオブジェクトに現在のマネージャーをセット
	GameObject::SetGameObjectManager(sceneStack.top()->GetObjectManager());

	//新しいシーンを初期化
	sceneStack.top()->Initialize();
	editor.Initialize(sceneStack.top());

}

void Looper::SceneStackPop()
{
	sceneStack.top()->Finalize();
	sceneStack.pop();
	sceneStack.top()->Initialize();
}


