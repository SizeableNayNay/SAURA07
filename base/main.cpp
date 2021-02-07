// used: sleep_for
#include <thread>

//		  ██████  ▄▄▄       █    ██  ██▀███   ▄▄▄      		//		credits: Sergey Fursa		//
//		▒██    ▒ ▒████▄     ██  ▓██▒▓██ ▒ ██▒▒████▄    		//			also known as qo0		//
//		░ ▓██▄   ▒██  ▀█▄  ▓██  ▒██░▓██ ░▄█ ▒▒██  ▀█▄  		//		                  ____		//
//		  ▒   ██▒░██▄▄▄▄██ ▓▓█  ░██░▒██▀▀█▄  ░██▄▄▄▄██ 		//		  ______  ____   / __ \		//
//		▒██████▒▒ ▓█   ▓██▒▒▒█████▓ ░██▓ ▒██▒ ▓█   ▓██▒		//		 / __  / / __ \ / / / /		//
//		▒ ▒▓▒ ▒ ░ ▒▒   ▓▒█░░▒▓▒ ▒ ▒ ░ ▒▓ ░▒▓░ ▒▒   ▓▒█░		//		/ /_/ / / /_/ // /_/ /		//
//		░ ░▒  ░ ░  ▒   ▒▒ ░░░▒░ ░ ░   ░▒ ░ ▒░  ▒   ▒▒ ░		//		\__  /  \____/ \____/		//
//		░  ░  ░    ░   ▒    ░░░ ░ ░   ░░   ░   ░   ▒   		//		  /_/						//
//		      ░        ░  ░   ░        ░           ░  ░		//									//
//	      ░        ░  ░   ░        ░           ░  ░			//		base amended by: NayNay		//

/*
 - comments navigation:
 1	"@note:"
 2	"@todo:"
 3	"@test: [things to test] [date]"
 4	"@credits:"
 5	"@xref:"
 *
 - conventions:
 1	prefix classes with 'C' and interfaces classes 'I'
 *		e.g. CSomeClass
 *		e.g. ISomeInterface
 2	postfix structures with '_t'
 *		e.g. SomeStruct_t
 3	prefix enumerations with 'E'
 *		e.g. enum ESomeEnum
 4	thirdparty (sdk, dependencies) macroses should be uppercase
 *		e.g. #define SOMEMACROS
 5	prefix own macroses with the first filename character
 *		e.g. #define M_SOMEMATHMACROS
 *
 - code style:
 1	all curly braces should be on it's own line
 *	e.g.
 *	if (true)
 *	{
 *		while (true)
 *		{
 *			// something...
 *		}
 *	}
 *
 2	camel case should be the prefered way of naming variables
 *	e.g.
 *	bool bSomeBool; int iSomeInt; float flSomeFloat; double dbSomeDouble;
 *	char chSomeSymbol; const char* szSomeString (std::string too);
 *	BYTE dSomeByte; DWORD dwSomeDword; unsigned uSomeInt + type (e.g. unsigned long ulOffset);
 *	CSomeClass someClass; CSomeClass* pSomeClass;
 *
 3	enumerations must inherit type
 *	e.g. enum ESomeEnum : short
 *	enumerations members should be uppercase
 *	e.g.
 *	enum
 *	{
 *		SOME_ENUM_MEMBER = 0
 *	}
 *
 4	includes paths must be separated with one slash
 *	additional: file names preferably be lowercase
 *
 5	use number literal's (in uppercase)
 *	e.g.
 *	long lSomeLong = 0L;
 *	unsigned int uSomeOffset = 0x0; // hex
 *	unsigned int uSomeInt = 0U;
 *	unsigned long ulSomeLong = 0UL;
 *
 - preprocessor definitions:
 1	_DEBUG - disable string encryption and activate external console logging -> automatic switches with compilation configuration
 2	DEBUG_CONSOLE - activate external console logging but deactivate file logging
 *
 - disabled warnings (release):
 1	4244 - 'argument' : conversion from 'type1' to 'type2', possible loss of data
 2	4307 - 'operator' : integral constant overflow
 *
 - additional dependencies:
 1	d3d9.lib - directx rendering
 2	d3dx9.lib - directx rendering
 3	freetype.lib - font rasterizer // @credits: https://www.freetype.org/
 *
 * @note: add "/d1 reportAllClassLayout" in compiler options to get all virtual tables indexes
 */

//		   _____         .__			//
//		  /     \ _____  |__| ____		//
//		 /  \ /  \\__  \ |  |/    \		//
//		/    Y    \/ __ \|  |   |  \	//
//		\____|__  (____  /__|___|  /	//
//		        \/     \/        \/		//

// used: winapi, directx, fmt includes
#include "common.h"
// used: global variables
#include "global.h"
// used: cheat variables
#include "core/variables.h"
// used: eventlistener setup/destroy
#include "utilities.h"
// used: result logging
#include "utilities/logging.h"
// used: exports setup
#include "utilities/math.h"
// used: inputsystem setup/destroy
#include "utilities/inputsystem.h"
// used: render setup/destroy
#include "utilities/draw.h"
// used: netvar manager setup
#include "core/netvar.h"
// used: configurations setup
#include "core/config.h"
// used: hooks setup/destroy
#include "core/hooks.h"

DWORD WINAPI OnDllAttach(LPVOID lpParameter)
{
	try
	{
		/*
		 * @note: serverbrowser.dll is last loaded module (u can seen it when debug)
		 * here is check for all modules loaded
		 */
		while (GetModuleHandle(SERVERBROWSER_DLL) == nullptr)
			std::this_thread::sleep_for(200ms);

		#ifdef DEBUG_CONSOLE
		// console logging
		if (!L::Attach(XorStr("SAURA07's developer-mode")))
			throw std::runtime_error(XorStr("failed to attach console"));

		L::Print(XorStr("console opened"));
		#else
		// file logging
		// @note: use std::ios::app instead std::ios::trunc to not clear every time
		L::ofsFile.open(C::GetWorkingPath().append(XorStr("SAURA07.log")), std::ios::out | std::ios::trunc);
		#endif

		// capture interfaces from game/steam (not always) modules
		if (!I::Setup())
			throw std::runtime_error(XorStr("failed to capture interfaces"));

		L::Print(XorStr("interfaces captured"));

		// version check to know when u need to fix something
		#ifdef DEBUG_CONSOLE
		if (strcmp(I::Engine->GetProductVersionString(), XorStr("1.37.7.6")) != 0)
		{
			L::PushConsoleColor(FOREGROUND_YELLOW);
			L::Print(fmt::format(XorStr("[warning] version doesnt match! current cs:go version: {}"), I::Engine->GetProductVersionString()));
			L::PopConsoleColor();
		}
		#endif

		/*
		 * fill networkable variables map
		 * dump received netvars to the file
		 */
		if (!CNetvarManager::Get().Setup(XorStr("netvars.saur")))
			throw std::runtime_error(XorStr("failed to initialize netvars"));

		L::Print(fmt::format(XorStr("found [{:d}] props in [{:d}] tables"), CNetvarManager::Get().iStoredProps, CNetvarManager::Get().iStoredTables));

		// export completed mathematics functions from game/steam (not always) modules
		if (!M::Setup())
			throw std::runtime_error(XorStr("failed to get math exports"));

		L::Print(XorStr("math exports loaded"));

		// replace window messages processor
		if (!IPT::Setup())
			throw std::runtime_error(XorStr("failed to set window messages processor"));

		L::Print(XorStr("inputsystem setup complete"));

		#if 0
		// start tracking entities
		U::EntityListener.Setup();
		L::Print(XorStr("entity listener initialized"));
		#endif

		// start tracking specified events from vector
		// @note: all events list: https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events
		U::EventListener.Setup({ XorStr("player_hurt"), XorStr("round_prestart"), XorStr("round_freeze_end") });
		L::Print(XorStr("events registered"));

		// add our functionality in client functions
		if (!H::Setup())
			throw std::runtime_error(XorStr("failed initialize hooks"));

		L::Print(XorStr("hooks setup complete"));

		// add our functionality in networkable functions
		if (!P::Setup())
			throw std::runtime_error(XorStr("failed initialize proxies"));

		L::Print(XorStr("proxies applied"));

		// setup values to save/load cheat variables in/from files and load default configuration
		if (!C::Setup(XorStr("default.saur")))
		{
			// this error is not critical, only show that
			L::PushConsoleColor(FOREGROUND_RED);
			L::Print(XorStr("[error] failed setup and/or load default configuration"));
			L::PopConsoleColor();
		}
		else
			L::Print(XorStr("default config loaded"));

		// show message about successful load in logs and in game console
		L::PushConsoleColor(FOREGROUND_MAGENTA);
		L::Print(XorStr("SAURA07 successfully loaded"));
		L::PopConsoleColor();
		I::GameConsole->Clear();
		I::ConVar->ConsoleColorPrintf(Color(255, 50, 255, 255), XorStr("SAURA07 successfully loaded.\nbuild date: %s / %s\n"), __DATE__, __TIME__);
	}
	catch (const std::exception& ex)
	{
		// print error message
		L::PushConsoleColor(FOREGROUND_INTENSE_RED);
		L::Print(fmt::format(XorStr("[error] {}"), ex.what()));
		L::PopConsoleColor();

		#ifdef _DEBUG
		// show error message window (or replace to your exception handler)
		MessageBox(nullptr, ex.what(), XorStr("SAURA07 (error)"), MB_OK | MB_ICONERROR | MB_TOPMOST);
		#else
		// unload
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
		#endif
	}

	return 1UL;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter)
{
	// unload cheat if pressed specified key
	while (!IPT::IsKeyReleased(C::Get<int>(Vars.iPanicKey)))
		std::this_thread::sleep_for(500ms);

	#ifdef DEBUG_CONSOLE
	// detach console
	L::Detach();
	#else
	// close logging output file
	if (L::ofsFile.is_open())
		L::ofsFile.close();
	#endif

	#if 0
	// destroy entity listener
	U::EntityListener.Destroy();
	#endif

	// destroy events listener
	U::EventListener.Destroy();

	// reset crosshair state
	I::ConVar->FindVar(XorStr("crosshair"))->SetValue(true);

	// restore proxies
	P::Restore();

	// restore hooks
	H::Restore();

	// set back to original window messages processor
	IPT::Restore();

	// destroy render
	D::Destroy();

	/*
	 * free our library memory from process and exit from our thread
	 * anyway throws assertion about source engine max unique threads limit (16)
	 */
	FreeLibraryAndExitThread((HMODULE)lpParameter, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls(hModule);

		// basic process check
		if (GetModuleHandle(XorStr("csgo.exe")) == nullptr)
		{
			MessageBox(nullptr, XorStr("this cannot be injected in another process\nopen <csgo.exe> to inject"), XorStr("SAURA07"), MB_OK);
			return FALSE;
		}

		// save our module
		G::hDll = hModule;

		// create main thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		// create detach thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllDetach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		return TRUE;
	}

	return FALSE;
}
