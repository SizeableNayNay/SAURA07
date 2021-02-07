#pragma once
// used: winapi includes
#include "common.h"
// used: vector
#include "sdk/datatypes/vector.h"
// used: qangle
#include "sdk/datatypes/qangle.h"
// used: usercmd
#include "sdk/datatypes/usercmd.h"
// used: baseentity
#include "sdk/entity.h"

//		  ________.__        ___.          .__				//
//		 /  _____/|  |   ____\_ |__ _____  |  |   ______	//
//		/   \  ___|  |  /  _ \| __ \\__  \ |  |  /  ___/	//
//		\    \_\  \  |_(  <_> ) \_\ \/ __ \|  |__\___ \		//
//		 \______  /____/\____/|___  (____  /____/____  >	//
//		        \/                \/     \/          \/		//

/*
 * GLOBALS
 * globally defined values
 * may used in any source/header file what includes in
 */
namespace G
{
	// current module
	inline HMODULE			hDll = nullptr;
	// last localplayer pointer
	inline CBaseEntity*		pLocal = nullptr;
	// last cmd pointer
	inline CUserCmd*		pCmd = nullptr;

	// last sendpacket state
	inline bool				bSendPacket = true;
	// is breaking?
	inline bool				bIsBreaking = false;

	// amount of choked ticks
	inline int				iChokedTicks = 0;

	// last viewangle
	inline QAngle			angViewPoint = { };
	// last viewangle while choking packets
	inline QAngle			angFakeAngle = { };
	// last viewangle while sending packets
	inline QAngle			angRealAngle = { };
	// last aimstep restriction
	inline QAngle			angStep = { };

	// camera origin
	inline Vector			vecCamera = { };
	// main menu highlight color
	inline ImVec4			vecMenuHighlight = { };
}
