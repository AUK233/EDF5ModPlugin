#pragma once

constexpr unsigned char nop1 = 0x90;
constexpr unsigned char nop2[] = {0x66, 0x90};
constexpr unsigned char nop3[] = {0x0F, 0x1F, 0x00};
constexpr unsigned char nop4[] = {0x0F, 0x1F, 0x40, 0x00};
constexpr unsigned char nop5[] = {0x0F, 0x1F, 0x44, 0x00, 0x00};
constexpr unsigned char nop6[] = {0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00};
constexpr unsigned char nop7[] = {0x0F, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00};
constexpr unsigned char nop8[] = {0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00};
constexpr unsigned char nop9[] = {0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00};
constexpr unsigned char nop10[] = {0x66, 0x66, 0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00};