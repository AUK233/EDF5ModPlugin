.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto

extern vedf125AB68 : qword
extern dragonSmallAmmoNextAddr : qword
extern dragonSmallAmmoRetAddr : qword
extern dragonSmallAmmoSetNextAddr : qword
extern dragonSmallAmmoSetRetAddr : qword
extern aligned_mallocAddr : qword
extern edf136890Address : qword
extern edf15FD90Address : qword
extern edf16DBB0Address : qword
extern edf185510Address : qword
extern edf171140Address : qword
extern edf156FF0Address : qword
extern edf187EC0Address : qword
extern edf150AD0Address : qword

; L"dragon_AttackBulletType"
dragonBulletType db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,65,0,116,0,116,0,97,0,99,0,107
db 0,66,0,117,0,108,0,108,0,101,0,116,0,84,0,121,0,112,0,101,0,0,0
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
; L"dragon_CustomBulletParam"
dragonCustomBulletParam db 100,0,114,0,97,0,103,0,111,0,110,0,95,0,67,0,117,0,115,0,116,0,111,0
db 109,0,66,0,117,0,108,0,108,0,101,0,116,0,80,0,97,0,114,0,97,0,109,0,0,0

; ASMxgsOCdragonSmallAmmoSet jump table
dragonAmmoTypeTable dq setDragonAcidBullet01, setDragonLightningBullet01, setDragonPlasmaBullet01, setDragonSolidBullet01, setDragonPulseBullet01, setDragonSolidExpBullet01, setDragonHomingLaserBullet01, setDragonLaserBullet02, 0


.code

ASMxgsOCdragonSmall proc
    ;mov dword ptr [rbx+129C],3F800000 ; damage
    ;mov dword ptr [rbx+12B4],ecx ;ammo size
    ;mov dword ptr [rbx+13A8],ecx ;accuracy
        movss dword ptr [rbx+14F0h], xmm0
    ;new
        mov dword ptr [rbx+1220h], 0
    AmmoTypeBlock:
        lea rdx, dragonBulletType
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je ChangeModelBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+1220h], ecx ; AmmoType

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
        je AmmoCBPBlock ; if node does not exist, jump
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
        ; check 0
        mov edx, 1
        cmp eax, edx
        cmovl eax, edx ; if < 1
        ; write value
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

    AmmoCBPBlock:
        lea rdx, dragonCustomBulletParam
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
        mov qword ptr [rbx+12E8h], rcx ; Ammo_CustomParameter

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

ASMxgsOCdragonSmallAmmo proc

        mov rax, qword ptr [rsi+2A8h]
        imul rax, r13
        add rax, 269EC3h
        mov qword ptr [rsi+2A8h], rax
        shr rax, 20h
        shl rax, 10h
        shr rax, 20h
        mov dword ptr [rbp+1A0h], eax
    ; set new ammo type
        cmp dword ptr [rsi+1220h], 0
        je returnNext
        lea r9, qword ptr [rbp+70h]
        lea r8, qword ptr [rbp-60h]
        lea rdx, qword ptr [rsp+40h]
        mov rcx, rsi
        call ASMxgsOCdragonSmallAmmoSet
        jmp dragonSmallAmmoRetAddr
    returnNext:
        jmp dragonSmallAmmoNextAddr
        int 3

ASMxgsOCdragonSmallAmmo ENDP

ASMxgsOCdragonSmallAmmoSet proc

        mov rax, rsp
        push rsi
        push rdi
        push r12
        push r14
        push r15
        sub rsp, 60h
        mov qword ptr [rax-60h], -2
        mov qword ptr [rax+8h], rbx
        mov qword ptr [rax+10h], rbp
        mov rdi, r9
        mov rsi, rdx
        mov r14, vedf125AB68
        mov r14, [r14]
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h], rax
    ; check ammo type
        mov eax, [rcx+1220h]
        cmp eax, 9
        jge ofs1F6C3C
    ; read jump table
        lea rdx, dragonAmmoTypeTable-8
        cdqe
        mov rax, [rdx+rax*8]
        jmp rax

    setDragonAcidBullet01::
        lea edx, [r12+10h]
        mov ecx, 650h ;AcidBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf136890Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonLightningBullet01::
        lea edx, [r12+10h]
        mov ecx, 880h ;LightningBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf15FD90Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonPlasmaBullet01::
        lea edx, [r12+10h]
        mov ecx, 0B00h ;PlasmaBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf16DBB0Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonSolidBullet01::
        lea edx, [r12+10h]
        mov ecx, 710h ;SolidBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf185510Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonPulseBullet01::
        lea edx, [r12+10h]
        mov ecx, 710h ;PulseBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf171140Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonSolidExpBullet01::
        lea edx, [r12+10h]
        mov ecx, 6F0h ;SolidExpBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf187EC0Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonHomingLaserBullet01::
        lea edx, [r12+10h]
        mov ecx, 8C0h ;HomingLaserBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf150AD0Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    setDragonLaserBullet02::
        lea edx, [r12+10h]
        mov ecx, 0C90h ;LaserBullet02Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs1F6C69
        mov rdx, rdi
        mov rcx, rax
        call edf156FF0Address
        mov rbp, rax
        jmp dragonSmallAmmoSetRetAddr
    ; over
    ofs1F6C69:
        mov rbp, r12
        jmp dragonSmallAmmoSetRetAddr
    ofs1F6C3C:
        jmp dragonSmallAmmoSetNextAddr
        int 3

ASMxgsOCdragonSmallAmmoSet ENDP

END