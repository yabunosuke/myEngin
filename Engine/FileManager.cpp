#include "FileManager.h"

FileManager *FileManager::GetIns()
{
	static FileManager *instance;
	return instance;
}

bool FileManager::FileSave(SAVE_TYPE saveType)
{
	static OPENFILENAME openFile;
	//初期フルパス
	static TCHAR firstFilePath[MAX_PATH];
	//保存先フルパス
	static TCHAR saveFilePath[MAX_PATH];

	if (firstFilePath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, firstFilePath);
	}

	if (openFile.lStructSize == 0) {
		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = WinApp::GetHwnd();
		openFile.lpstrInitialDir = firstFilePath;	//初期フォルダ位置
		openFile.lpstrFile = saveFilePath;			//選択ファイル格納
		openFile.nMaxFile = MAX_PATH;
		switch (saveType)
		{
		case SAVE_TYPE::PROJECT:
			openFile.lpstrDefExt = TEXT(".project");		//拡張子
			openFile.lpstrFilter = TEXT("yabuEnginProject(*.project)\0*.project\0");
			openFile.lpstrTitle = TEXT("Save the project");
			break;
		case SAVE_TYPE::SCENE:
			openFile.lpstrDefExt = TEXT(".scene");		//拡張子
			openFile.lpstrFilter = TEXT("yabuEnginScene(*.scene)\0*.scene\0");
			openFile.lpstrTitle = TEXT("Save the scene");
			break;
		default:
			break;
		}

		openFile.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	//保存処理
	if (GetSaveFileName(&openFile)) {

		//メッセージボックス
		//MessageBox(WinApp::GetHwnd(), saveFilePath, TEXT("Save the project"), MB_OK);
		
		return true;
	}


	return false;
}

bool FileManager::LoadFile()
{
	return false;
}
