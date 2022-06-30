#include <d3dx12.h>
#include "SystemMain.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystemMain system;
	if (system.Initialize()) {
		system.main();
	}
	system.Finalize();

	return 0;
}
