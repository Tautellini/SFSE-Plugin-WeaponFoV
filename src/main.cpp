#include <windows.h>
#include "sfse_common/Log.h"
#include "sfse_common/sfse_version.h"
#include "sfse/PluginAPI.h"
#include <ShlObj.h>  // CSIDL_MYDOCUMENTS
#include "version.h"  // VERSION_VERSTRING, VERSION_MAJOR
#include "sfse_common/Relocation.h"
#include "sfse_common/SafeWrite.h"
#include <thread>
#include <SimpleIni.h>

RelocAddr <uintptr_t*> weaponFoV = 0x79FD448;

extern "C" {
	SFSEPluginVersionData SFSEPlugin_Version = {
		SFSEPluginVersionData::kVersion,
		WFOV_VERSION,
		WFOV_NAME,
		WFOV_AUTHOR,
		0,	// not address independent
		0,	// not structure independent
		{ RUNTIME_VERSION_1_7_23, 0 },
		0,	// works with any version of the script extender. you probably do not need to put anything here
		0, 0,	// set these reserved fields to 0
	};

	void UpdateFoVValue() 
	{
		_MESSAGE("Thread for updating FoV Values started");

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(L"Data\\SFSE\\Plugins\\weaponfov.ini");
		float newFoV = ini.GetDoubleValue("global", "weaponfov", 120.0f);

		while (true) {
			safeWriteBuf(weaponFoV.getUIntPtr(), &newFoV, sizeof(float));
			Sleep(1000);
		}
	}

	void SFSEPlugin_Load(const SFSEInterface* sfse)
	{
		DebugLog::openRelative(CSIDL_MYDOCUMENTS, "\\My Games\\" SAVE_FOLDER_NAME "\\SFSE\\Logs\\sfse-plugin-weaponfov.txt");
		_MESSAGE("WeaponFoVPlugin loaded");

		std::thread loopThread (UpdateFoVValue);

		_MESSAGE("detaching thread for overriding weapon fov values");
		loopThread.detach();
	}
};
