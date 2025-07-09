#pragma once

constexpr UINT32 AcidBullet01Size = 0x650;
constexpr UINT32 FlameBullet01Size = 0x8D0;
constexpr UINT32 FlameBullet02Size = 0x940;
constexpr UINT32 PlasmaBullet01Size = 0xB00;
constexpr UINT32 SolidBullet01Size = 0x710;
constexpr UINT32 SolidExpBullet01Size = 0x6F0;
constexpr UINT32 SpiderStringBullet01Size = 0x770;
constexpr UINT32 SpiderStringBullet02Size = 0x710;
constexpr UINT32 PulseBullet01Size = 0x710;
constexpr UINT32 LaserBullet01Size = 0x750;
constexpr UINT32 LaserBullet02Size = 0xC90;
constexpr UINT32 HomingLaserBullet01Size = 0x8C0;
constexpr UINT32 LightningBullet01Size = 0x880;
constexpr UINT32 RocketBullet01Size = 0xA70;
constexpr UINT32 BeamBullet01Size = 0x710;
constexpr UINT32 NeedleBullet01Size = 0x760;

extern "C" {
float _Common_F0P001 = 0.001f;
float _Common_F0P004 = 0.004f;
float _Common_F0P005 = 0.005f;
float _Common_F0P015 = 0.015f;
float _Common_F0P01 = 0.01f;
float _Common_F0P025 = 0.025f;
float _Common_F0P05 = 0.05f;
float _Common_F0P1 = 0.1f;
float _Common_F0P2 = 0.2f;
float _Common_F0P3 = 0.3f;
float _Common_F0P5 = 0.5f;
float _Common_F0P8 = 0.8f;
float _Common_F0P9 = 0.9f;
float _Common_F1P0 = 1.0f;
float _Common_F1P1 = 1.1f;
float _Common_PI2 = 1.5708f;
float _Common_F1P58 = 1.58f;
float _Common_PI = 3.1415926f;
float _Common_F5P0 = 5.0f;
float _Common_2PI = 6.28319f;
float _Common_F8P0 = 8.0f;
float _Common_F10P0 = 10.0f;
float _Common_F25P0 = 25.0f; 
float _Common_F60P0 = 60.0f; 
float _Common_F70P0 = 70.0f; 
float _Common_F3600P0 = 3600.0f;
float _Common_F0P025N = -0.025f;
float _Common_F0P75N = -0.75f;
float _Common_F1P0N = -1.0f;
float _Common_NPI2 = -1.5708f;
float _Common_F1P58N = -1.58f;

__declspec(align(16)) float _CommonRVA_EE7700[4] = { 0.001f, 0.001f, 0.001f, 1.0f};
__declspec(align(16)) float _CommonRVA_EE80D0[4] = { -1.0f, -1.0f, -1.0f, -1.0f };
__declspec(align(16)) float _CommonRVA_EE7350[4] = { 1.0f, 1.0f, 1.0f, 0.5f };
__declspec(align(16)) float _CommonRVA_EE6BE0[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
// Maybe it needs to be fixed.
__declspec(align(16)) float _CommonRVA_EE7320[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
__declspec(align(16)) float _CommonRVA_EE7460[4] = { 0.0f, 0.0f, 0.0f, 1.0f};
__declspec(align(16)) float _CommonRVA_EE7550[4] = { 0.0f, 1.0f, 0.0f, 1.0f};
__declspec(align(16)) float _CommonRVA_EE7060[4] = { 0.02f, 0.02f, 0.02f, 0.02f };
__declspec(align(16)) int _CommonRVA_EE8040[4] = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
__declspec(align(16)) int _CommonRVA_EE8050[4] = { -1, 0x7FFFFFFF, -1, 0x7FFFFFFF };
__declspec(align(16)) float _CommonRVA_EE8070[4] = { -0.0f, -0.0f, -0.0f, -0.0f };
__declspec(align(16)) float _CommonRVA_EE80E0[4] = { -4.25f, 8.5f, -4.25f, -1.0f };
__declspec(align(16)) float _CommonRVA_EE7DE0[4] = { 1.0f, -1.0f, -4.25f, 8.5f };
__declspec(align(16)) float _CommonRVA_EE79C0[4] = { 4.25f, -1.0f, 1.0f, 1.0f };
__declspec(align(16)) float _CommonRVA_EE7C10[4] = { 4.25f, 8.5f, -4.25f, 1.0f };
__declspec(align(16)) float _CommonRVA_EE7DD0[4] = { 1.0f, -1.0f, 4.25f, 8.5f };
__declspec(align(16)) float _CommonRVA_EE7940[4] = { 4.25f, 1.0f, 1.0f, 1.0f };
__declspec(align(16)) float _CommonRVA_EE7AB0[4] = { 18.0625f, 0.0f, 18.0625f, 1.0f };
__declspec(align(16)) float _ShotIndicatorCircleColor1[4] = { 0.3f, 0.0f, 0.0f, 0.8f };
__declspec(align(16)) float _ShotIndicatorCircleColor2[4] = { 0.2f, 0.0f, 0.0f, 0.2f };
__declspec(align(16)) float _ShotIndicatorCircleEdgeColor[4] = { 0.3f, 0.0f, 0.0f, 0.3f };

__declspec(align(8)) float _SubtitlePosition[2] = { 320.0f, 720.0f};
__declspec(align(8)) float _TextWingEnergy6Position[2] = { 18.0f, 283.0f };
__declspec(align(8)) WCHAR _AnimationEvent_BurstFire[] = L"BurstFire";
__declspec(align(8)) WCHAR _AnimationEvent_BurstFireStop[] = L"BurstFire_Stop";
// lyt_HUiMessageBoxNoFocus
__declspec(align(8)) WCHAR _lyt_HuiSubtitleBox[] = L"app:/ui/lyt_HuiSubtitleBox.sgo";
__declspec(align(8)) WCHAR _str_GameObjectDestroyFadeOutTime[] = L"game_object_destroy_fadeout_time";
__declspec(align(8)) WCHAR _str_GameObjectDestroySpawnObject[] = L"game_object_destroy_spawn_object";
__declspec(align(8)) WCHAR _str_RagdollBitmark[] = L"ragdoll_bitmark";
}