#include "FileManager.h"

FileManager *FileManager::GetIns()
{
	static FileManager *instance;
	return instance;
}

bool FileManager::FileSave(SAVE_TYPE saveType)
{
	static OPENFILENAME openFile;
	//�����t���p�X
	static TCHAR firstFilePath[MAX_PATH];
	//�ۑ���t���p�X
	static TCHAR saveFilePath[MAX_PATH];

	if (firstFilePath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, firstFilePath);
	}

	if (openFile.lStructSize == 0) {
		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = WinApp::GetHwnd();
		openFile.lpstrInitialDir = firstFilePath;	//�����t�H���_�ʒu
		openFile.lpstrFile = saveFilePath;			//�I���t�@�C���i�[
		openFile.nMaxFile = MAX_PATH;
		switch (saveType)
		{
		case SAVE_TYPE::PROJECT:
			openFile.lpstrDefExt = TEXT(".project");		//�g���q
			openFile.lpstrFilter = TEXT("yabuEnginProject(*.project)\0*.project\0");
			openFile.lpstrTitle = TEXT("Save the project");
			break;
		case SAVE_TYPE::SCENE:
			openFile.lpstrDefExt = TEXT(".scene");		//�g���q
			openFile.lpstrFilter = TEXT("yabuEnginScene(*.scene)\0*.scene\0");
			openFile.lpstrTitle = TEXT("Save the scene");
			break;
		default:
			break;
		}

		openFile.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	//�ۑ�����
	if (GetSaveFileName(&openFile)) {

		//���b�Z�[�W�{�b�N�X
		//MessageBox(WinApp::GetHwnd(), saveFilePath, TEXT("Save the project"), MB_OK);
		
		return true;
	}


	return false;
}

bool FileManager::LoadFile()
{
	return false;
}
