#pragma once

//		_________											//
//		\_   ___ \  ____   _____   _____   ____   ____		//
//		/    \  \/ /  _ \ /     \ /     \ /  _ \ /    \		//
//		\     \___(  <_> )  Y Y  \  Y Y  (  <_> )   |  \	//
//		 \______  /\____/|__|_|  /__|_|  /\____/|___|  /	//
//		        \/             \/      \/            \/		//

/* winapi */
#include <windows.h>

/* directx includes */
#include <d3d9.h>
#include <d3dx9.h>

/* imgui */
// @credits: https://github.com/ocornut/imgui
#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_freetype.h"

/* imgui implementation */
#include "../dependencies/imgui/imgui_internal.h"
#include "../dependencies/imgui/dx9/imgui_impl_dx9.h"
#include "../dependencies/imgui/win32/imgui_impl_win32.h"

/* formatting library */
// @credits: https://github.com/fmtlib/fmt/
#define FMT_HEADER_ONLY
#include "../dependencies/fmt/format.h"
#include "../dependencies/fmt/chrono.h"

// used: ns,ms,s,m,h time literals
using namespace std::chrono_literals;

/* crypt */
#include "utilities/xorstr.h"
// @note: also u can try lazy importer for some anti-reverse safety. documentation is available at https://github.com/JustasMasiulis/lazy_importer

/* other */
#include "utilities/memory.h"
