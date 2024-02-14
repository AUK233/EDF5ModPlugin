#pragma once

typedef struct EDFColor4Struct {
	float r;
	float g;
	float b;
	float a;
} EDFColor4Pointer;

typedef struct EDFVector2Struct {
	float x;
	float y;
} EDFVector2Pointer;

typedef struct EDFVector4Struct {
	float x;
	float y;
	float z;
	float w;
} EDFVector4Pointer;

typedef struct EDFWeaponStruct {
	BYTE pad0[0xD9];
	// When it's 1, weapon will fire
	BYTE fireWeapon;
	// To be verified
	BYTE fireCheck;
	BYTE pad1[5];
	BYTE addr224;
	BYTE pad2[0xBF];
	int reloadType;
	int reloadTime;
	int reloadTime2;
	BYTE pad3[0x18];
	float EnergyChargeRequire;
	float EnergyChargeOriginal;
	float EnergyChargePercent;
	int maxAmmo;
	BYTE pad4[0x3D8];
	// current weapon?
	BYTE addr1452;
	BYTE pad41[0x327];
	// reloading phase, 0 means no
	int reloadPhase;
	BYTE pad42[0x10];
	// current ammunition count
	int curAmmo;
	BYTE pad5[0x254];
	int ROFCount;
	BYTE pad6[0x4C];
	int reloadTimeCount;
	BYTE pad7[0x10];
	float EnergyChargeCount;
	BYTE pad71[0x88];
	INT64 addr3120;
	// maybe
	INT64 reloadFlag;
	BYTE pad8[0x8];
	INT64 addr3144;
	// new value
	BYTE pad9[0x18B0];
	int reloadPadType;
	int extraReloadTime;
	int chargeTime;
	int chargeTimeCount;
	// 1 is fire together,
	int use_extraShotType;
} EDFWeaponPointer;
static_assert(offsetof(EDFWeaponStruct, fireWeapon) == 0xD9);
static_assert(offsetof(EDFWeaponStruct, fireCheck) == 0xDA);
static_assert(offsetof(EDFWeaponStruct, addr224) == 0xE0);
static_assert(offsetof(EDFWeaponStruct, reloadType) == 0x1A0);
static_assert(offsetof(EDFWeaponStruct, reloadTime) == 0x1A4);
static_assert(offsetof(EDFWeaponStruct, reloadTime2) == 0x1A8);
static_assert(offsetof(EDFWeaponStruct, EnergyChargeRequire) == 0x1C4);
static_assert(offsetof(EDFWeaponStruct, maxAmmo) == 0x1D0);
static_assert(offsetof(EDFWeaponStruct, addr1452) == 0x5AC);
static_assert(offsetof(EDFWeaponStruct, reloadPhase) == 0x8D4);
static_assert(offsetof(EDFWeaponStruct, curAmmo) == 0x8E8);
static_assert(offsetof(EDFWeaponStruct, ROFCount) == 0xB40);
static_assert(offsetof(EDFWeaponStruct, reloadTimeCount) == 0xB90);
static_assert(offsetof(EDFWeaponStruct, EnergyChargeCount) == 0xBA4);
static_assert(offsetof(EDFWeaponStruct, reloadFlag) == 0xC38);
static_assert(offsetof(EDFWeaponStruct, addr3144) == 0xC48);
static_assert(offsetof(EDFWeaponStruct, reloadPadType) == 0x2500);
static_assert(offsetof(EDFWeaponStruct, extraReloadTime) == 0x2504);
static_assert(offsetof(EDFWeaponStruct, chargeTime) == 0x2508);
static_assert(offsetof(EDFWeaponStruct, chargeTimeCount) == 0x250C);
static_assert(offsetof(EDFWeaponStruct, use_extraShotType) == 0x2510);

typedef struct HUiHudTextContentStruct {
	BYTE pad1[0x18];
	EDFVector2Pointer fontSize;
	BYTE pad2[0x40];
	WCHAR* text;
	BYTE pad3[0x18];
	size_t textLength;
} HUiHudTextContentPointer;
static_assert(offsetof(HUiHudTextContentStruct, fontSize) == 0x18);
static_assert(offsetof(HUiHudTextContentStruct, text) == 0x60);
static_assert(offsetof(HUiHudTextContentStruct, textLength) == 0x80);

typedef struct HUiHudTextStruct {
	BYTE pad1[0x1A0];
	EDFVector4Pointer RawPos;
	EDFVector4Pointer RawMatrixSize;
	BYTE pad2[0x68];
	void * addr228h;
	BYTE pad3[0x40];
	EDFColor4Pointer font_color;
} HUiHudTextPointer;
static_assert(offsetof(HUiHudTextStruct, RawPos) == 0x1A0);
static_assert(offsetof(HUiHudTextStruct, RawMatrixSize) == 0x1B0);
static_assert(offsetof(HUiHudTextStruct, addr228h) == 0x228);
static_assert(offsetof(HUiHudTextStruct, font_color) == 0x270);

typedef struct HUiHudWeaponStruct {
	BYTE pad1[0x7E8];
	//  vehicle may not have it.
	EDFWeaponPointer* Weapon;
	BYTE pad11[0x228];
	HUiHudTextPointer *TextNumeric;
	void *TextNumericCheck;
	HUiHudTextPointer *TextWingEnergy;
	void *TextWingEnergyCheck;
	HUiHudTextPointer *WingChargeGuage;
	void *WingChargeGuageCheck;
	BYTE pad2[0x1E8];
	// 0 is soldiers
	BYTE IsVehicle;
	// 0 is no?
	BYTE HasVehicleWeaponUI;
	BYTE pad3[0xCE];
	// new
	HUiHudTextPointer *TextNumericType2;
	void *TextNumericType2Check;
	EDFColor4Pointer TextNumericType2Color;
	HUiHudTextPointer *TextDamage;
	void *TextDamageCheck;
	EDFVector2Pointer damageFontSize;
	BYTE pad8[0x8];
	HUiHudTextPointer* TextDamageUP;
	void* TextDamageUPCheck;
	BYTE padEnd[0x10];
} HUiHudWeaponPointer;
static_assert(offsetof(HUiHudWeaponStruct, Weapon) == 0x7E8);
static_assert(offsetof(HUiHudWeaponStruct, TextNumeric) == 0xA18);
static_assert(offsetof(HUiHudWeaponStruct, TextNumericCheck) == 0xA20);
static_assert(offsetof(HUiHudWeaponStruct, TextWingEnergy) == 0xA28);
static_assert(offsetof(HUiHudWeaponStruct, TextWingEnergyCheck) == 0xA30);
static_assert(offsetof(HUiHudWeaponStruct, WingChargeGuage) == 0xA38);
static_assert(offsetof(HUiHudWeaponStruct, WingChargeGuageCheck) == 0xA40);
static_assert(offsetof(HUiHudWeaponStruct, IsVehicle) == 0xC30);
static_assert(offsetof(HUiHudWeaponStruct, HasVehicleWeaponUI) == 0xC31);
static_assert(offsetof(HUiHudWeaponStruct, TextNumericType2) == 0xD00);
static_assert(offsetof(HUiHudWeaponStruct, TextNumericType2Check) == 0xD08);
static_assert(offsetof(HUiHudWeaponStruct, TextNumericType2Color) == 0xD10);
static_assert(offsetof(HUiHudWeaponStruct, TextDamage) == 0xD20);
static_assert(offsetof(HUiHudWeaponStruct, TextDamageCheck) == 0xD28);
static_assert(offsetof(HUiHudWeaponStruct, damageFontSize) == 0xD30);
static_assert(offsetof(HUiHudWeaponStruct, TextDamageUP) == 0xD40);
static_assert(offsetof(HUiHudWeaponStruct, TextDamageUPCheck) == 0xD48);
