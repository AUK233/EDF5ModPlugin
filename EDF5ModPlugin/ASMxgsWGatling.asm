.data

extern wGatlingSetupRetAddr : qword
extern wGatlingShotRetAddr : qword

.code

ASMweaponGatlingSetup proc
; original
mov dword ptr [rbx+11D4h], 3E800000h
mov dword ptr [rbx+11D8h], 3D800000h
mov byte ptr [rbx+11F0h], 0
; new, set pre-heat type
; initialize memory
mov qword ptr [rbx+1408h], 0
mov dword ptr [rbx+1400h], 0
mov ecx, dword ptr [rbx+2FCh] ; store original FireInterval
mov dword ptr [rbx+1404h], ecx
; check node
mov eax, dword ptr [rsi+4]
cmp eax, 10
jl ofs39A0E0
; If it exists, read node9
movsxd rax, dword ptr [rsi+8]
mov ecx, dword ptr [rax+rsi+116]
mov dword ptr [rbx+1400h], ecx ; set pre-heat type
; return original
ofs39A0E0:
jmp wGatlingSetupRetAddr

ASMweaponGatlingSetup ENDP

; next

ASMweaponGatlingShot proc
; check preheat type
cmp dword ptr [rcx+1400h], 0
je ofs39A7AA
; check other
cmp dword ptr [rcx+8E8h], 0 ; check ammo
je returnBlock
cmp dword ptr [rcx+0B40h], 0 ; check ROF count
je checkROFBlock
jmp wGatlingShotRetAddr
; decision to modify ROF
checkROFBlock:
cmp dword ptr [rcx+11A4h], 1 ; check delay count
jle returnBlock
mov eax, dword ptr [rcx+1404h] ; get old rof
cvtsi2ss xmm0, dword ptr [rcx+11A4h] ; delay count
cvtsi2ss xmm1, dword ptr [rcx+11A8h] ; total delay
comiss xmm0, xmm1
jl setROFBlock ; delay count < total delay
mov dword ptr [rcx+2FCh], eax ; set ROF is FireInterval
jmp wGatlingShotRetAddr

setROFBlock:
divss xmm1, xmm0 ; total delay / delay count
cvtsi2ss xmm0, eax ; ROF to float
mulss xmm0, xmm1 ; * ROF
cvttss2si eax, xmm0 ; float to int
shr eax, 1 ; / 2
mov dword ptr [rcx+2FCh], eax ; to ROF
jmp wGatlingShotRetAddr
; original
ofs39A7AA:
mov eax, dword ptr [rcx+11A8h] ; total delay
cmp dword ptr [rcx+11A4h], eax ; delay count
jl returnBlock
jmp wGatlingShotRetAddr
; return
returnBlock:
add rsp, 38h
ret 

ASMweaponGatlingShot ENDP

END