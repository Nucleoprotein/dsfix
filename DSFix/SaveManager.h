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
	void init();

public:
	static SaveManager& get()
	{
		return instance;
	}

	SaveManager::SaveManager() : lastBackupTime(0) {};
	~SaveManager();

	bool enabled() { return Settings::get().getEnableBackups() && !userBackupFolder.empty(); }

	void manualBackup(int slot);
	void manualRestore(int slot);

	void tick();
};

