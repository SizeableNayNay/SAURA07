#pragma once
// used: define to add values to variables vector
#include "config.h"

//		____   ____            .__      ___.   .__                 
//		\   \ /   /____ _______|__|____ \_ |__ |  |   ____   ______		//
//		 \   Y   /\__  \\_  __ \  \__  \ | __ \|  | _/ __ \ /  ___/		//
//		  \     /  / __ \|  | \/  |/ __ \| \_\ \  |_\  ___/ \___ \		//
//		   \___/  (____  /__|  |__(____  /___  /____/\___  >____  >		//
//		               \/              \/    \/          \/     \/		//

#pragma region variables_array_entries
/* multicombo lists */
// esp removals 
enum EVisualsRemovals : int
{
	REMOVAL_POSTPROCESSING = 0,
	REMOVAL_PUNCH,
	REMOVAL_SMOKE,
	REMOVAL_SCOPE,
	REMOVAL_MAX
};
#pragma endregion

#pragma region variables_combo_entries
/* singlecombo / combo lists */

// antiaim pitch
enum class EAntiAimPitchType : int
{
	NONE = 0,
	UP,
	DOWN,
	ZERO
};

// antiaim yaw
enum class EAntiAimYawType : int
{
	NONE = 0,
	DESYNC,
	LEGITDESYNC
};

// esp box shape
enum class EVisualsBoxType : int
{
	NONE = 0,
	FULL,
	CORNERS
};

// esp glow type
enum class EVisualsGlowStyle : int
{
	OUTER = 0,
	COVER,
	INNER
};

// esp player cham type
enum class EVisualsPlayersChams : int
{
	COVERED = 0,
	FLAT,
	WIREFRAME,
	REFLECTIVE
};

// esp viewmodel cham type
enum class EVisualsPlayerViewModelChams : int
{
	NO_DRAW = 0,
	COVERED,
	FLAT,
	WIREFRAME,
	GLOW,
	SCROLL,
	CHROME
};

/* esp combos */
// esp info
enum class EVisualsMenuInfoType : int
{
	ENEMIES = 0,
	ALLIES,
	WEAPONS,
	GRENADES,
	BOMB
};

// esp glow
enum class EVisualsMenuGlowType : int
{
	ENEMIES = 0,
	ALLIES,
	SELF,
	WEAPONS,
	GRENADES,
	BOMB
};

// esp chams
enum class EVisualsMenuChamType : int
{
	ENEMIES = 0,
	ALLIES,
	SELF
};

// esp self chams type
enum class EVisualsMenuChamSelfType : int
{
	SELF = 0,
	DESYNC,
	FAKELAG,
	VIEWMODEL
};

#pragma endregion

/* utility variables */
struct Variables_t
{
	/* ragebot vars */
	#pragma region variables_ragebot
	C_ADD_VARIABLE(bool, bRagebot, false); // turn ragebot on

	#pragma endregion



	/* antiaim vars */
	#pragma region variables_ragebot
	C_ADD_VARIABLE(bool, bAntiAim, false); // turn anitaim on
	C_ADD_VARIABLE(int, iAntiAimDesyncKey, VK_XBUTTON1); // key to switch antiaim side

	C_ADD_VARIABLE(int, iAntiAimPitch, 0); // antiaim pitch type
	C_ADD_VARIABLE(int, iAntiAimYaw, 0); // antiaim type

	C_ADD_VARIABLE(bool, bAntiAimCustomDesync, false); // alter desync values?
	C_ADD_VARIABLE(float, flAntiAimDesyncRotation, 180.f); // rotation of antiaim
	C_ADD_VARIABLE(float, flAntiAimDesyncFakeYaw, 60.f); // antiaim fake yaw
	C_ADD_VARIABLE(float, flAntiAimDesyncRealYaw, 60.f); // antiaim real yaw
	#pragma endregion



	/* legitbot vars */
	#pragma region variables_legitbot
	C_ADD_VARIABLE(bool, bLegitbot, false); // turn legitbot on

	/* triggerbot vars */
	C_ADD_VARIABLE(bool, bTriggerbot, false); // turn triggerbot on
	C_ADD_VARIABLE(int, iTriggerbotKey, 0); // key to enable triggerbot

	C_ADD_VARIABLE(int, iTriggerbotDelay, 0); // time to delay triggerbot
	C_ADD_VARIABLE(bool, bTriggerbotAutoWall, false); // turn on tiggerbot through walls
	C_ADD_VARIABLE(int, iTriggerbotMinimalDamage, 70); // minimum enemy hp for triggerbot

	/* triggerbot hitboxes */
	C_ADD_VARIABLE(bool, bTriggerbotHead, true); // activate on head
	C_ADD_VARIABLE(bool, bTriggerbotChest, true); // activate on chest
	C_ADD_VARIABLE(bool, bTriggerbotStomach, true); // activate on stomach
	C_ADD_VARIABLE(bool, bTriggerbotArms, false); // activate on arms
	C_ADD_VARIABLE(bool, bTriggerbotLegs, false); // activate on legs
	#pragma endregion



	/* esp vars */
	#pragma region variables_visuals
	C_ADD_VARIABLE(bool, bEsp, false); // turn visuals on

	C_ADD_VARIABLE(bool, bEspMain, false); // turn esp on

	/* esp players */
	/* esp boxes */
	C_ADD_VARIABLE(bool, bEspMainEnemies, false); // turn grenade esp on
	C_ADD_VARIABLE(bool, bEspMainAllies, false); // turn grenade esp on

	C_ADD_VARIABLE(int, iEspMainEnemyBox, static_cast<int>(EVisualsBoxType::NONE)); // esp box type enemy
	C_ADD_VARIABLE(int, iEspMainAllyBox, static_cast<int>(EVisualsBoxType::NONE)); // esp box type ally

	C_ADD_VARIABLE(Color, colEspMainBoxEnemies, Color(255, 255, 255, 255));			// enemies 
	C_ADD_VARIABLE(Color, colEspMainBoxEnemiesWall, Color(255, 255, 255, 255));		// enemies wall

	C_ADD_VARIABLE(Color, colEspMainBoxAllies, Color(255, 255, 255, 255));			// allies
	C_ADD_VARIABLE(Color, colEspMainBoxAlliesWall, Color(255, 255, 255, 255));		// allies wall

	/* esp player flags */
	C_ADD_VARIABLE(bool, bEspMainEnemyInfo, false); // show player info on/off enemy
	C_ADD_VARIABLE(bool, bEspMainAllyInfo, false); // show player info on/off ally

	C_ADD_VARIABLE(int, iEspMainInfoType, static_cast<int>(EVisualsMenuInfoType::ENEMIES)); // info type

	// top of esp box
	// allies
	C_ADD_VARIABLE(bool, bEspMainAllyName, true); // show player name
	C_ADD_VARIABLE(bool, bEspMainAllyRank, false); // show player rank
	C_ADD_VARIABLE(bool, bEspMainAllyFlash, false); // show player is flashed

	// enemies
	C_ADD_VARIABLE(bool, bEspMainEnemyName, true); // show player name
	C_ADD_VARIABLE(bool, bEspMainEnemyRank, false); // show player rank
	C_ADD_VARIABLE(bool, bEspMainEnemyFlash, false); // show player is flashed

	// right of esp box
	// enemies
	C_ADD_VARIABLE(bool, bEspMainEnemyHelmet, true); // show player helmet
	C_ADD_VARIABLE(bool, bEspMainEnemyKevlar, false); // show player kevlar
	C_ADD_VARIABLE(bool, bEspMainEnemyDefuseKit, false); // show player defuser
	C_ADD_VARIABLE(bool, bEspMainEnemyZoom, false); // show player zoom

	// allies
	C_ADD_VARIABLE(bool, bEspMainAllyHelmet, true); // show player helmet
	C_ADD_VARIABLE(bool, bEspMainAllyKevlar, false); // show player kevlar
	C_ADD_VARIABLE(bool, bEspMainAllyDefuseKit, false); // show player defuser
	C_ADD_VARIABLE(bool, bEspMainAllyZoom, false); // show player zoom

	// left of esp box
	C_ADD_VARIABLE(bool, bEspMainAllyHealth, true); // show player health
	C_ADD_VARIABLE(bool, bEspMainAllyMoney, false); // show player money

	C_ADD_VARIABLE(bool, bEspMainEnemyHealth, true); // show player health
	C_ADD_VARIABLE(bool, bEspMainEnemyMoney, false); // show player money

	// bottom of esp box
	// enemies
	C_ADD_VARIABLE(bool, bEspMainEnemyWeapons, true); // show player weapons
	C_ADD_VARIABLE(bool, bEspMainEnemyAmmo, true); // show player ammo
	C_ADD_VARIABLE(bool, bEspMainEnemyDistance, false); // show player distance

	// allies
	C_ADD_VARIABLE(bool, bEspMainAllyWeapons, true); // show player weapons
	C_ADD_VARIABLE(bool, bEspMainAllyAmmo, true); // show player ammo
	C_ADD_VARIABLE(bool, bEspMainAllyDistance, false); // show player distance

	/* esp players misc */
	C_ADD_VARIABLE(bool, bEspMainEnemyFarRadar, false); // show player on radar enemy
	C_ADD_VARIABLE(bool, bEspMainAllyFarRadar, false); // show player on radar ally



	/* esp equipment */
	/* esp weapons */
	C_ADD_VARIABLE(bool, bEspMainWeapons, false); // turn weapon esp on

	C_ADD_VARIABLE(int, iEspMainWeaponBox, static_cast<int>(EVisualsBoxType::NONE)); // weapon esp box type
	C_ADD_VARIABLE(Color, colEspMainBoxWeapons, Color(255, 255, 255, 155)); // color of esp box

	C_ADD_VARIABLE(bool, bEspMainWeaponInfo, false); // enable weapon info

	C_ADD_VARIABLE(bool, bEspMainWeaponIcon, true); // show weapon icon
	C_ADD_VARIABLE(bool, bEspMainWeaponAmmo, true); // show weapon ammo bar
	C_ADD_VARIABLE(bool, bEspMainWeaponDistance, false); // show distance to weapon

	/* esp grenades */
	C_ADD_VARIABLE(bool, bEspMainGrenades, false); // turn grenade esp on

	/* esp bomb */
	C_ADD_VARIABLE(bool, bEspMainBomb, false); // turn bomb esp on



	/* esp glow system */
	C_ADD_VARIABLE(bool, bEspGlow, false); // enable glow system

	C_ADD_VARIABLE(int, iEspGlowType, static_cast<int>(EVisualsMenuGlowType::ENEMIES)); // glow type

	C_ADD_VARIABLE(bool, bEspGlowEnemies, false); // enable glow for enemies
	C_ADD_VARIABLE(bool, bEspGlowAllies, false); // enable glow for allies
	C_ADD_VARIABLE(bool, bEspGlowSelf, false); // enable glow for allies
	C_ADD_VARIABLE(bool, bEspGlowWeapons, false); // enable glow for weapons
	C_ADD_VARIABLE(bool, bEspGlowGrenades, false); // enable glow for grenades
	C_ADD_VARIABLE(bool, bEspGlowBomb, false); // enable glow for bomb

	C_ADD_VARIABLE(bool, bEspGlowBloomEnemies, false); // enable glow bloom enemies
	C_ADD_VARIABLE(bool, bEspGlowBloomAllies, false); // enable glow bloom allies
	C_ADD_VARIABLE(bool, bEspGlowBloomSelf, false); // enable glow bloom self

	/* glow colors */
	C_ADD_VARIABLE(Color, colEspGlowEnemies, Color(252, 3, 140, 128));			// enemies
	C_ADD_VARIABLE(Color, colEspGlowEnemiesWall, Color(252, 3, 140, 128));		// enemies wall

	C_ADD_VARIABLE(Color, colEspGlowAllies, Color(252, 207, 3, 128));			// allies
	C_ADD_VARIABLE(Color, colEspGlowAlliesWall, Color(252, 207, 3, 128));		// allies wall

	C_ADD_VARIABLE(Color, colEspGlowSelf, Color(252, 3, 140, 128));				// self
	C_ADD_VARIABLE(Color, colEspGlowSelfWall, Color(252, 3, 140, 128));			// self wall

	C_ADD_VARIABLE(Color, colEspGlowWeapons, Color(3, 252, 115, 128));			// weapons
	C_ADD_VARIABLE(Color, colEspGlowGrenades, Color(252, 36, 3, 128));			// grenades

	C_ADD_VARIABLE(Color, colEspGlowBomb, Color(3, 252, 115, 128));				// bomb
	C_ADD_VARIABLE(Color, colEspGlowBombPlanted, Color(252, 36, 3, 128));		// bomb planted



	/* esp chams */
	C_ADD_VARIABLE(bool, bEspChams, false); // enable chams

	C_ADD_VARIABLE(int, iEspChamType, static_cast<int>(EVisualsMenuChamType::ENEMIES)); // cham type

	C_ADD_VARIABLE(int, iEspChamSelfType, static_cast<int>(EVisualsMenuChamSelfType::SELF)); // cham type

	/* chams main */
	C_ADD_VARIABLE(bool, bEspChamsEnemies, false); // enable enemy chams
	C_ADD_VARIABLE(bool, bEspChamsAllies, false); // enable ally chams
	C_ADD_VARIABLE(bool, bEspChamsSelf, false); // enable self chams

	/* self */
	C_ADD_VARIABLE(bool, bEspChamsPlayerSelf, false); // enable self chams
	C_ADD_VARIABLE(bool, bEspChamsPlayerDesync, false); // enable desync chams
	C_ADD_VARIABLE(bool, bEspChamsPlayerFakelag, false); // enable fkaelag chams
	C_ADD_VARIABLE(bool, bEspChamsPlayerViewModel, false); // enable view model chams

	/* players */
	C_ADD_VARIABLE(bool, bEspChamsDisableOcclusion, false); // disable obstructions
	// enemies
	C_ADD_VARIABLE(bool, bEspChamsXQZEnemies, false); // enable chams through walls

	// allies
	C_ADD_VARIABLE(bool, bEspChamsXQZAllies, false); // enable chams through walls

	/* cham type */
	C_ADD_VARIABLE(int, iEspChamsEnemies, static_cast<int>(EVisualsPlayersChams::COVERED)); // enemy cham type
	C_ADD_VARIABLE(int, iEspChamsAllies, static_cast<int>(EVisualsPlayersChams::COVERED)); // ally cham type
	C_ADD_VARIABLE(int, iEspChamsPlayerSelf, static_cast<int>(EVisualsPlayersChams::COVERED)); // player cham type
	C_ADD_VARIABLE(int, iEspChamsPlayerDesync, static_cast<int>(EVisualsPlayersChams::COVERED)); // desync cham type
	C_ADD_VARIABLE(int, iEspChamsPlayerFakelag, static_cast<int>(EVisualsPlayersChams::COVERED)); // fakelag cham type
	C_ADD_VARIABLE(int, iEspChamsPlayerViewModel, static_cast<int>(EVisualsPlayerViewModelChams::COVERED)); // view model cham type

	/* cham colors */
	C_ADD_VARIABLE(Color, colEspChamsEnemies, Color(157, 252, 3, 255));				// enemies
	C_ADD_VARIABLE(Color, colEspChamsEnemiesWall, Color(20, 20, 20, 255));			// enemies wall

	C_ADD_VARIABLE(Color, colEspChamsAllies, Color(3, 161, 252, 255));				// allies
	C_ADD_VARIABLE(Color, colEspChamsAlliesWall, Color(235, 235, 235, 255));		// allies wall

	C_ADD_VARIABLE(Color, colEspChamsPlayer, Color(173, 3, 252, 255));				// player
	C_ADD_VARIABLE(Color, colEspChamsPlayerDesync, Color(157, 252, 3, 128));	// player desync
	C_ADD_VARIABLE(Color, colEspChamsPlayerFakelag, Color(252, 219, 3, 128));		// player fakelag

	C_ADD_VARIABLE(Color, colEspChamsPlayerViewModel, Color(173, 3, 252, 255));			// view model
	C_ADD_VARIABLE(Color, colEspChamsPlayerViewModelAdditional, Color(157, 252, 3, 255));	// view model additional

	/* esp world */
	C_ADD_VARIABLE(bool, bWorld, false); // enable world esp

	C_ADD_VARIABLE(bool, bWorldNightMode, false); // enable night mode
	C_ADD_VARIABLE(int, iWorldMaxFlash, 100); // max flash effect slider

	C_ADD_VARIABLE(int, iWorldThirdPersonKey, 0); // thirdperson toggle key
	C_ADD_VARIABLE(float, flWorldThirdPersonOffset, 150.f); // thirdperson camera offset

	C_ADD_VARIABLE_VECTOR(bool, REMOVAL_MAX, vecWorldRemovals, false); // visual removals

	/* esp on-screen */
	C_ADD_VARIABLE(bool, bScreen, false); // enable screen esp

	C_ADD_VARIABLE(float, flScreenCameraFOV, 0.f); // change view FOV
	C_ADD_VARIABLE(float, flScreenViewModelFOV, 0.f); // change view model FOV

	/* hitmarker */
	C_ADD_VARIABLE(bool, bScreenHitMarker, false); // enable on-screen hitmarker
	C_ADD_VARIABLE(bool, bScreenHitMarkerDamage, false); // enable damage indicator
	C_ADD_VARIABLE(bool, bScreenHitMarkerSound, false); // enable sound on hit

	// hitmarker vars
	C_ADD_VARIABLE(float, flScreenHitMarkerTime, 1.0f); // duration of hitmarker
	C_ADD_VARIABLE(int, iScreenHitMarkerGap, 5); // hitmaker gap
	C_ADD_VARIABLE(int, iScreenHitMarkerLenght, 10); // hitmarker length

	// hitmarker colors
	C_ADD_VARIABLE(Color, colScreenHitMarker, Color(255, 255, 255, 255)); // hitmarker color	
	C_ADD_VARIABLE(Color, colScreenHitMarkerDamage, Color(200, 55, 20, 255)); // damage indicator color
	#pragma endregion



	/* misc vars */
	#pragma region variables_misc
	/* movement vars */
	C_ADD_VARIABLE(bool, bMiscBunnyHop, false); // enable auto bunny hop
	C_ADD_VARIABLE(int, iMiscBunnyHopChance, 100); // change hitchance of each auto bunny hop
	C_ADD_VARIABLE(bool, bMiscAutoStrafe, false); // enable auto straffing

	/* misc */
	C_ADD_VARIABLE(bool, bMiscFakelag, false); // enable fakelag
	C_ADD_VARIABLE(bool, bMiscAutoAccept, false); // enable auto match accept
	C_ADD_VARIABLE(bool, bMiscAutoPistol, false); // enable auto pistol
	C_ADD_VARIABLE(bool, bMiscNoCrouchCooldown, false); // disable timed crouch limitations

	/* exploit vars */
	C_ADD_VARIABLE(bool, bMiscPingSpike, false); // enable fake ping
	C_ADD_VARIABLE(float, flMiscLatencyFactor, 0.4f); // fake ping amount

	C_ADD_VARIABLE(bool, bMiscRevealRanks, false); // show player ranks on tab
	C_ADD_VARIABLE(bool, bMiscUnlockInventory, false); // enable in-match inventory
	C_ADD_VARIABLE(bool, bMiscAntiUntrusted, true); // limit vac untrusted commands (recommended on)
	#pragma endregion



	/* misc menu vars */
	#pragma region variables_menu
	// menu keybinds
	C_ADD_VARIABLE(int, iMenuKey, VK_INSERT); // open menu key
	C_ADD_VARIABLE(int, iPanicKey, VK_END); // end utility key

	// menu misc
	C_ADD_VARIABLE(bool, bMenuWatermark, true); // limit vac untrusted commands (recommended on)

	// menu colors
	C_ADD_VARIABLE(Color, colColorHighlights, Color(183, 30, 63, 255)); // main menu color
	C_ADD_VARIABLE(ImVec4, vecColorHighlights, ImVec4(0.60f, 0.09f, 0.21f, 1.00f));
	#pragma endregion
};

inline Variables_t Vars; // define struct as Vars for reference
