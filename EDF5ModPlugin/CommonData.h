#pragma once

constexpr UINT32 AcidBullet01Size = 0x650;
constexpr UINT32 FlameBullet01Size = 0x8D0;
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

extern "C" {
float _Common_F0P01 = 0.01f;
float _Common_F0P025 = 0.025f;
float _Common_F0P2 = 0.2f;
float _Common_F0P8 = 0.8f;
float _Common_F1P0 = 1.0f;
float _Common_F5P0 = 5.0f;
float _Common_2PI = 6.28319f;
float _Common_F8P0 = 8.0f;
float _Common_F60P0 = 60.0f; 
float _Common_F70P0 = 70.0f; 
float _Common_F3600P0 = 3600.0f;

__declspec(align(16)) float _CommonRVA_EE7700[4] = { 1.0f, 0.001f, 0.001f, 0.001f };
__declspec(align(16)) float _CommonRVA_EE7550[4] = {0.0f, 1.0f, 0.0f, 1.0f};
__declspec(align(16)) float _CommonRVA_EE7460[4] = {0.0f, 0.0f, 0.0f, 1.0f};
__declspec(align(16)) float _CommonRVA_EE7320[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
__declspec(align(16)) float _CommonRVA_EE7060[4] = { 0.02f, 0.02f, 0.02f, 0.02f };
}