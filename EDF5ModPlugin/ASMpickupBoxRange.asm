.data

extern pickupBoxRangeFRetAddr : qword
extern pickupBoxRangeTRetAddr : qword
; 25.0f
pickupRangePlusPlus dd 41C80000h

.code

; [rbx+60h] pstr
ASMpickupBoxRange proc
shufps xmm0, xmm0, 0AAh
addss xmm1, xmm0
movaps xmm0, xmm7
mulss xmm0, xmm7
; weapon = 0, armor = 1
cmp dword ptr [rdi+0A8h], 1
jle getBoxBlock
jmp pickupBoxRangeFRetAddr

getBoxBlock:
; check total, A88h is weapon, A8Ch is armor
cmp dword ptr [rsi+0A84h], 128
jg rangeBlock
jmp pickupBoxRangeTRetAddr

rangeBlock:
; xmm0 is player, xmm1 is item
mulss xmm0, pickupRangePlusPlus
jmp pickupBoxRangeFRetAddr

ASMpickupBoxRange ENDP

END