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
private:	//�V���O���g���p�^�[��
	imguiManager() = default;
	~imguiManager() = default;
	imguiManager(const imguiManager &r) = default;
	imguiManager &operator= (const imguiManager &r) = default;
private:	//�L�q�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:
	//�C���X�^���X�擾
	static imguiManager *GetIns();
	
	//�q�[�v�쐬
	bool CreateDescriptorHeap();
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �E�B���h�E�`��
	/// </summary>
	void Draw();

private:
	static ComPtr<ID3D12DescriptorHeap> heap;

};

