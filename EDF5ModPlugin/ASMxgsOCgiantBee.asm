.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto
extern edf6136C0 : proto

extern vedf125AB68 : qword

;L"bee_AmmoClass"
beeAmmoClass db 98,0,101,0,101,0,95,0,65,0,109,0,109,0,111,0,67,0,108,0,97,0,115,0,115,0,0,0
; L"bee_BulletAlive"
beeBulletAlive db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"bee_BulletExSet"
beeBulletExSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCgiantBee proc

;lea rdx, beeAmmoClass
;lea rcx, [rbx+0B0h]
;call edf5BDF30 ; read sgo node
;cmp eax, -1
;je OriginalEndBlock
;mov r8, [rbx+0B0h]
;cdqe 
;movsxd rdx, dword ptr [r8+0Ch]
;lea rcx, [rax+rax*2]
;lea rax, [r8+rdx]
;lea rax, [rax+rcx*4]
;movsxd rdx, dword ptr [rax+8]
;mov rcx, vedf125AB68
;add rdx, rax
;mov rcx, [rcx]
;call edf6136C0
;mov [rbx+12A0h], rax

AmmoAliveBlock:
lea rdx, beeBulletAlive
lea rcx, [rbx+0B0h]
call edf5BDF30 ; read sgo node
cmp eax, -1
je AmmoExSetBlock ; if node does not exist, jump
; read int in node 
mov r8, [rbx+0B0h]
cdqe
movsxd rdx, dword ptr [r8+0Ch]
lea rcx, [rax+rax*2]
lea rax, [r8+rdx]
mov ecx, [rax+rcx*4+8]
mov [rbx+1318h], ecx ; AmmoAlive

AmmoExSetBlock:
lea rdx, beeBulletExSet
lea rcx, [rbx+0B0h]
call edf5BDF30 ; read sgo node
cmp eax, -1
je OriginalEndBlock ; if node does not exist, jump
; read ptr in node
mov r8, [rbx+0B0h]
cdqe
movsxd rdx, dword ptr [r8+0Ch]
lea rcx, [rax+rax*2]
lea rax, [r8+rdx]
lea rcx, [rax+rcx*4] ; store ptr address
; change shot count
movsxd rax, dword ptr [rcx+8]
mov eax, [rax+rcx+8]
imul eax, [rbx+13F0h] ; old * this
mov [rbx+13F0h], eax
; change ammo speed
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+20]
mulss xmm0, dword ptr [rbx+1314h] ; old * this
movss dword ptr [rbx+1314h], xmm0
; change ammo damage
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+32]
mulss xmm0, dword ptr [rbx+131Ch] ; old * this
movss dword ptr [rbx+131Ch], xmm0
; change ammo explosion range
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+44]
movss dword ptr [rbx+1328h], xmm0
; change ammo penetration
movsxd rax, dword ptr [rcx+8]
lea rax, [rax+rcx+48] ; store address
cmp dword ptr [rax+8],0
setne al
mov byte ptr [rbx+132Ch], al
; change ammo size
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+68]
movss dword ptr [rbx+1334h], xmm0
; change ammo hit size
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+80]
movss dword ptr [rbx+1338h], xmm0
; change ammo hit impulse
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+92]
movss dword ptr [rbx+133Ch], xmm0

OriginalEndBlock:
mov rbx, qword ptr [rsp+78h]
movaps xmm6, xmmword ptr [rsp+50h]
movaps xmm8, xmmword ptr [rsp+40h]
add rsp, 60h
pop rdi
ret 

;mov dword ptr ds:[rbx+1350],3F800000
;mov dword ptr ds:[rbx+1354],3F800000
;mov dword ptr ds:[rbx+1358],3F800000
;mov dword ptr ds:[rbx+135C],3F800000
;cmp dword ptr ds:[rbx+418],1
;mov dword ptr ds:[rbx+1360],3F800000
;mov byte ptr ds:[rbx+132E],1

ASMxgsOCgiantBee ENDP

END