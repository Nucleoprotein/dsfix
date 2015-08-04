#include "stdafx.h"

#include "SaveManager.h"

#include <algorithm>

#include "Settings.h"

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

#define TIMESTAMP_LENGTH 12
#define TIMESTAMP_LENGTH_STR EXPAND_AND_QUOTE(TIMESTAMP_LENGTH)

SaveManager SaveManager::instance;

void SaveManager::init()
{
	if (!Settings::get().getEnableBackups())
		return;

	PWSTR buffer;
	HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &buffer);
	if (FAILED(hr) || !buffer)
		return;

	std::wstring documents(buffer);
	CoTaskMemFree(buffer);

	std::wstring search_userdir = StringFromFormat(L"%s%s", documents.c_str(), L"\\NBGI\\DarkSouls\\*");

	// find user save folder
	WIN32_FIND_DATAW userSaveFolderData;
	HANDLE searchHandle = FindFirstFileW(search_userdir.c_str(), &userSaveFolderData);
	bool found = false;
	if (searchHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wcslen(userSaveFolderData.cFileName) > 2)
			{
				userSaveFolder = StringFromFormat(L"%s\\NBGI\\DarkSouls\\%s", documents.c_str(), userSaveFolderData.cFileName);
				userBackupFolder = StringFromFormat(L"%s\\backup", userSaveFolder.c_str());
				saveGameFile = StringFromFormat(L"%s\\DRAKS0005.sl2", userSaveFolder.c_str());

				CreateDirectoryW(userBackupFolder.c_str(), NULL);

				SDLOG(0, "SaveManager: user save folder is %ls", userSaveFolder.c_str());
				SDLOG(0, "SaveManager: user backup folder is %ls", userBackupFolder.c_str());
				found = true;
				break;
			}

		} while (FindNextFileW(searchHandle, &userSaveFolderData));
	}

	if (!found)
	{
		SDLOG(0, "SaveManager: could not determine user save folder");
		return;
	}

	removeOldBackups();
}

void SaveManager::tick()
{
	if (Settings::get().getEnableBackups())
	{
		time_t curTime = time(NULL);
		if (curTime - getLastBackupTime() > Settings::get().getBackupInterval())
		{
			autoBackup(curTime);
			lastBackupTime = curTime;
		}
	}
}

time_t SaveManager::getLastBackupTime()
{
	if (lastBackupTime == 0)
	{
		std::vector<std::wstring> backupFiles = getBackupFiles();
		if (!backupFiles.empty())
		{
			std::wstring fileName = getFileNameFromPath(backupFiles.front());
			swscanf_s(fileName.c_str(), L"%llu", &lastBackupTime);
		}
	}
	SDLOG(3, "SaveManager: last backup time %llu", lastBackupTime);
	return lastBackupTime;
}

std::vector<std::wstring> SaveManager::getBackupFiles()
{
	std::vector<std::wstring> files;
	// find saved files
	if (userBackupFolder.length() > 0)
	{
		std::wstring search_file = StringFromFormat(L"%s\\*.bak", userBackupFolder.c_str());
		WIN32_FIND_DATAW saveFileData;
		HANDLE searchHandle = FindFirstFileW(search_file.c_str(), &saveFileData);
		if (searchHandle != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::wstring filename = StringFromFormat(L"%s\\%s", userBackupFolder.c_str(), saveFileData.cFileName);
				files.push_back(filename);
			} while (FindNextFileW(searchHandle, &saveFileData));
		}
		std::sort(files.rbegin(), files.rend());
		for (auto& file : files)
		{
			SDLOG(4, "SaveManager: found existing file %ls", file.c_str());
		}
	}
	return files;
}

void SaveManager::autoBackup(const time_t curTime)
{
	SDLOG(1, "SaveManager: Backing up save files");

	std::wstring newPath = StringFromFormat(L"%s\\%012llu_auto.bak", userBackupFolder.c_str(), curTime);

	if (CopyFileW(saveGameFile.c_str(), newPath.c_str(), FALSE) == 0)
	{
		SDLOG(0, "ERROR: SaveManager failed to back up file! (Copying %ls to %ls)", saveGameFile.c_str(), newPath.c_str());
	}
	else
	{
		SDLOG(1, "SaveManager: Backed up %ls", saveGameFile.c_str());
	}

	removeOldBackups();
}

void SaveManager::manualBackup(int slot)
{
	std::wstring backupPath = StringFromFormat(L"%s\\manual%d.bak", userBackupFolder.c_str(), slot);

	if (CopyFileW(saveGameFile.c_str(), backupPath.c_str(), FALSE) == 0)
	{
		SDLOG(0, "ERROR: SaveManager failed to back up file! (Copying %ls to %ls)", saveGameFile.c_str(), backupPath.c_str());
	}
	else
	{
		SDLOG(1, "SaveManager: Backed up %ls", saveGameFile.c_str());
	}
}

void SaveManager::manualRestore(int slot)
{
	std::wstring backupPath = StringFromFormat(L"%s\\manual%d.bak", userBackupFolder.c_str(), slot);
	if (CopyFileW(backupPath.c_str(), saveGameFile.c_str(), FALSE) == 0)
	{
		SDLOG(0, "ERROR: SaveManager failed to restore file! (Copying %ls to %ls)", backupPath.c_str(), saveGameFile.c_str());
	}
	else
	{
		SDLOG(1, "SaveManager: Restore up %ls", backupPath.c_str());
	}
}

void SaveManager::removeOldBackups()
{
	std::vector<std::wstring> backupFiles = getBackupFiles();
	if (Settings::get().getMaxBackups() < backupFiles.size())
	{
		SDLOG(1, "SaveManager: Removing %u old backups", backupFiles.size() - Settings::get().getMaxBackups());
		for (size_t i = Settings::get().getMaxBackups(); i < backupFiles.size(); ++i)
		{
			DeleteFileW(backupFiles[i].c_str());
		}
	}
}

std::wstring SaveManager::getFileNameFromPath(const std::wstring& path)
{
	size_t pos = path.rfind(L'\\');
	if (pos != path.npos)
	{
		pos += 1;
		return path.substr(pos);
	}
	else
	{
		SDLOG(0, "ERROR: SaveManager could not extract file name from path %ls", path.c_str());
	}
	return path;
}
