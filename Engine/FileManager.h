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
	/// ファイル保存
	/// </summary>
	/// <param name="saveType">ダイアログの内容変更</param>
	/// <returns>保存した場合はtrue</returns>
	bool FileSave(SAVE_TYPE saveType);


	bool LoadFile();
private:

	bool CreateSceneFile(OPENFILENAME &openFile);
};

