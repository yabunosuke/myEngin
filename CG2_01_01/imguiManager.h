#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


#include <wrl.h>

#include "AbstractScene.h"


class imguiManager
{
private:	//シングルトンパターン
	imguiManager() = default;
	~imguiManager() = default;
	imguiManager(const imguiManager &r) = default;
	imguiManager &operator= (const imguiManager &r) = default;
private:	//記述省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:
	//インスタンス取得
	static imguiManager *GetIns();
	
	//ヒープ作成
	bool CreateDescriptorHeap();
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// ウィンドウ描画
	/// </summary>
	void Draw();

private:
	static ComPtr<ID3D12DescriptorHeap> heap;

};

