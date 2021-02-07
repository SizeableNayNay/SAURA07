#include "antiaim.h"

// used: global variables
#include "../global.h"
// used: cheat variables
#include "../core/variables.h"
// used: globals interface
#include "../core/interfaces.h"
// used: math definitions
#include "../utilities/math.h"
// used: keybind for desync side
#include "../utilities/inputsystem.h"
// used: get corrected tickbase
#include "prediction.h"

void CAntiAim::Run(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
	// check is not frozen and alive
	if (!pLocal->IsAlive() || pLocal->GetFlags() & FL_FROZEN)
		return;

	// is not on a ladder or use noclip (to skip that needs more proper movefix)
	if (pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	// is not uses anything (open doors, defuse, etc)
	if (pCmd->iButtons & IN_USE)
		return;

	CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

	if (pWeapon == nullptr)
		return;

	short nDefinitionIndex = pWeapon->GetItemDefinitionIndex();
	CCSWeaponData* pWeaponData = I::WeaponSystem->GetWeaponData(nDefinitionIndex);

	if (pWeaponData == nullptr)
		return;

	float flServerTime = TICKS_TO_TIME(CPrediction::Get().GetTickbase(pCmd, pLocal));

	// weapon shoot check
	if (pWeaponData->IsGun() && pLocal->CanShoot(static_cast<CWeaponCSBase*>(pWeapon)) && (pCmd->iButtons & IN_ATTACK || (nDefinitionIndex == WEAPON_REVOLVER && pCmd->iButtons & IN_SECOND_ATTACK)))
		return;
	// knife attack check
	else if (pWeaponData->nWeaponType == WEAPONTYPE_KNIFE)
	{
		// slash
		if ((pCmd->iButtons & IN_ATTACK && pWeapon->GetNextPrimaryAttack() <= flServerTime) ||
			// stab
			(pCmd->iButtons & IN_SECOND_ATTACK && pWeapon->GetNextSecondaryAttack() <= flServerTime))
			return;
	}
	// grenade throw check
	else if (auto pGrenade = static_cast<CBaseCSGrenade*>(pWeapon); pGrenade != nullptr && pWeaponData->nWeaponType == WEAPONTYPE_GRENADE)
	{
		// check is being thrown a grenade
		if (!pGrenade->IsPinPulled() || pCmd->iButtons & (IN_ATTACK | IN_SECOND_ATTACK))
		{
			if (pGrenade->GetThrowTime() > 0.f)
				return;
		}
	}

	// save angles to modify it later
	angSentView = pCmd->angViewPoint;

	/* edge antiaim, fakewalk, other hvhboi$tuff do here */

	/*
	 * @note: fyi: https://www2.clarku.edu/faculty/djoyce/complex/polarangle.gif
	 *
	 *	 \     90|450    /
	 *	   \     |     /
	 *	135  \   |   /  45,405
	 *	       \ | /
	 *	-180,180 | 0,360,720
	 *	--------------------
	 *	       / | \
	 *	-135 /   |   \ -45,315
	 *	   /     |     \
	 *	 /    -90|270    \
	 *
	 */

	// do antiaim for pitch
	Pitch(pCmd, pLocal);
	// do antiaim for yaw
	Yaw(pCmd, pLocal, flServerTime, bSendPacket);

	if (C::Get<bool>(Vars.bMiscAntiUntrusted))
	{
		angSentView.Normalize();
		angSentView.Clamp();
	}

	// send angles
	pCmd->angViewPoint = angSentView;
}

void CAntiAim::UpdateServerAnimations(CUserCmd* pCmd, CBaseEntity* pLocal, bool& bSendPacket)
{
	// get values to check for change/reset
	static CBaseHandle hOldLocal = pLocal->GetRefEHandle();
	static float flOldSpawnTime = pLocal->GetSpawnTime();

	bool bAllocate = (pServerAnimState == nullptr);
	bool bChange = (!bAllocate && pLocal->GetRefEHandle() != hOldLocal);
	bool bReset = (!bAllocate && !bChange && pLocal->GetSpawnTime() != flOldSpawnTime);

	// player changed, free old animation state
	if (bChange)
		I::MemAlloc->Free(pServerAnimState);

	// check is need to reset (on respawn)
	if (bReset)
	{
		if (pServerAnimState != nullptr)
			pServerAnimState->Reset();

		flOldSpawnTime = pLocal->GetSpawnTime();
	}

	// get accurate server time
	float flServerTime = TICKS_TO_TIME(CPrediction::Get().GetTickbase(pCmd, pLocal));

	// need to allocate or create new due to player change
	if (bAllocate || bChange)
	{
		// create temporary animstate
		CBasePlayerAnimState* pAnimState = static_cast<CBasePlayerAnimState*>(I::MemAlloc->Alloc(sizeof(CBasePlayerAnimState)));

		if (pAnimState != nullptr)
			pAnimState->Create(pLocal);

		hOldLocal = pLocal->GetRefEHandle();
		flOldSpawnTime = pLocal->GetSpawnTime();

		// note animstate for future use
		pServerAnimState = pAnimState;
	}
	else
	{
		// backup values
		std::array<CAnimationLayer, MAXOVERLAYS> arrNetworkedLayers;
		std::copy(pLocal->GetAnimationOverlays(), pLocal->GetAnimationOverlays() + arrNetworkedLayers.size(), arrNetworkedLayers.data());
		const QAngle angAbsViewOld = pLocal->GetAbsAngles();
		const std::array<float, MAXSTUDIOPOSEPARAM> arrPosesOld = pLocal->GetPoseParameter();

		pServerAnimState->Update(pCmd->angViewPoint);

		// restore values
		std::copy(arrNetworkedLayers.begin(), arrNetworkedLayers.end(), pLocal->GetAnimationOverlays());
		pLocal->GetPoseParameter() = arrPosesOld;
		pLocal->SetAbsAngles(angAbsViewOld);

		// check is walking, delay next update by 0.22s
		if (pServerAnimState->flVelocityLenght2D > 0.1f)
			flNextLowerBodyUpdate = flServerTime + 0.22f;
		// check is standing, update every 1.1s
		else if (std::fabsf(std::remainderf(pServerAnimState->flGoalFeetYaw - pServerAnimState->flEyeYaw, 360.f)) > 35.f && flServerTime > flNextLowerBodyUpdate)
			flNextLowerBodyUpdate = flServerTime + 1.1f;
	}
}

void CAntiAim::UpdateFakeAnimations(CBaseEntity* pLocal, bool& bSendPacket)
{
	if (!pLocal || !pLocal->IsAlive())
		return;

	// get values to check for change/reset
	static CBaseHandle hOldLocal = pLocal->GetRefEHandle();
	static float flOldSpawnTime = pLocal->GetSpawnTime();

	bool bAllocate = (pFakeAnimState == nullptr);
	bool bChange = (!bAllocate && pLocal->GetRefEHandle() != hOldLocal);
	bool bReset = (!bAllocate && !bChange && pLocal->GetSpawnTime() != flOldSpawnTime);

	if (bChange)
		I::MemAlloc->Free(pFakeAnimState);

	if (bReset)
	{
		if (pFakeAnimState != nullptr)
			pFakeAnimState->Reset();

		flOldSpawnTime = pLocal->GetSpawnTime();
	}

	if (bAllocate || bChange)
	{
		// create temporary animstate
		CBasePlayerAnimState* pAnimState = static_cast<CBasePlayerAnimState*>(I::MemAlloc->Alloc(sizeof(CBasePlayerAnimState)));

		// create temp animstate if empty
		if (pAnimState != nullptr)
			pAnimState->Create(pLocal);
		
		hOldLocal = pLocal->GetRefEHandle();
		flOldSpawnTime = pLocal->GetSpawnTime();

		// define the fake animstate as the temp one
		pFakeAnimState = pAnimState;
	}
	else
	{
		// backup values
		std::array<CAnimationLayer, MAXOVERLAYS> arrNetworkedLayers;
		std::copy(pLocal->GetAnimationOverlays(), pLocal->GetAnimationOverlays() + arrNetworkedLayers.size(), arrNetworkedLayers.data());
		const QAngle angAbsViewOld = pLocal->GetAbsAngles();
		const std::array<float, MAXSTUDIOPOSEPARAM> arrPosesOld = pLocal->GetPoseParameter();
		int iEffectsOld = pLocal->GetEffects();

		pLocal->GetEffects() |= EF_NOINTERP;

		// update animstate
		pFakeAnimState->Update(G::angFakeAngle);

		// setup drawmodel variables
		*(uint32_t*)((uintptr_t)pLocal + 0xA68) = 0;
		pLocal->SetupBones(pFakeBones, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->flCurrentTime);
		*(uint32_t*)((uintptr_t)pLocal + 0xA68) = 0;

		// restore values
		pLocal->GetEffects() |= iEffectsOld;
		std::copy(arrNetworkedLayers.begin(), arrNetworkedLayers.end(), pLocal->GetAnimationOverlays());
		pLocal->GetPoseParameter() = arrPosesOld;
		pLocal->SetAbsAngles(angAbsViewOld);
	}
}

void CAntiAim::UpdateFakelagAnimations(CBaseEntity* pLocal, bool& bSendPacket)
{
	if (!pLocal || !pLocal->IsAlive())
		return;

	// get values to check for change/reset
	static CBaseHandle hOldLocal = pLocal->GetRefEHandle();
	static float flOldSpawnTime = pLocal->GetSpawnTime();

	bool bAllocate = (pFakelagAnimState == nullptr);
	bool bChange = (!bAllocate && pLocal->GetRefEHandle() != hOldLocal);
	bool bReset = (!bAllocate && !bChange && pLocal->GetSpawnTime() != flOldSpawnTime);

	if (bChange)
		I::MemAlloc->Free(pFakelagAnimState);

	if (bReset)
	{
		if (pFakelagAnimState != nullptr)
			pFakelagAnimState->Reset();

		flOldSpawnTime = pLocal->GetSpawnTime();
	}

	if (bAllocate || bChange)
	{
		// create temporary animstate
		CBasePlayerAnimState* pAnimState = static_cast<CBasePlayerAnimState*>(I::MemAlloc->Alloc(sizeof(CBasePlayerAnimState)));

		// create temp animstate if empty
		if (pAnimState != nullptr)
			pAnimState->Create(pLocal);

		hOldLocal = pLocal->GetRefEHandle();
		flOldSpawnTime = pLocal->GetSpawnTime();

		// define the fake animstate as the temp one
		pFakelagAnimState = pAnimState;
	}
	else
	{
		// backup values
		std::array<CAnimationLayer, MAXOVERLAYS> arrNetworkedLayers;
		std::copy(pLocal->GetAnimationOverlays(), pLocal->GetAnimationOverlays() + arrNetworkedLayers.size(), arrNetworkedLayers.data());
		const QAngle angAbsViewOld = pLocal->GetAbsAngles();
		const std::array<float, MAXSTUDIOPOSEPARAM> arrPosesOld = pLocal->GetPoseParameter();
		int iEffectsOld = pLocal->GetEffects();

		pLocal->GetEffects() |= EF_NOINTERP;

		// update animstate
		pFakelagAnimState->Update(G::angRealAngle);

		// setup drawmodel variables
		if (bSendPacket)
			pLocal->SetupBones(pFakelagBones, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::Globals->flCurrentTime);

		// restore values
		pLocal->GetEffects() |= iEffectsOld;
		std::copy(arrNetworkedLayers.begin(), arrNetworkedLayers.end(), pLocal->GetAnimationOverlays());
		pLocal->GetPoseParameter() = arrPosesOld;
		pLocal->SetAbsAngles(angAbsViewOld);
	}
}

void CAntiAim::Pitch(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	switch (C::Get<int>(Vars.iAntiAimPitch))
	{
	case (int)EAntiAimPitchType::NONE:
		break;
	case (int)EAntiAimPitchType::UP:
		angSentView.x = -89.0f;
		break;
	case (int)EAntiAimPitchType::DOWN:
		angSentView.x = 89.f;
		break;
	}
}

void CAntiAim::Yaw(CUserCmd* pCmd, CBaseEntity* pLocal, float flServerTime, bool& bSendPacket)
{
	if (pServerAnimState == nullptr)
		return;

	const float flMaxDesyncDelta = GetMaxDesyncDelta(pServerAnimState);

	switch (C::Get<int>(Vars.iAntiAimYaw))
	{
	case (int)EAntiAimYawType::NONE:
		G::bIsBreaking = false;
		break;
	case (int)EAntiAimYawType::DESYNC:
	{
		static float flSide = 1.0f;
		float flDesyncTotalFake = C::Get<float>(Vars.flAntiAimDesyncFakeYaw);
		float flDesyncTotalReal = C::Get<float>(Vars.flAntiAimDesyncRealYaw);

		G::bIsBreaking = true;

		/*
		 * manually change the side
		 * @note: to visually seen that - make desync chams by saving matrix or draw direction arrows
		 */
		if (C::Get<int>(Vars.iAntiAimDesyncKey) > 0 && IPT::IsKeyReleased(C::Get<int>(Vars.iAntiAimDesyncKey)))
			flSide = -flSide;

		// desync values
		angSentView.y += C::Get<float>(Vars.flAntiAimDesyncRotation);

		// check is lowerbody updated
		if (flServerTime >= flNextLowerBodyUpdate)
		{
			// check is we not choke now
			if (I::ClientState->nChokedCommands == 0)
				// choke packet to make update invisibly
				bSendPacket = false;

			angSentView.y -= 120 * flSide;
			return;
		}

		if (bSendPacket)
		{
			// real
			angSentView.y += flDesyncTotalReal * flSide;
			G::angRealAngle.y = angSentView.y;
		}
		else
		{
			// fake
			angSentView.y -= flDesyncTotalFake * flSide;
			G::angFakeAngle.y = angSentView.y;
		}

		break;
	}

	case (int)EAntiAimYawType::LEGITDESYNC:
	{
		static float flSide = 1.0f;
		float flDesyncTotalFake = 0.f;
		float flDesyncTotalReal = 0.f;

		G::bIsBreaking = true;

		/*
		 * manually change the side
		 * @note: to visually seen that - make desync chams by saving matrix or draw direction arrows
		 */
		if (C::Get<int>(Vars.iAntiAimDesyncKey) > 0 && IPT::IsKeyReleased(C::Get<int>(Vars.iAntiAimDesyncKey)))
			flSide = -flSide;

		// define the total desync value before break
		flDesyncTotalReal = flMaxDesyncDelta + 40;

		// check is lowerbody updated
		if (flServerTime >= flNextLowerBodyUpdate)
		{
			// check is we not choke now
			if (I::ClientState->nChokedCommands == 0)
				// choke packet to make update invisibly
				bSendPacket = false;

			angSentView.y += 120 * flSide;
			return;
		}

		if (bSendPacket)
		{
			// real
			angSentView.y += flDesyncTotalReal * flSide;
			G::angRealAngle.y = angSentView.y;
		}
		else
			G::angFakeAngle.y = angSentView.y;

		break;
	}
	default:
		break;
	}
}

float CAntiAim::GetMaxDesyncDelta(CBasePlayerAnimState* pAnimState)
{
	float flDuckAmount = pAnimState->flDuckAmount;
	float flRunningSpeed = std::clamp(pAnimState->flRunningSpeed, 0.0f, 1.0f);
	float flDuckingSpeed = std::clamp(pAnimState->flDuckingSpeed, 0.0f, 1.0f);
	float flYawModifier = (((pAnimState->flWalkToRunTransition * -0.3f) - 0.2f) * flRunningSpeed) + 1.0f;

	if (flDuckAmount > 0.0f)
		flYawModifier += ((flDuckAmount * flDuckingSpeed) * (0.5f - flYawModifier));

	float flMaxYawModifier = flYawModifier * pAnimState->flMaxBodyYaw;
	return flMaxYawModifier;
}
