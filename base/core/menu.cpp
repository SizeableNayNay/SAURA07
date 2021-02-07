#include "menu.h"

//		   _____							//
//		  /     \   ____   ____  __ __		//
//		 /  \ /  \_/ __ \ /    \|  |  \		//
//		/    Y    \  ___/|   |  \  |  /		//
//		\____|__  /\___  >___|  /____/		//
//		        \/     \/     \/			//

// used: global variables
#include "../global.h"
// used: config variables
#include "../core/variables.h"
// used: actions with config
#include "../core/config.h"
// used: configurations error logging
#include "../utilities/logging.h"
// used: render setup, etc
#include "../utilities/draw.h"
// used: engine, inputsystem, convar interfaces
#include "../core/interfaces.h"
// used: render visuals
#include "../features/visuals.h"
// used: skinchanger tab items map
#include "../features/skinchanger.h"
// used: inputtext() wrappers for c++ standard library (stl) type: std::string
#include "../../dependencies/imgui/cpp/imgui_stdlib.h"

#pragma region menu_arrays
const std::pair<const char*, std::uint32_t> arrColors[] =
{
	{ "[esp] box - enemies", Vars.colEspMainBoxEnemies },
	{ "[esp] box - enemies wall", Vars.colEspMainBoxEnemiesWall },
	{ "[esp] box - allies", Vars.colEspMainBoxAllies },
	{ "[esp] box - allies wall", Vars.colEspMainBoxAlliesWall },
	{ "[glow] enemies", Vars.colEspGlowEnemies },
	{ "[glow] enemies wall", Vars.colEspGlowEnemiesWall },
	{ "[glow] allies", Vars.colEspGlowAllies },
	{ "[glow] allies wall", Vars.colEspGlowAlliesWall },
	{ "[glow] weapons", Vars.colEspGlowWeapons },
	{ "[glow] grenades", Vars.colEspGlowGrenades },
	{ "[glow] bomb", Vars.colEspGlowBomb },
	{ "[glow] planted bomb", Vars.colEspGlowBombPlanted },
	{ "[chams] enemies", Vars.colEspChamsEnemies },
	{ "[chams] enemies wall", Vars.colEspChamsEnemiesWall },
	{ "[chams] allies", Vars.colEspChamsAllies },
	{ "[chams] allies wall", Vars.colEspChamsAlliesWall },
	{ "[chams] viewmodel", Vars.colEspChamsPlayerViewModel },
	{ "[chams] viewmodel additional", Vars.colEspChamsPlayerViewModelAdditional },
	{ "[screen] hitmarker - lines", Vars.colScreenHitMarker },
	{ "[screen] hitmarker - damage", Vars.colScreenHitMarkerDamage }
};

constexpr std::array<std::string_view, 4U> arrVisualsRemovals =
{
	"post-processing",
	"punch",
	"smoke",
	"scope"
};
#pragma endregion

// spectator list, radar, other stuff here
#pragma region menu_windows
void W::MainWindow(IDirect3DDevice9* pDevice)
{
	ImGuiIO& io = ImGui::GetIO();
	const ImVec2 vecScreenSize = io.DisplaySize;

	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* pForegroundDrawList = ImGui::GetForegroundDrawList();

	#pragma region main_visuals
	if (!I::Engine->IsTakingScreenshot() && !I::Engine->IsDrawingLoadingImage())
	{
		if (C::Get<bool>(Vars.bMenuWatermark))
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.f, 0.f, 0.f, 0.f));

			ImGui::BeginMainMenuBar();
			{
				ImGui::PushFont(F::WhitneySmall);
				ImGui::Dummy(ImVec2(1, 1));

				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr("welcome, naynay"));
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr(" | "));	// Divider

				#ifdef _DEBUG
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr("debug"));
				#endif

				if (strstr(GetCommandLine(), XorStr("-insecure")) != nullptr)
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr(" | "));	// Divider
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr("insecure"));
				}

				if (I::Engine->IsInGame())
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr(" | "));	// Divider
					ImGui::TextColored(G::bSendPacket ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("choked ticks :"));

					char dispsize[100];
					sprintf(dispsize, "%d", G::iChokedTicks);

					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), XorStr(dispsize));
				}

				const char* const szName = XorStr("SAURA07   |    public beta    |    1.0.0.0");
				static ImVec2 vecNameSize = ImGui::CalcTextSize(szName);
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - vecNameSize.x);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), szName);

				ImGui::PopFont();
				ImGui::EndMainMenuBar();
			}

			ImGui::PopStyleColor(3);
		}
	}

	ImDrawList* pBackgroundDrawList = ImGui::GetBackgroundDrawList();
	D::RenderDrawData(pBackgroundDrawList);
	#pragma endregion

	#pragma region main_window
	io.MouseDrawCursor = bMainOpened;

	if (bMainOpened)
	{
		int x, y;
		I::InputSystem->GetCursorPosition(&x, &y);

		// set imgui mouse position
		io.MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));

		ImGui::SetNextWindowPos(ImVec2(vecScreenSize.x * 0.5f, vecScreenSize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.04f, 0.04f, 0.04f, 1.f));

		ImGui::Begin(XorStr("SAURA07"), &bMainOpened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::PopStyleColor();

			const ImVec2 vecPosition = ImGui::GetCursorScreenPos();
			const float flWindowWidth = ImGui::GetWindowWidth();
			ImDrawList* pWindowDrawList = ImGui::GetWindowDrawList();

			// push clip so separator will be drawn at maximal window bounds
			ImGui::PushClipRect(ImVec2(vecPosition.x - 8.f, vecPosition.y - 8.f), ImVec2(vecPosition.x + flWindowWidth - 8.f, vecPosition.y - 6.f), false);

			// restore cliprect
			ImGui::PopClipRect();

			// add tabs
			static std::array<CTab, 4U> const arrTabs =
			{
				CTab{ "rage", &T::RageBot },
				CTab{ "legit", &T::LegitBot },
				CTab{ "visuals", &T::Visuals },
				CTab{ "miscellaneous", &T::Miscellaneous }
				//CTab{ "skinchanger", &T::SkinChanger }
			};

			ImGui::BeginChild(XorStr("menu.tabs"), ImVec2(), true, ImGuiWindowFlags_None);
			{
				T::Render<arrTabs.size()>(XorStr("main_tabs"), arrTabs, &iMainTab, C::Get<ImVec4>(Vars.vecColorHighlights));
				ImGui::EndChild();
			}

			ImGui::End();
		}
	}
	#pragma endregion
}
#pragma endregion

#pragma region menu_tabs
template <std::size_t S>
void T::Render(const char* szTabBar, const std::array<CTab, S> arrTabs, int* nCurrentTab, const ImVec4& colActive, ImGuiTabBarFlags flags)
{
	// is empty check
	if (arrTabs.empty())
		return;

	// set active tab color
	ImGui::PushStyleColor(ImGuiCol_TabActive, colActive);
	if (ImGui::BeginTabBar(szTabBar, flags))
	{
		for (std::size_t i = 0U; i < arrTabs.size(); i++)
		{
			// add tab
			if (ImGui::BeginTabItem(arrTabs.at(i).szName))
			{
				// set current tab index
				*nCurrentTab = i;
				ImGui::EndTabItem();
			}
		}

		// render inner tab
		if (arrTabs.at(*nCurrentTab).pRenderFunction != nullptr)
			arrTabs.at(*nCurrentTab).pRenderFunction();

		ImGui::EndTabBar();
	}
	ImGui::PopStyleColor();
}
#pragma endregion

#pragma region menu_tabs_main
void T::RageBot()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Columns(2, nullptr, false);
	{
		ImGui::BeginChild(XorStr("ragebot.aimbot"), ImVec2(), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("aimbot##ragebot"), &C::Get<bool>(Vars.bRagebot), ImGuiSelectableFlags_None, ImVec2(30, 0));
				ImGui::PopStyleVar();
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));

			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorStr("ragebot.antiaim"), ImVec2(), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("anti-aim"), &C::Get<bool>(Vars.bAntiAim), ImGuiSelectableFlags_None, ImVec2(40, 0));
				ImGui::PopStyleVar();
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));

			if (C::Get<int>(Vars.iAntiAimYaw) > 0)
				ImGui::HotKey(XorStr("desync switch"), &C::Get<int>(Vars.iAntiAimDesyncKey));

			ImGui::Combo(XorStr("pitch"), &C::Get<int>(Vars.iAntiAimPitch), XorStr("none\0up\0down\0zero (untrusted)\0\0"));
			ImGui::Combo(XorStr("yaw"), &C::Get<int>(Vars.iAntiAimYaw), XorStr("none\0desync\0legit desync\0\0"));

			ImGui::Spacing();

			if (C::Get<int>(Vars.iAntiAimYaw) == (int)EAntiAimYawType::DESYNC)
			{
				ImGui::SliderFloat(XorStr("roation"), &C::Get<float>(Vars.flAntiAimDesyncRotation), 0.f, 360.f, "%1.f\u00B0");
				ImGui::SliderFloat(XorStr("fake yaw"), &C::Get<float>(Vars.flAntiAimDesyncFakeYaw), 0.f, 60.f, "%1.f\u00B0");
				ImGui::SliderFloat(XorStr("real yaw"), &C::Get<float>(Vars.flAntiAimDesyncRealYaw), 0.f, 60.f, "%1.f\u00B0");
			}

			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::Columns(1);
}

void T::LegitBot()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Columns(2, nullptr, false);
	{
		ImGui::BeginChild(XorStr("legitbot.aimbot"), ImVec2(), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("aimbot##legitbot"), &C::Get<bool>(Vars.bLegitbot), ImGuiSelectableFlags_None, ImVec2(30, 0));
				ImGui::PopStyleVar();
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));

			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		ImGui::BeginChild(XorStr("legitbot.triggerbot"), ImVec2(), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("triggerbot##master"), &C::Get<bool>(Vars.bTriggerbot), ImGuiSelectableFlags_None, ImVec2(45, 0));
				ImGui::PopStyleVar();

				if (ImGui::BeginMenu(XorStr("filters")))
				{
					ImGui::MenuItem(XorStr("head"), nullptr, &C::Get<bool>(Vars.bTriggerbotHead));
					ImGui::MenuItem(XorStr("chest"), nullptr, &C::Get<bool>(Vars.bTriggerbotChest));
					ImGui::MenuItem(XorStr("stomach"), nullptr, &C::Get<bool>(Vars.bTriggerbotStomach));
					ImGui::MenuItem(XorStr("arms"), nullptr, &C::Get<bool>(Vars.bTriggerbotArms));
					ImGui::MenuItem(XorStr("legs"), nullptr, &C::Get<bool>(Vars.bTriggerbotLegs));
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
			ImGui::HotKey(XorStr("activation key"), &C::Get<int>(Vars.iTriggerbotKey));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::SliderInt(XorStr("reaction delay##trigger"), &C::Get<int>(Vars.iTriggerbotDelay), 0, 500, "%dms");
			ImGui::Checkbox(XorStr("auto wall##trigger"), &C::Get<bool>(Vars.bTriggerbotAutoWall));
			ImGui::SliderInt(XorStr("minimal damage##trigger"), &C::Get<int>(Vars.iTriggerbotMinimalDamage), 1, 100, "%dhp");
			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::Columns(1);
}

void T::Visuals()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Columns(2, nullptr, false);
	{
		ImGui::BeginChild(XorStr("visuals.esp"), ImVec2(0, 0), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("esp##master"), &C::Get<bool>(Vars.bEsp), ImGuiSelectableFlags_None, ImVec2(30, 0));
				ImGui::PopStyleVar();

				ImGui::EndMenuBar();
			}

			static std::array<CTab, 3U> const arrEspTabs =
			{
				CTab{ "main", [&style]()
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
				ImGui::Checkbox(XorStr("enable##main"), &C::Get<bool>(Vars.bEspMain));
				ImGui::Spacing();

				ImGui::Combo(XorStr("type"), &C::Get<int>(Vars.iEspMainInfoType), XorStr("enemies\0allies\0weapons\0grenades\0bomb\0\0"));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (C::Get<int>(Vars.iEspMainInfoType) == 0)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights),XorStr("enemies"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspMainEnemies));
					ImGui::Spacing();

					ImGui::Combo(XorStr("enemy box"), &C::Get<int>(Vars.iEspMainEnemyBox), XorStr("none\0full\0corners\0\0"));
					ImGui::Checkbox(XorStr("far radar"), &C::Get<bool>(Vars.bEspMainEnemyFarRadar));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enemy info"), &C::Get<bool>(Vars.bEspMainEnemyInfo));
					if (C::Get<bool>(Vars.bEspMainEnemyInfo))
					{
						// top of esp box
						// ImGui::Checkbox(XorStr("rank##player"), &C::Get<bool>(Vars.bEspMainAllyRank)); out of order sorry boss
						ImGui::Checkbox(XorStr("name##enemy"), &C::Get<bool>(Vars.bEspMainEnemyName));
						ImGui::Checkbox(XorStr("flash##enemy"), &C::Get<bool>(Vars.bEspMainAllyFlash));
						ImGui::Spacing();

						// left of esp box
						ImGui::Checkbox(XorStr("health##enemy"), &C::Get<bool>(Vars.bEspMainEnemyHealth));
						ImGui::Checkbox(XorStr("money##enemy"), &C::Get<bool>(Vars.bEspMainEnemyMoney));

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						// right of esp box
						ImGui::Checkbox(XorStr("helmet##enemy"), &C::Get<bool>(Vars.bEspMainEnemyHelmet));
						ImGui::Checkbox(XorStr("kevlar##enemy"), &C::Get<bool>(Vars.bEspMainEnemyKevlar));
						ImGui::Checkbox(XorStr("defuser##enemy"), &C::Get<bool>(Vars.bEspMainEnemyDefuseKit));
						ImGui::Checkbox(XorStr("scoped##enemy"), &C::Get<bool>(Vars.bEspMainEnemyZoom));
						ImGui::Spacing();

						// bottom of esp box
						ImGui::Checkbox(XorStr("weapons##enemy"), &C::Get<bool>(Vars.bEspMainEnemyWeapons));
						ImGui::Checkbox(XorStr("ammo##enemy"), &C::Get<bool>(Vars.bEspMainEnemyAmmo));
						ImGui::Checkbox(XorStr("distance##enemy"), &C::Get<bool>(Vars.bEspMainEnemyDistance));
					}
				}

				else if (C::Get<int>(Vars.iEspMainInfoType) == 1)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("allies"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspMainAllies));
					ImGui::Spacing();

					ImGui::Combo(XorStr("ally box"), &C::Get<int>(Vars.iEspMainAllyBox), XorStr("none\0full\0corners\0\0"));
					ImGui::Checkbox(XorStr("far radar"), &C::Get<bool>(Vars.bEspMainAllyFarRadar));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("ally info"), &C::Get<bool>(Vars.bEspMainAllyInfo));
					if (C::Get<bool>(Vars.bEspMainAllyInfo))
					{
						// top of esp box
						// ImGui::Checkbox(XorStr("rank##player"), &C::Get<bool>(Vars.bEspMainAllyRank)); out of order sorry boss
						ImGui::Checkbox(XorStr("name##ally"), &C::Get<bool>(Vars.bEspMainAllyName));
						ImGui::Checkbox(XorStr("flash##player"), &C::Get<bool>(Vars.bEspMainAllyFlash));
						ImGui::Spacing();

						// left of esp box
						ImGui::Checkbox(XorStr("health##ally"), &C::Get<bool>(Vars.bEspMainAllyHealth));
						ImGui::Checkbox(XorStr("money##ally"), &C::Get<bool>(Vars.bEspMainAllyMoney));

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						// right of esp box
						ImGui::Checkbox(XorStr("helmet##ally"), &C::Get<bool>(Vars.bEspMainAllyHelmet));
						ImGui::Checkbox(XorStr("kevlar##ally"), &C::Get<bool>(Vars.bEspMainAllyKevlar));
						ImGui::Checkbox(XorStr("defuser##ally"), &C::Get<bool>(Vars.bEspMainAllyDefuseKit));
						ImGui::Checkbox(XorStr("scoped##ally"), &C::Get<bool>(Vars.bEspMainAllyZoom));
						ImGui::Spacing();

						// bottom of esp box
						ImGui::Checkbox(XorStr("weapons##ally"), &C::Get<bool>(Vars.bEspMainAllyWeapons));
						ImGui::Checkbox(XorStr("ammo##ally"), &C::Get<bool>(Vars.bEspMainAllyAmmo));
						ImGui::Checkbox(XorStr("distance##ally"), &C::Get<bool>(Vars.bEspMainAllyDistance));
					}
				}

				else if (C::Get<int>(Vars.iEspMainInfoType) == 2)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("weapons"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspMainWeapons));
					ImGui::Spacing();

					ImGui::Combo(XorStr("weapon box"), &C::Get<int>(Vars.iEspMainWeaponBox), XorStr("none\0full\0corners\0\0"));

					ImGui::Checkbox(XorStr("weapons info"), &C::Get<bool>(Vars.bEspMainWeaponInfo));
					if (C::Get<bool>(Vars.bEspMainWeaponInfo))
					{
						ImGui::Checkbox(XorStr("icon##weapon"), &C::Get<bool>(Vars.bEspMainWeaponIcon));
						ImGui::Checkbox(XorStr("ammo##weapon"), &C::Get<bool>(Vars.bEspMainWeaponAmmo));
						ImGui::Checkbox(XorStr("distance##weapon"), &C::Get<bool>(Vars.bEspMainWeaponDistance));
					}
				}

				else if (C::Get<int>(Vars.iEspMainInfoType) == 3)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("grenades"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspMainGrenades));
				}

				else if (C::Get<int>(Vars.iEspMainInfoType) == 4)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("bomb"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspMainBomb));
				}

				ImGui::PopStyleVar();
			}},
				CTab{ "glow", [&style]()
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
				ImGui::Checkbox(XorStr("enable##glow"), &C::Get<bool>(Vars.bEspGlow));
				ImGui::Spacing();

				ImGui::Combo(XorStr("type"), &C::Get<int>(Vars.iEspGlowType), XorStr("enemies\0allies\0self\0weapons\0grenades\0bomb\0\0"));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (C::Get<int>(Vars.iEspGlowType) == 0)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("enemies"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowEnemies));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("bloom"), &C::Get<bool>(Vars.bEspGlowBloomEnemies));
				}

				else if (C::Get<int>(Vars.iEspGlowType) == 1)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("allies"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowAllies));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("bloom"), &C::Get<bool>(Vars.bEspGlowBloomAllies));
				}

				else if (C::Get<int>(Vars.iEspGlowType) == 2)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("self"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowSelf));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("bloom"), &C::Get<bool>(Vars.bEspGlowBloomSelf));
				}

				else if (C::Get<int>(Vars.iEspGlowType) == 3)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("weapons"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowWeapons));
				}

				else if (C::Get<int>(Vars.iEspGlowType) == 4)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("grenades"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowGrenades));
				}

				else if (C::Get<int>(Vars.iEspGlowType) == 5)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("bomb"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspGlowBomb));
				}

				// @note: if u rebuild glow and wanna use styles do like that
				//ImGui::Combo(XorStr("styles example##glow"), XorStr("outer\0rim\0edge\0edge pulse\0\0"));

				ImGui::PopStyleVar();
			}},
				CTab{ "chams", [&style]()
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
				ImGui::Checkbox(XorStr("enable##chams"), &C::Get<bool>(Vars.bEspChams));
				ImGui::Spacing();

				ImGui::Combo(XorStr("type"), &C::Get<int>(Vars.iEspChamType), XorStr("enemies\0allies\0self\0\0"));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (C::Get<int>(Vars.iEspChamType) == 0)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("enemies"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspChamsEnemies));
					ImGui::Spacing();

					ImGui::Combo(XorStr("enemies style##chams"), &C::Get<int>(Vars.iEspChamsEnemies), XorStr("covered\0flat\0wireframe\0reflective\0\0"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("xqz"), &C::Get<bool>(Vars.bEspChamsXQZEnemies));
					ImGui::Checkbox(XorStr("disable occlusion"), &C::Get<bool>(Vars.bEspChamsDisableOcclusion));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorStr("may cause \"invisible\" players models sometimes"));
				}

				if (C::Get<int>(Vars.iEspChamType) == 1)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("allies"));
					ImGui::Spacing();

					ImGui::Combo(XorStr("allies style##chams"), &C::Get<int>(Vars.iEspChamsAllies), XorStr("covered\0flat\0wireframe\0reflective\0\0"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspChamsAllies));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("xqz"), &C::Get<bool>(Vars.bEspChamsXQZAllies));
					ImGui::Checkbox(XorStr("disable occlusion"), &C::Get<bool>(Vars.bEspChamsDisableOcclusion));
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip(XorStr("may cause \"invisible\" players models sometimes"));
				}

				if (C::Get<int>(Vars.iEspChamType) == 2)
				{
					ImGui::TextColored(C::Get<ImVec4>(Vars.vecColorHighlights), XorStr("self"));
					ImGui::Spacing();

					ImGui::Checkbox(XorStr("enable"), &C::Get<bool>(Vars.bEspChamsSelf));
					ImGui::Spacing();

					ImGui::Combo(XorStr("player style##chams"), &C::Get<int>(Vars.iEspChamSelfType), XorStr("self\0desync\0fakelag\0viewmodel\0\0"));
					ImGui::Spacing();

					if (C::Get<int>(Vars.iEspChamSelfType) == 0)
					{
						ImGui::Checkbox(XorStr("self"), &C::Get<bool>(Vars.bEspChamsPlayerSelf));
						ImGui::SameLine(0.f, 16.f);
						ImGui::ColorEdit4(XorStr("self color"), &C::Get<Color>(Vars.colEspChamsPlayer), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel);
						ImGui::Spacing();

						ImGui::Combo(XorStr("self style##chams"), &C::Get<int>(Vars.iEspChamsPlayerSelf), XorStr("covered\0flat\0wireframe\0reflective\0\0"));
					}

					else if (C::Get<int>(Vars.iEspChamSelfType) == 1)
					{
						ImGui::Checkbox(XorStr("desync"), &C::Get<bool>(Vars.bEspChamsPlayerDesync));
						ImGui::SameLine(0.f, 16.f);
						ImGui::ColorEdit4(XorStr("desync color"), &C::Get<Color>(Vars.colEspChamsPlayerDesync), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel);
						ImGui::Spacing();

						ImGui::Combo(XorStr("desync style##chams"), &C::Get<int>(Vars.iEspChamsPlayerDesync), XorStr("covered\0flat\0wireframe\0reflective\0\0"));
					}

					else if (C::Get<int>(Vars.iEspChamSelfType) == 2)
					{
						ImGui::Checkbox(XorStr("fakelag"), &C::Get<bool>(Vars.bEspChamsPlayerFakelag));
						ImGui::SameLine(0.f, 16.f);
						ImGui::ColorEdit4(XorStr("fakelag color"), &C::Get<Color>(Vars.colEspChamsPlayerFakelag), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel);
						ImGui::Spacing();

						ImGui::Combo(XorStr("fakelag style##chams"), &C::Get<int>(Vars.iEspChamsPlayerFakelag), XorStr("covered\0flat\0wireframe\0reflective\0\0"));
					}

					else if (C::Get<int>(Vars.iEspChamSelfType) == 3)
					{
						ImGui::Checkbox(XorStr("viewmodel"), &C::Get<bool>(Vars.bEspChamsPlayerViewModel));
						ImGui::SameLine(0.f, 16.f);
						ImGui::ColorEdit4(XorStr("viewmodel color"), &C::Get<Color>(Vars.colEspChamsPlayerViewModel), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoLabel);
						ImGui::Spacing();

						ImGui::Combo(XorStr("viewmodel style##chams"), &C::Get<int>(Vars.iEspChamsPlayerViewModel), XorStr("no draw\0covered\0flat\0wireframe\0glow\0scroll\0chrome\0\0"));
					}
				}

				ImGui::PopStyleVar();
			}}
			};

			T::Render<arrEspTabs.size()>(XorStr("visuals_esp"), arrEspTabs, &iEspTab, style.Colors[ImGuiCol_ScrollbarGrab]);

			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		static float flWorldChildSize = 0.f;
		ImGui::BeginChild(XorStr("visuals.world"), ImVec2(0, flWorldChildSize), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("world##master"), &C::Get<bool>(Vars.bWorld), ImGuiSelectableFlags_None, ImVec2(30, 0));
				ImGui::PopStyleVar();
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
			ImGui::Checkbox(XorStr("night mode"), &C::Get<bool>(Vars.bWorldNightMode));
			ImGui::SliderInt(XorStr("max flash effect"), &C::Get<int>(Vars.iWorldMaxFlash), 0, 100, "%d%%");
			ImGui::MultiCombo(XorStr("removals"), C::Get<std::vector<bool>>(Vars.vecWorldRemovals), arrVisualsRemovals.data(), arrVisualsRemovals.size());

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::HotKey(XorStr("thirdperson"), &C::Get<int>(Vars.iWorldThirdPersonKey));
			ImGui::SliderFloat(XorStr("camera offset"), &C::Get<float>(Vars.flWorldThirdPersonOffset), 50.f, 300.f, "%.1f units");
			ImGui::PopStyleVar();

			flWorldChildSize = ImGui::GetCursorPosY() + style.ItemSpacing.y;
			ImGui::EndChild();
		}

		ImGui::BeginChild(XorStr("visuals.screen"), ImVec2(0, 0), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
				ImGui::Selectable(XorStr("screen##master"), &C::Get<bool>(Vars.bScreen), ImGuiSelectableFlags_None, ImVec2(30, 0));
				ImGui::PopStyleVar();
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));

			/* menu highlights */
			ImGui::ColorEdit4(XorStr("highlights"), &C::Get<Color>(Vars.colColorHighlights), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);

			static ImVec4 vecOldColorHighlights = C::Get<ImVec4>(Vars.vecColorHighlights);
			C::Get<ImVec4>(Vars.vecColorHighlights) = C::Get<Color>(Vars.colColorHighlights).GetVec4(); // store as ImVec4 for later use

			// if highlight colors are changed update draw setup colors
			if (vecOldColorHighlights.x != C::Get<ImVec4>(Vars.vecColorHighlights).x || vecOldColorHighlights.y != C::Get<ImVec4>(Vars.vecColorHighlights).y || vecOldColorHighlights.z != C::Get<ImVec4>(Vars.vecColorHighlights).z)
				D::SetupColors(); // setup colors
			ImGui::Spacing();

			ImGui::SliderFloat(XorStr("camera fov"), &C::Get<float>(Vars.flScreenCameraFOV), -89.f, 89.f, u8"%.1f\u00B0");
			ImGui::SliderFloat(XorStr("viewmodel fov"), &C::Get<float>(Vars.flScreenViewModelFOV), -90.f, 90.f, u8"%.1f\u00B0");

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox(XorStr("hitmarker"), &C::Get<bool>(Vars.bScreenHitMarker));
			ImGui::Checkbox(XorStr("damage"), &C::Get<bool>(Vars.bScreenHitMarkerDamage));
			ImGui::Checkbox(XorStr("sound"), &C::Get<bool>(Vars.bScreenHitMarkerSound));
			ImGui::SliderFloat(XorStr("time"), &C::Get<float>(Vars.flScreenHitMarkerTime), 0.5f, 5.f, "%.1fsec");
			ImGui::SliderInt(XorStr("gap"), &C::Get<int>(Vars.iScreenHitMarkerGap), 1, 20, "%d pixels");
			ImGui::SliderInt(XorStr("length"), &C::Get<int>(Vars.iScreenHitMarkerLenght), 1, 20, "%d pixels");
			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::Columns(1);
}

void T::Miscellaneous()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::Columns(2, nullptr, false);
	{
		static float flMovementChildSize = 0.f;
		ImGui::BeginChild(XorStr("misc.movement"), ImVec2(0, flMovementChildSize), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted(XorStr("movement"));
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
			ImGui::Checkbox(XorStr("bunny hop"), &C::Get<bool>(Vars.bMiscBunnyHop));
			ImGui::SliderInt(XorStr("chance"), &C::Get<int>(Vars.iMiscBunnyHopChance), 0, 100, "%d%%");
			ImGui::Checkbox(XorStr("autostrafe"), &C::Get<bool>(Vars.bMiscAutoStrafe));

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox(XorStr("fake lag"), &C::Get<bool>(Vars.bMiscFakelag));
			ImGui::Checkbox(XorStr("auto accept"), &C::Get<bool>(Vars.bMiscAutoAccept));
			ImGui::Checkbox(XorStr("auto pistol"), &C::Get<bool>(Vars.bMiscAutoPistol));
			ImGui::Checkbox(XorStr("no crouch cooldown"), &C::Get<bool>(Vars.bMiscNoCrouchCooldown));
			ImGui::PopStyleVar();

			flMovementChildSize = ImGui::GetCursorPosY() + style.ItemSpacing.y;
			ImGui::EndChild();
		}

		ImGui::BeginChild(XorStr("misc.exploits"), ImVec2(0, 0), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted(XorStr("exploits"));
				ImGui::EndMenuBar();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
			ImGui::Checkbox(XorStr("ping spike"), &C::Get<bool>(Vars.bMiscPingSpike));
			ImGui::SliderFloat(XorStr("latency factor"), &C::Get<float>(Vars.flMiscLatencyFactor), 0.1f, 1.0f, "%.1f second");
			ImGui::Checkbox(XorStr("reveal ranks"), &C::Get<bool>(Vars.bMiscRevealRanks));
			ImGui::Checkbox(XorStr("unlock inventory"), &C::Get<bool>(Vars.bMiscUnlockInventory));
			ImGui::Checkbox(XorStr("anti-untrusted"), &C::Get<bool>(Vars.bMiscAntiUntrusted));
			ImGui::PopStyleVar();

			ImGui::EndChild();
		}
	}
	ImGui::NextColumn();
	{
		// current selected configuration name
		static std::string szCurrentConfig = { };

		static float flConfigChildSize = 0.f;
		ImGui::BeginChild(XorStr("misc.config"), ImVec2(0, flConfigChildSize), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted(XorStr("configuration"));
				ImGui::EndMenuBar();
			}

			ImGui::Columns(2, XorStr("#CONFIG"), false);
			{
				ImGui::PushItemWidth(-1);

				ImGui::ListBox(XorStr("##config.list"), &iSelectedConfig, [](int nIndex)
					{
						// return current displaying configuration name
						return C::vecFileNames.at(nIndex).c_str();
					}, C::vecFileNames.size(), 5);

				szCurrentConfig = !C::vecFileNames.empty() ? C::vecFileNames.at(iSelectedConfig) : "";
				ImGui::PopItemWidth();
			}
			ImGui::NextColumn();
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, -1));
				ImGui::PushItemWidth(-1);
				if (ImGui::InputTextWithHint(XorStr("##config.file"), XorStr("create new..."), &szConfigFile, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (!C::Save(szConfigFile))
					{
						L::PushConsoleColor(FOREGROUND_RED);
						L::Print(fmt::format(XorStr("[error] failed to create \"{}\" config"), szConfigFile));
						L::PopConsoleColor();
					}

					szConfigFile.clear();
					C::Refresh();
				}
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip(XorStr("press enter to create new configuration"));

				if (ImGui::Button(XorStr("save"), ImVec2(-1, 15)))
				{
					if (!C::Save(szCurrentConfig))
					{
						L::PushConsoleColor(FOREGROUND_RED);
						L::Print(fmt::format(XorStr("[error] failed to save \"{}\" config"), szCurrentConfig));
						L::PopConsoleColor();
					}
				}

				if (ImGui::Button(XorStr("load"), ImVec2(-1, 15)))
				{
					if (!C::Load(szCurrentConfig))
					{
						L::PushConsoleColor(FOREGROUND_RED);
						L::Print(fmt::format(XorStr("[error] failed to load \"{}\" config"), szCurrentConfig));
						L::PopConsoleColor();
					}
				}

				if (ImGui::Button(XorStr("remove"), ImVec2(-1, 15)))
					ImGui::OpenPopup(XorStr("confirmation##config.remove"));

				if (ImGui::Button(XorStr("refresh"), ImVec2(-1, 15)))
					C::Refresh();

				ImGui::PopItemWidth();
				ImGui::PopStyleVar();
			}
			ImGui::Columns(1);

			if (ImGui::BeginPopupModal(XorStr("confirmation##config.remove"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text(XorStr("are you sure you want to remove \"%s\" configuration?"), szCurrentConfig.c_str());
				ImGui::Spacing();

				if (ImGui::Button(XorStr("no"), ImVec2(30, 0)))
					ImGui::CloseCurrentPopup();

				ImGui::SameLine();

				if (ImGui::Button(XorStr("yes"), ImVec2(30, 0)))
				{
					C::Remove(iSelectedConfig);

					// reset current configuration
					iSelectedConfig = 0;

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			flConfigChildSize = ImGui::GetCursorPosY() + style.ItemSpacing.y;
			ImGui::EndChild();
		}

		ImGui::BeginChild(XorStr("misc.colors"), ImVec2(), true, ImGuiWindowFlags_MenuBar);
		{
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted(XorStr("colors"));
				ImGui::EndMenuBar();
			}

			const char* szColorNames[IM_ARRAYSIZE(arrColors)];
			for (int i = 0; i < IM_ARRAYSIZE(arrColors); i++)
				szColorNames[i] = arrColors[i].first;

			ImGui::Spacing();
			ImGui::PushItemWidth(-1);

			ImGui::ListBox(XorStr("##colors.select"), &iSelectedColor, szColorNames, IM_ARRAYSIZE(szColorNames), 4);
			ImGui::ColorEdit4(XorStr("##colors.picker"), &C::Get<Color>(arrColors[iSelectedColor].second), ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
			ImGui::PopItemWidth();

			ImGui::EndChild();
		}
	}
	ImGui::Columns(1);
}

void T::SkinChanger()
{
	ImGui::BeginChild(XorStr("skins"), ImVec2(), true);
	{
		for (const auto& item : mapItemList) //first - itemdefindex, second - skin item struct
		{

		}

		ImGui::EndChild();
	}
}
#pragma endregion
