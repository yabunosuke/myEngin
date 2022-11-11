#include <d3dx12.h>
#include "SystemMain.h"

// メモリリーク検出
//#if _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	SystemMain system;
	if (system.Initialize()) {
		system.main();
	}
	system.Finalize();

	return 0;
}