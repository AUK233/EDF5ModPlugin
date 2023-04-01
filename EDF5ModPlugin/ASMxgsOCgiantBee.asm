.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto
;extern edf6136C0 : proto

extern vedf125AB68 : qword
extern giantBeeAmmoNextAddr : qword
extern giantBeeAmmoRetAddr : qword
extern giantBeeAmmoSetNextAddr : qword
extern giantBeeAmmoSetRetAddr : qword
extern aligned_mallocAddr : qword
extern edf136890Address : qword
extern edf14DE20Address : qword
extern edf16DBB0Address : qword
extern edf185510Address : qword
extern edf171140Address : qword
extern edf156FF0Address : qword
extern edf187EC0Address : qword
extern edf150AD0Address : qword

;L"bee_BulletSet"
beeBulletSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,83,0,101,0,116,0,0,0
; L"bee_BulletAccuracy"
beeBulletAccuracy db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,99,0,99,0,117,0,114,0,97,0,99,0,121,0,0,0
; L"bee_BulletAlive"
beeBulletAlive db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"bee_BulletExSet"
beeBulletExSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

; ASMxgsOCgiantBeeAmmoSet jump table
beeAmmoTypeTable dq setBeeAcidBullet01, setBeeFlameBullet01, setBeePlasmaBullet01, setBeeSolidBullet01, setBeePulseBullet01, setBeeSolidExpBullet01, setBeeHomingLaserBullet01, setBeeLaserBullet02, 0

.code

ASMxgsOCgiantBee proc

        mov dword ptr [rbx+12A0h], 0 ; ammo type
        mov dword ptr [rbx+12A4h], 3F333333h ; shot accuracy, 0.7f
    AmmoSetBlock:
        lea rdx, beeBulletSet
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        ; set ammo type
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        mov [rbx+12A0h], eax
        ; set ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+1350h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+1354h], xmm1 ;g
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+1358h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+56]
        movss dword ptr [rbx+135Ch], xmm1 ;g

    AmmoAliveBlock:
        lea rdx, beeBulletAlive
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAccuracyBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+1318h], ecx ; AmmoAlive

    AmmoAccuracyBlock:
        lea rdx, beeBulletAccuracy
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
        movss xmm0, dword ptr [rax+rcx*4+8]
        movss dword ptr [rbx+12A4h], xmm0 ; shot accuracy

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
        ; check 0
        mov edx, 1
        cmp eax, edx
        cmovl eax, edx ; if < 1
        ; write value
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
        int 3
        ;cmp dword ptr [rbx+418],1
        ;mov dword ptr [rbx+1360],3F800000
        ;mov byte ptr [rbx+132E],1

ASMxgsOCgiantBee ENDP

; set ammo

ASMxgsOCgiantBeeAmmo proc

        movaps xmmword ptr [rbp-50h], xmm0
        movaps xmm0, xmmword ptr [rbp+10h]
        movdqa xmmword ptr [rbp-40h], xmm0
    ; set new ammo type
        cmp dword ptr [r14+12A0h], 0
        je returnNext
        lea r9, [rbp+60h]
        lea r8, [rbp-70h]
        lea rdx, [rsp+40h]
        mov rcx, r14
        call ASMxgsOCgiantBeeAmmoSet
        jmp giantBeeAmmoRetAddr
    returnNext:
        jmp giantBeeAmmoNextAddr
        int 3

ASMxgsOCgiantBeeAmmo ENDP

ASMxgsOCgiantBeeAmmoSet proc

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
        mov qword ptr [r9+20h],rax
    ; check ammo type
        mov eax, [rcx+12A0h]
        cmp eax, 9
        jge ofs2105DC
    ; read jump table
        lea rdx, beeAmmoTypeTable-8
        cdqe
        mov rax, [rdx+rax*8]
        jmp rax
        ;cmp eax, 1
        ;je setAcidBullet01
        ;cmp eax, 2
        ;je setFlameBullet01
        ;cmp eax, 3
        ;je setPlasmaBullet01
        ;jmp giantBeeAmmoSetNextAddr
    ; SpiderStringBullet can cause crashes
    setBeeAcidBullet01::
        lea edx, [r12+10h]
        mov ecx, 650h ;AcidBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf136890Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeeFlameBullet01::
        lea edx, [r12+10h]
        mov ecx, 8D0h ;FlameBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf14DE20Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeePlasmaBullet01::
        lea edx, [r12+10h]
        mov ecx, 0B00h ;PlasmaBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf16DBB0Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeeSolidBullet01::
        lea edx, [r12+10h]
        mov ecx, 710h ;SolidBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf185510Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeePulseBullet01::
        lea edx, [r12+10h]
        mov ecx, 710h ;PulseBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf171140Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeeSolidExpBullet01::
        lea edx, [r12+10h]
        mov ecx, 6F0h ;SolidExpBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf187EC0Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeeHomingLaserBullet01::
        lea edx, [r12+10h]
        mov ecx, 8C0h ;HomingLaserBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf150AD0Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    setBeeLaserBullet02::
        lea edx, [r12+10h]
        mov ecx, 0C90h ;LaserBullet02Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf156FF0Address
        mov rbp, rax
        jmp giantBeeAmmoSetRetAddr
    ; over
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
    ofs2105DC:
        jmp giantBeeAmmoSetNextAddr
        int 3

ASMxgsOCgiantBeeAmmoSet ENDP

END