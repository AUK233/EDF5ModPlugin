.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto

; L"dragon_ChangeModelParam"
dragonChangeModel db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,67,0,104,0,97,0,110,0,103,0,101,0
db 77,0,111,0,100,0,101,0,108,0,80,0,97,0,114,0,97,0,109,0,0,0
; L"dragon_AttackBulletAlive"
dragonBulletAlive db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,65,0,116,0,116,0,97,0,99,0,107
db 0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"dragon_AttackBulletColor"
dragonBulletColor db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,65,0,116,0,116,0,97,0,99,0,107
db 0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
; L"dragon_AttackBulletExSet"
dragonBulletExSet db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,65,0,116,0,116,0,97,0,99,0,107
db 0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCdragonSmall proc
;mov dword ptr ds:[rbx+129C],3F800000 ; damage
;mov dword ptr ds:[rbx+12B4],ecx ;ammo size
;mov dword ptr ds:[rbx+13A8],ecx?
        movss dword ptr [rbx+14F0h], xmm0

    ChangeModelBlock:
        lea rdx, dragonChangeModel
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock
        mov r8, [rbx+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        lea rcx, [rbx+480h]
        call edf3AE530

    AmmoAliveBlock:
        lea rdx, dragonBulletAlive
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoColorBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+1298h], ecx ; AmmoAlive

    AmmoColorBlock:
        lea rdx, dragonBulletColor
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        ; change ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+8]
        movss dword ptr [rbx+12D0h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+12D4h], xmm1 ;g
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+12D8h], xmm0 ;b
        movss xmm1, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+12DCh], xmm1 ;a

    AmmoExSetBlock:
        lea rdx, dragonBulletExSet
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
        imul eax, [rbx+1370h] ; old * this
        mov [rbx+1370h], eax
        ; change ammo explosion range
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+12A8h], xmm0
        ; change ammo penetration
        movsxd rax, dword ptr [rcx+8]
        lea rax, [rax+rcx+24] ; store address
        cmp dword ptr [rax+8], 0
        setne al
        mov byte ptr [rbx+12ACh], al
        ; change ammo hit size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+12B8h], xmm0
        ; change ammo hit impulse
        movss xmm1, dword ptr [rax+rcx+56]
        movss dword ptr [rbx+12BCh], xmm1

    OriginalEndBlock:
        lea r11, qword ptr [rsp+0B0h]
        mov rbx, qword ptr [r11+18h]
        mov rsi, qword ptr [r11+20h]
        movaps xmm6, xmmword ptr [r11-10h]
        movaps xmm7, xmmword ptr [r11-20h]
        movaps xmm8, xmmword ptr [r11-30h]
        movaps xmm9, xmmword ptr [r11-40h]
        movaps xmm10, xmmword ptr [r11-50h]
        movaps xmm11, xmmword ptr [r11-60h]
        movaps xmm12, xmmword ptr [r11-70h]
        mov rsp, r11
        pop rdi
        ret 

ASMxgsOCdragonSmall endp

END