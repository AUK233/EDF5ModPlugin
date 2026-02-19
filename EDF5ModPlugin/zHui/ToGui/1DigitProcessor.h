#pragma once
#include <d3d11.h>
#include "Base/g_system.h"
#include "zEDF/EDFSoldierClass.hpp"

namespace DigitRenderer {
    PG_SoldierBase* __fastcall GetLocalCurrentPlayersPointer();
    int __fastcall GetIsSplitScreen();

    void InitializeDigitProcessor();
}

extern "C" {
    int __fastcall DigitProcessor_SetLocalCurrentPlayer(PXGS_System_Player pSysPlayer, UINT32 pCount);
}

