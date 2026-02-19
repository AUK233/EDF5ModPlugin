#include <windows.h>
#include <string>
#include <vector>
#include <format>
#include <stdexcept>
#include <list>
#include <cstdlib>

#include "Base/g_gameFunc.h"
#include "1DigitProcessor.h"

namespace DigitRenderer {
	PG_SoldierBase pLocalCurrentPlayers[2];
	int bIsSplitScreen;

	PG_SoldierBase* __fastcall GetLocalCurrentPlayersPointer() {
		return pLocalCurrentPlayers;
	}

	int __fastcall GetIsSplitScreen() {
		return bIsSplitScreen;
	}

	void InitializeDigitProcessor() {
		ZeroMemory(&pLocalCurrentPlayers[0], sizeof(16));
		bIsSplitScreen = 0;
	}
// end
}

int __fastcall DigitProcessor_SetLocalCurrentPlayer(PXGS_System_Player pSysPlayer, UINT32 pCount) {
	auto playerID = pSysPlayer->PlayerID;
	if (pCount > 2) return playerID;

	if (Game_IsOnlineMode()){
		DigitRenderer::bIsSplitScreen = 0;
		if(playerID == 0){
			auto pPlayerObject = (PG_SoldierBase)pSysPlayer->pCamera->pGameObject;
			DigitRenderer::pLocalCurrentPlayers[0] = pPlayerObject;
		}
		// end
	}
	else{
		if (pCount == 1) {
			DigitRenderer::bIsSplitScreen = 0;
		} else {
			DigitRenderer::bIsSplitScreen = 1;
		}

		auto pPlayerObject = (PG_SoldierBase)pSysPlayer->pCamera->pGameObject;
		DigitRenderer::pLocalCurrentPlayers[playerID] = pPlayerObject;
	}

	return playerID;
}
