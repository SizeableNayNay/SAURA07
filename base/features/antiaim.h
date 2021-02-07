#pragma once
// used: winapi includes, singleton
#include "../common.h"
// used: usercmd
#include "../sdk/datatypes/usercmd.h"
// used: angle
#include "../sdk/datatypes/qangle.h"
// used: baseentity
#include "../sdk/entity.h"

class CAntiAim : public CSingleton<CAntiAim>
{
public:
	// Get
	void Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);

	/* align local animation state to server */
	// @credits: pazzo
	void UpdateServerAnimations(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket);

	/* chams animstates */
	void UpdateFakeAnimations(CBaseEntity* pLocal, bool& bSendPacket); 
	void UpdateFakelagAnimations(CBaseEntity* pLocal, bool& bSendPacket); 

	// Values
	/* angles modified by antiaim and being sent */
	QAngle angSentView = { };
	/* updated by server animstate */
	CBasePlayerAnimState* pServerAnimState = nullptr;
	/* updated by fake animstate */
	CBasePlayerAnimState* pFakeAnimState = nullptr;
	/* updated by fake animstate */
	CBasePlayerAnimState* pFakelagAnimState = nullptr;
	/* updated by fake animstate */
	matrix3x4_t pFakeBones[126];
	/* updated by fake animstate */
	matrix3x4_t pFakelagBones[258];

	/* variables used in drawmodel */
	int iFakeWeightCount = { };
	float flFakeWeights = { };
	float flFakeDelayedWeights = { };
private:
	// Main
	void Pitch(CUserCmd* pCmd, CBaseEntity* pLocal);
	void Yaw(CUserCmd* pCmd, CBaseEntity* pLocal, float flServerTime, bool& bSendPacket);

	// Extra
	/* returns max server desynchronization angle delta */
	float GetMaxDesyncDelta(CBasePlayerAnimState* pAnimState); // @credits: sharklaser1's reversed setupvelocity

	// Values
	/* next lower body yaw update time from server */
	float flNextLowerBodyUpdate = 0.f;
};
