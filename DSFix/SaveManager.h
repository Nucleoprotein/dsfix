#pragma once

#include <Shlobj.h>
#include <ctime>
#include <cstdio>

#include <string>
#include <vector>

#include "main.h"

class SaveManager
{
	static SaveManager instance;

	std::wstring userSaveFolder;
	std::wstring userBackupFolder;
	std::wstring saveGameFile;

	time_t lastBackupTime;

	std::wstring getFileNameFromPath(const std::wstring& path);
	std::vector<std::wstring> getBackupFiles();

	void autoBackup(const time_t curTime);
	void removeOldBackups();

	time_t getLastBackupTime();

public:
	static SaveManager& get()
	{
		return instance;
	}

	SaveManager() : lastBackupTime(0) {}

	void manualBackup(int slot);
	void manualRestore(int slot);

	void init();
	void tick();
};

