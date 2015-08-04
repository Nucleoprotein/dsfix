#include "stdafx.h"


#include "KeyActions.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "main.h"
#include "WindowManager.h"
#include "SaveManager.h"
#include "Settings.h"
#include "RenderstateManager.h"

KeyActions KeyActions::instance;

void KeyActions::load()
{
	std::ifstream sfile;
	sfile.open(GetDirectoryFile("DSfixKeys.ini"), std::ios::in);
	char buffer[256];
	while (!sfile.eof())
	{
		sfile.getline(buffer, 256);
		if (buffer[0] == '#') continue;
		if (sfile.gcount() <= 1) continue;
		std::string bstring(buffer);

		size_t pos = bstring.npos;
		char postChar;
#define KEY(_name, _val) \
		pos = bstring.find(#_name); \
		postChar = buffer[pos + strlen(#_name)]; \
		if(pos != bstring.npos && (postChar == '\r' || postChar == '\n' || postChar == ' ' || postChar == '\0')) { \
			std::string action; std::stringstream ss(bstring); ss >> action; \
			keyBindingMap.insert(make_pair(_val, action)); \
				}
#include "Keys.def"
#undef KEY
	}
	sfile.close();
}

void KeyActions::report()
{
	SDLOG(0, "= Loaded Keybindings:");
	for (IntStrMap::const_iterator i = keyBindingMap.begin(); i != keyBindingMap.end(); ++i)
	{
		SDLOG(0, " - %p => %s", i->first, i->second.c_str());
	}
	SDLOG(0, "=============");

	SDLOG(5, "= Possible Actions:");
#define ACTION(_name, _action) \
	SDLOG(5, "%s, ", #_name);
#include "Actions.def"
#undef ACTION
	SDLOG(5, "=============");

	SDLOG(5, "= Possible Keys:");
#define KEY(_name, _val) \
	SDLOG(5, "%s, ", #_name);
#include "Keys.def"
#undef KEY
	SDLOG(5, "=============");
}

void KeyActions::performAction(const char* name)
{
#define ACTION(_name, _action) \
	if(strcmp(#_name, name) == 0) _name();
#include "Actions.def"
#undef ACTION
}

void KeyActions::processIO()
{
	for (auto& key : keyBindingMap)
	{
		if (GetAsyncKeyState(key.first) & 1)
		{
			SDLOG(0, "Action triggered: %s", key.second.c_str());
			performAction(key.second.c_str());
		}
	}
}

#define ACTION(_name, _action) \
void KeyActions::_name() { _action; };
#include "Actions.def"
#undef ACTION
