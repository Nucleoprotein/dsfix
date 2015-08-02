#include "stdafx.h"
#include "Settings.h"

#include <fstream>
#include "main.h"
#include "WindowManager.h"
#include "Detouring.h"

Settings Settings::instance;

void Settings::load()
{
	std::ifstream sfile;
	sfile.open(GetDirectoryFile("DSfix.ini"), std::ios::in);
	char buffer[128];
	while (!sfile.eof()) {
		sfile.getline(buffer, 128);
		if (buffer[0] == '#') continue;
		if (sfile.gcount() <= 1) continue;
		std::string bstring(buffer);

#define SETTING(_type, _var, _inistring, _defaultval) \
		if(bstring.find(_inistring) == 0) { \
			read(buffer + strlen(_inistring) + 1, _var); \
                		}
#include "Settings.def"
#undef SETTING
	}
	sfile.close();

	if (getBackupInterval() < 300) {
		BackupInterval = 300;
	}

	if (getPresentWidth() == 0) PresentWidth = getRenderWidth();
	if (getPresentHeight() == 0) PresentHeight = getRenderHeight();

	curFPSlimit = getFPSLimit();
}

void Settings::report()
{
	SDLOG(0, "= Settings read:");
#define SETTING(_type, _var, _inistring, _defaultval) \
	log(_inistring, _var);
#include "Settings.def"
#undef SETTING
	SDLOG(0, "=============");
}

void Settings::init()
{
	if (!inited) {
		if (getDisableCursor()) WindowManager::get().toggleCursorVisibility();
		if (getCaptureCursor()) WindowManager::get().toggleCursorCapture();
		if (getBorderlessFullscreen()) WindowManager::get().toggleBorderlessFullscreen();

		WindowManager::get().resize(NULL, NULL);
		inited = true;
	}
}

void Settings::shutdown()
{
	if (inited) {
		inited = false;
	}
}

unsigned Settings::getCurrentFPSLimit()
{
	return curFPSlimit;
}
void Settings::setCurrentFPSLimit(unsigned limit)
{
	curFPSlimit = limit;
}
void Settings::toggle30FPSLimit()
{
	if (curFPSlimit == 30) curFPSlimit = getFPSLimit();
	else curFPSlimit = 30;
}


// reading --------------------------------------------------------------------

void Settings::read(char* source, bool& value)
{
	std::string ss(source);
	if (ss.find("true") == 0 || ss.find("1") == 0 || ss.find("TRUE") == 0 || ss.find("enable") == 0) value = true;
	else value = false;
}

void Settings::read(char* source, int& value)
{
	sscanf_s(source, "%d", &value);
}

void Settings::read(char* source, unsigned& value)
{
	sscanf_s(source, "%u", &value);
}

void Settings::read(char* source, float& value)
{
	sscanf_s(source, "%f", &value);
}

void Settings::read(char* source, std::string& value)
{
	value.assign(source);
}


// logging --------------------------------------------------------------------

void Settings::log(const char* name, bool value)
{
	SDLOG(0, " - %s : %s", name, value ? "true" : "false");
}

void Settings::log(const char* name, int value)
{
	SDLOG(0, " - %s : %d", name, value);
}

void Settings::log(const char* name, unsigned value)
{
	SDLOG(0, " - %s : %u", name, value);
}

void Settings::log(const char* name, float value)
{
	SDLOG(0, " - %s : %f", name, value);
}

void Settings::log(const char* name, const std::string& value)
{
	SDLOG(0, " - %s : %s", name, value.c_str());
}


