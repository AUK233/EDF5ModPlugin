.data

extern weaponStartReloadRetAddr : qword

.code

ASMweaponStartReload proc

cmp dword ptr [rbx+2100h], 0
je originalBlock
; current reload time = (totalAmmo - remainAmmo) * reloadTime  / totalAmmo
mov eax, dword ptr [rbx+1D0h]
mov ecx, eax ; totalAmmo
sub eax, dword ptr [rbx+8E8h] ; - remainAmmo
imul eax, dword ptr [rbx+1A4h] ; * reloadTime
; use float division
cvtsi2ss xmm0, eax
cvtsi2ss xmm1, ecx ; int to float
divss xmm0, xmm1 ; / totalAmmo
cvttss2si eax, xmm0 ; float to int
add eax, dword ptr [rbx+2104h] ; + extra time
mov dword ptr [rbx+0B90h], eax
; original
originalBlock:
mov dword ptr [rbx+8E8h], 0
mov dword ptr [rbx+0B40h], 4
jmp weaponStartReloadRetAddr

ASMweaponStartReload ENDP

END