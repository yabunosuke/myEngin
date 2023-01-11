#pragma once
#include "WinApp.h"
#include <string>

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
	/// ƒtƒ@ƒCƒ‹•Û‘¶
	/// </summary>
	/// <param name="first_file_path"></param>
	/// <param name="save_file_path"></param>
	/// <param name="ext"></param>
	/// <param name="filter"></param>
	/// <param name="title"></param>
	/// <returns></returns>
	bool FileSave(
		OPENFILENAME &open_file,
		TCHAR first_file_path[],
		TCHAR save_file_path[],
		const std::wstring &ext,
		const std::wstring &filter,
		const std::wstring &title
	);

	//bool LoadLevelJson(const std::string &file_path);

	bool LoadFile();
private:

	bool SceneFileSetting(OPENFILENAME &openFile);
	bool CreateSceneFile();
};

