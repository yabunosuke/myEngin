#pragma once
#include "WinApp.h"
#include "json.hpp"

enum class SAVE_TYPE {
	PROJECT,
	SCENE,
};

class FileManager
{
private:
	FileManager() = default;
	~FileManager() = default;
	FileManager(const FileManager & r) = default;
	FileManager &operator= (const FileManager & r) = default;

public:
	static FileManager *GetIns();

	/// <summary>
	/// �t�@�C���ۑ�
	/// </summary>
	/// <param name="saveType">�_�C�A���O�̓��e�ύX</param>
	/// <returns>�ۑ������ꍇ��true</returns>
	bool FileSave(SAVE_TYPE saveType);


	bool LoadFile();
private:

	bool CreateSceneFile(OPENFILENAME &openFile);
};

