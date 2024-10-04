#pragma once


typedef struct {
	WCHAR* text;
	int id;
	int size;
} SubtitleTextStruct;

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
	// 140 is custom_parameter
	// 6e8 is Ammo_CustomParameter
	BYTE pad0[0xC0];
	void* pPlayer;
	BYTE pad0111[0x11];
	// When it's 1, weapon will fire
	BYTE fireWeapon;
	// To be verified
	BYTE fireCheck;
	// When it's 1, use SecondaryFire
	BYTE secondaryFire;
	BYTE pad1[4];
	BYTE addr224;
	BYTE pad2[0x67];
	WCHAR* WeaponName;
	BYTE pad2111[0x50];
	int reloadType;
	int reloadTime;
	int reloadTime2;
	BYTE pad3[0x18];
	float EnergyChargeRequire;
	float EnergyChargeOriginal;
	float EnergyChargePercent;
	int maxAmmo;
	BYTE pad4[0x11C];
	int FireSpreadType;
	float FireSpreadWidth;
	int FireCount;
	BYTE pad401[0x204];
	int SecondaryFire_Type;
	BYTE pad402[4];
	void* SecondaryFire_Parameter;
	int LockonType;
	int LockonTargetType;
	int Lockon_DistributionType;
	BYTE Lockon_AutoTimeOut;
	BYTE Lockon_FireEndToClear;
	BYTE pad403[2];
	EDFVector4Pointer LockonAngle;
	float LockonRange;
	// int to float
	float LockonTime;
	// int to float
	float LockonHoldTime;
	BYTE pad404[4];
	int LockonFailedTime;
	BYTE pad405[0x68];
	// current weapon?
	BYTE addr1452;
	BYTE pad43[0xEC];
	float AmmoDamage;
	float AmmoDamageReduceMin;
	float AmmoDamageReduceFactor;
	float AmmoExplosion;
	// 0 is no, 1 is has
	BYTE AmmoIsPenetration;
	// 0 is none, 1 is has
	BYTE AmmoIsFriendlyFire;
	// 0 is has, 1 is none
	BYTE AmmoIsFriendlyNonCollision;
	BYTE pad41[4];
	float AmmoSize;
	float AmmoHitSizeAdjust;
	float AmmoHitImpulseAdjust;
	BYTE pad4102[0x10];
	EDFColor4Pointer AmmoColor;
	BYTE pad42512[0x4];
	void* Ammo_CustomParameter;
	BYTE pad421[0x1E4];
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
	// 0 is none, 1 is has
	int AmmoFriendlyFireType;
} EDFWeaponPointer;
static_assert(offsetof(EDFWeaponStruct, pPlayer) == 0xC0);
static_assert(offsetof(EDFWeaponStruct, fireWeapon) == 0xD9);
static_assert(offsetof(EDFWeaponStruct, fireCheck) == 0xDA);
static_assert(offsetof(EDFWeaponStruct, secondaryFire) == 0xDB);
static_assert(offsetof(EDFWeaponStruct, addr224) == 0xE0);
static_assert(offsetof(EDFWeaponStruct, WeaponName) == 0x148);
static_assert(offsetof(EDFWeaponStruct, reloadType) == 0x1A0);
static_assert(offsetof(EDFWeaponStruct, reloadTime) == 0x1A4);
static_assert(offsetof(EDFWeaponStruct, reloadTime2) == 0x1A8);
static_assert(offsetof(EDFWeaponStruct, EnergyChargeRequire) == 0x1C4);
static_assert(offsetof(EDFWeaponStruct, maxAmmo) == 0x1D0);
static_assert(offsetof(EDFWeaponStruct, FireSpreadType) == 0x2F0);
static_assert(offsetof(EDFWeaponStruct, FireSpreadWidth) == 0x2F4);
static_assert(offsetof(EDFWeaponStruct, FireCount) == 0x2F8);
static_assert(offsetof(EDFWeaponStruct, SecondaryFire_Type) == 0x500);
static_assert(offsetof(EDFWeaponStruct, SecondaryFire_Parameter) == 0x508);
static_assert(offsetof(EDFWeaponStruct, LockonType) == 0x510);
static_assert(offsetof(EDFWeaponStruct, LockonTargetType) == 0x514);
static_assert(offsetof(EDFWeaponStruct, Lockon_DistributionType) == 0x518);
static_assert(offsetof(EDFWeaponStruct, Lockon_AutoTimeOut) == 0x51C);
static_assert(offsetof(EDFWeaponStruct, Lockon_FireEndToClear) == 0x51D);
static_assert(offsetof(EDFWeaponStruct, LockonAngle) == 0x520);
static_assert(offsetof(EDFWeaponStruct, LockonRange) == 0x530);
static_assert(offsetof(EDFWeaponStruct, LockonTime) == 0x534);
static_assert(offsetof(EDFWeaponStruct, LockonHoldTime) == 0x538);
static_assert(offsetof(EDFWeaponStruct, LockonFailedTime) == 0x540);
static_assert(offsetof(EDFWeaponStruct, addr1452) == 0x5AC);
static_assert(offsetof(EDFWeaponStruct, AmmoDamage) == 0x69C);
static_assert(offsetof(EDFWeaponStruct, AmmoDamageReduceMin) == 0x6A0);
static_assert(offsetof(EDFWeaponStruct, AmmoDamageReduceFactor) == 0x6A4);
static_assert(offsetof(EDFWeaponStruct, AmmoExplosion) == 0x6A8);
static_assert(offsetof(EDFWeaponStruct, AmmoIsPenetration) == 0x6AC);
static_assert(offsetof(EDFWeaponStruct, AmmoIsFriendlyFire) == 0x6AD);
static_assert(offsetof(EDFWeaponStruct, AmmoIsFriendlyNonCollision) == 0x6AE);
static_assert(offsetof(EDFWeaponStruct, AmmoSize) == 0x6B4);
static_assert(offsetof(EDFWeaponStruct, AmmoHitSizeAdjust) == 0x6B8);
static_assert(offsetof(EDFWeaponStruct, AmmoHitImpulseAdjust) == 0x6BC);
static_assert(offsetof(EDFWeaponStruct, AmmoColor) == 0x6D0);
static_assert(offsetof(EDFWeaponStruct, Ammo_CustomParameter) == 0x6E8);
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
static_assert(offsetof(EDFWeaponStruct, AmmoFriendlyFireType) == 0x2514);

// +1BA0
typedef struct FencerBoostAndDashStruct {
	int BoostRecoveryTime;
	int BoostMaxCount;
	int BoostCurrentCount;
	float BoostSpeed;
	int DashRecoveryTime;
	float DashInterval;
	int DashMaxCount;
	int DashCurrentCount;
} FencerBoostAndDashPointer;

typedef struct HUiHudTextContentStruct {
	BYTE pad1[0x18];
	EDFVector2Pointer fontSize;
	BYTE pad2[0x40];
	// maybe is wstring
	//std::wstring text;
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

typedef struct HUiHudPowerGuageStruct {
	BYTE pad1[0x7E8];
	//  vehicle may not have it.
	EDFWeaponPointer* Weapon;
	BYTE pad11[0x130];
	// aka Current HP
	HUiHudTextPointer* PowText;
	void* PowTextCheck;
	// aka Max HP
	HUiHudTextPointer* DefText;
	void* DefTextCheck;
	BYTE pad2[0x2C0];
	// new
	HUiHudTextPointer* TextFencerDash;
	void* TextFencerDashCheck;
	EDFColor4Pointer TextFencerDashColor;
	HUiHudTextPointer* TextFencerBoost;
	void* TextFencerBoostCheck;
	EDFColor4Pointer TextFencerBoostColor;
	BYTE padEnd[0x10];
} HUiHudPowerGuagePointer;
static_assert(offsetof(HUiHudPowerGuageStruct, Weapon) == 0x7E8);
static_assert(offsetof(HUiHudPowerGuageStruct, PowText) == 0x920);
static_assert(offsetof(HUiHudPowerGuageStruct, PowTextCheck) == 0x928);
static_assert(offsetof(HUiHudPowerGuageStruct, DefText) == 0x930);
static_assert(offsetof(HUiHudPowerGuageStruct, DefTextCheck) == 0x938);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerDash) == 0xC00);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerDashCheck) == 0xC08);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerDashColor) == 0xC10);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerBoost) == 0xC20);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerBoostCheck) == 0xC28);
static_assert(offsetof(HUiHudPowerGuageStruct, TextFencerBoostColor) == 0xC30);
