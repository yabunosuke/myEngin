#include "Looper.h"
#include "DirectXCommon.h"
#include "AudioManager.h"	//音声管理
//#include "KeyboardInput.h"
#include "Input.h"
#include "PrimitiveRenderer.h"
#include "Time/Time.h"
#include "Collision/CheckCollision.h"
//シーン
#include "TitleScene.h"
#include "GameScene.h"

#include "Object/GameObject/GameObject.h"
#include "Object/Component/Camera.h"
#include "Object/Component/Light.h"
#include "Texture.h"
#include "Singleton/Singleton.h"

Sprite *sprite;
int sprite_num;

Looper::Looper() {
	//最初のシーン
	OnSceneChanged(Scenes::Game, false);
	//KeyboardInput::GetIns()->Initialize();
	Input::Initialize();

	sprite_num = TextureManager::LoadTexture(DirectXCommon::dev.Get(), L"Assets/2d/Panorama-V01.png");
	sprite = Sprite::Create(DirectXCommon::dev, sprite_num);

}

bool Looper::Loop()
{
	// 遷移更新
	if (scene_change_data_.type != ChangeType::None)
	{
		if (scene_change_data_.type == ChangeType::Push)
		{
			//現在のシーンを終了
			if (scene_stack_.size() != 0)
			{
				scene_stack_.top()->Finalize();
			}
			if (scene_change_data_.is_clear) {				//スタックをクリアする設定なら
				while (!scene_stack_.empty()) {	//スタックがからになるまでポップする
					scene_stack_.pop();
				}
			}
			switch (scene_change_data_.scene_name) {
			case Scenes::Title:
				scene_stack_.push(make_unique<TitleScene>(this));
				break;

			case Scenes::Game:
				scene_stack_.push(make_unique<GameScene>(this));
				break;

			default:
				//存在しないシーンがスタックトップにある際のエラー処理

				break;
			}
			// ゲームオブジェクトに現在のマネージャーをセット
			GameObject::SetGameObjectManager(scene_stack_.top()->GetObjectManager());
			Camera::SetCameraManager(scene_stack_.top()->GetCameraManager());
			Light::SetLightManager(scene_stack_.top()->GetLightManager());
			//新しいシーンを初期化
			scene_stack_.top()->Initialize();
		}
		else
		{
			scene_stack_.top()->Finalize();
			scene_stack_.pop();
			//scene_stack_.top()->Initialize();
			GameObject::SetGameObjectManager(scene_stack_.top()->GetObjectManager());
			Camera::SetCameraManager(scene_stack_.top()->GetCameraManager());
			Light::SetLightManager(scene_stack_.top()->GetLightManager());
		}

		editor.Initialize(scene_stack_.top().get());
		scene_change_data_.type = ChangeType::None;
		scene_change_data_.scene_name = Scenes::MAX;
		scene_change_data_.is_clear = false;
	}

	// 計測開始
	Singleton<Time>::GetInstance().InstrumentationStart();

// ImGui描画前処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// エディタ描画
	editor.Draw();


	// 追加処理
	scene_stack_.top()->GetObjectManager()->AddObjects();

	//キーボード更新
	//KeyboardInput::GetIns()->Update();
	Input::Update();

	// 物理挙動アップデート
	{
		for (int i = 0; Singleton<Time>::GetInstance().CheckFixedUpdate(); ++i)
		{
			_RPT0(_CRT_WARN, "FixedUpdate\n");
			scene_stack_.top()->FixedUpdate();

			// 当たり判定
			CheckCollision::CheckColliders(scene_stack_.top()->GetObjectManager()->game_objects_);

			// 経過時間を減少させる
			Singleton<Time>::GetInstance().SubFixedTimer();
			CheckCollision::PenaltyCalc();
			// 2回処理して改善しなければ強制的に離脱
			if(i > 0)
			{
				Singleton<Time>::GetInstance().ClearFixedTimer();
				break;
			}
		}
	}
	// 各種初期化
	PrimitiveRenderer::GetInstance().FrameInitialize();	// プリミティブのバッファインデックス初期化


	//シーンの更新
	_RPT0(_CRT_WARN, "Update\n");
	scene_stack_.top()->Update();

	// 削除処理
	Object::Destroyer();

	//シーンの描画コマンドを発行
	scene_stack_.top()->PreDrawMultiRenderTarget(DirectXCommon::dev,DirectXCommon::cmdList);	// マルチレンダーターゲットの設定
	scene_stack_.top()->Draw();																	// 描画コマンド発行
	scene_stack_.top()->PostDrawMultiRenderTarget(DirectXCommon::cmdList);						// 設定終了
	// ここまでの描画はマルチレンダーターゲットの対象

	scene_stack_.top()->PreDrawPostEffect(DirectXCommon::dev,DirectXCommon::cmdList);				// ポストエフェクトの設定
	// スカイボックス描画
	sprite->Draw(DirectXCommon::dev, DirectXCommon::cmdList, "SkyBox");
	scene_stack_.top()->DrawMulutiRenderTarget(DirectXCommon::dev, DirectXCommon::cmdList);		// ディファ―レンダリング描画
	scene_stack_.top()->PostDrawPoseEffect(DirectXCommon::cmdList);								// 設定終了

	//バッファクリア
	DirectXCommon::ResourceBarrierWriting();
	DirectXCommon::ScreenClear();
	

	// ポストエフェクトの描画
	scene_stack_.top()->DrawPostEffect(DirectXCommon::cmdList);

	ImGui::Begin("fps");
	float fps = 1.0f/static_cast<float>(Singleton<Time>::GetInstance().time);
	ImGui::DragFloat("FPS", &fps);
	ImGui::End();
	
	imguiManager::GetIns()->Draw();
	/*PrimitiveRenderer::GetInstance().DrawBox(
		DirectXCommon::cmdList,
		PrimitiveRenderer::Box{ {0,10,0},{15,15,15},{20,20,20} },
		Camera::main.r_->viewMatrix.r_ * Camera::main.r_->projectionMatrix.r_
	);*/
	// 全コマンド実行
	DirectXCommon::PlayCommandList();


	// 計測終了
	Singleton<Time>::GetInstance().InstrumentationEnd();

	return !exit_window_;
}

void Looper::OnSceneChanged(const Scenes scene, const bool stackClear)
{
	scene_change_data_.type = ChangeType::Push;
	scene_change_data_.scene_name = scene;
	scene_change_data_.is_clear = stackClear;
}

void Looper::SceneStackPop()
{
	scene_change_data_.type = ChangeType::Pop;
}


