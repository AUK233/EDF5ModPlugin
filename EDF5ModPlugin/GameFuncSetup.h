#pragma once

// here hook all changed functions
void hookGameFunctions();
// here is the part overwritten with hex
void OverwriteGameFunctions();
// new functions require more memory
void ReallocateWeaponMemory();
