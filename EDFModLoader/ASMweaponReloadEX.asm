.data
; Use other asm functions
extern edf5BDF30 : proto

extern weaponReloadEXRetAddr : qword

; L"ReloadInit"
wReloadInit db 82,0,101,0,108,0,111,0,97,0,100,0,73,0,110,0,105,0,116,0,0,0
; L"ReloadPadType"
wReloadPadType db 82,0,101,0,108,0,111,0,97,0,100,0,80,0,97,0,100,0,84,0,121,0,112,0,101,0,0,0
; 0.99f
wReloadInitFloat db 0A4h, 70h, 7Dh, 3Fh


.code

ASMweaponReloadEX proc

lea rdx, wReloadInit
mov rcx, r14
call edf5BDF30
movsxd rcx, eax
cmp ecx, -1
jne ofs38E2F9
mov rax,rdi
jmp ofs38E30B
ofs38E2F9:
mov rax, qword ptr [r14]
movsxd rdx, dword ptr [rax+0Ch]
add rdx, rax
lea rcx, qword ptr [rcx+rcx*2]
lea rax, qword ptr [rdx+rcx*4]
ofs38E30B:
movss xmm1, dword ptr [rax+8]
movss xmm0, dword ptr wReloadInitFloat
comiss xmm0, xmm1
jbe ofs38E33C
mov dword ptr [rsi+8E8h], edi
movd xmm0, dword ptr [rsi+1A4h]
cvtdq2ps xmm0, xmm0
mulss xmm0, xmm1
cvttss2si eax, xmm0
sub dword ptr [rsi+0B90h], eax
ofs38E33C:
mov dword ptr [rsi+590h], 3F800000h

; initialize memory
mov qword ptr [rsi+2100h], 0
; read new function
lea rdx, wReloadPadType
mov rcx, r14
call edf5BDF30
movsxd rcx, eax
cmp ecx, -1
je EndBlock ; if node does not exist, jump
; read ptr in node
mov rax, qword ptr [r14]
movsxd rdx, dword ptr [rax+12]
add rdx, rax
lea rcx, qword ptr [rcx+rcx*2]
lea rcx, qword ptr [rdx+rcx*4]
; node0 decides on midsection reload
movsxd rax, dword ptr [rcx+8]
mov eax, [rax+rcx+8]
mov [rsi+2100h], eax ; midsection reload
cmp eax, 2
jne EndBlock ; if node does not exist, jump
; node1
movsxd rax, dword ptr [rcx+8]
mov eax, [rax+rcx+20]
cmp eax, 1
jl EndBlock ; if < 1
mov [rsi+2104h], eax

EndBlock:
jmp weaponReloadEXRetAddr

ASMweaponReloadEX ENDP

END