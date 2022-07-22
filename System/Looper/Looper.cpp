#include "Looper.h"
#include "DirectXCommon.h"
#include "AudioManager.h"	//音声管理
#include "KeyboardInput.h"

#include "ComponentList.h"

//シーン
#include "TitleScene.h"

Looper::Looper() {
	//最初のシーン
	sceneStack.push(make_shared<TitleScene>(this));
	sceneStack.top()->Initialize();
	ComponentList::GetIns()->Initialize();
	editor.Initialize(sceneStack.top());
	KeyboardInput::GetIns()->Initialize();
}

bool Looper::Loop()
{
	// ImGui描画前処理
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//キーボード更新
	KeyboardInput::GetIns()->Update();
	//シーンの更新
	sceneStack.top()->Update();

	//シーンの描画コマンドを発行
	sceneStack.top()->PreDrawScene(DirectXCommon::cmdList);
	sceneStack.top()->Draw();
	sceneStack.top()->PostDrawScene(DirectXCommon::cmdList);
	// ここまでの描画はポストエフェクトの対象
	// エディタ描画
	editor.Draw();
	//描画コマンド実行
	DirectXCommon::ResourceBarrierWriting();
	DirectXCommon::ScreenClear();
	// ポストエフェクト
	sceneStack.top()->DrawPostEffect();

	imguiManager::GetIns()->Draw();
	// 全コマンド実行
	DirectXCommon::PlayCommandList();

	//ESCが押されたらゲームを終了
	if (KeyboardInput::GetIns()->GetKeyPressT(DIK_ESCAPE)) {
		return false;
	}
	return true;
}

void Looper::OnSceneChanged(const Scenes scene, const bool stackClear)
{
	//現在のシーンを終了
	sceneStack.top()->Finalize();
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


