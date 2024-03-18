.data

extern vftable_WeaponHeavyShoot : qword

extern aligned_mallocAddr : qword
extern aligned_freeAddr : qword
extern __sqrtfAddr : qword
extern rva38AEF0 : qword
extern rva399760 : qword
extern rva38B100 : qword
extern rva949E0 : qword
extern vedf125AB30 : qword
extern vedf125AB68 : qword

extern rva38A4F0 : qword
extern rva25A20 : qword
;extern rva38A960 : qword
extern ASMrva38A960mod: proto
extern vftable_WeaponIndicatorLine : qword

extern rva3B67C0 : qword
;extern rva3B4F40 : qword
extern ASMrva3B4F40mod: proto
extern _CommonRVA_EE7460 : xmmword
extern rva3B5460 : qword


extern _Common_F0P01 : dword
extern _Common_F0P025 : dword
extern _Common_F1P0 : dword
extern _Common_F5P0 : dword
extern _Common_F3600P0 : dword

extern jmp_Umbra_Object_set : qword
extern rva38B6A0 : qword
extern weaponHeavyShootFuncP0Ret : qword

extern weaponHeavyShootFuncP10Call : qword
extern _CommonRVA_EE7060 : xmmword
extern rva38B390 : qword

extern weaponHeavyShootFuncFreeRet : qword
extern rva181DA0 : qword

.code

ASMweaponHeavyShootSetup proc

    ; original
        movaps xmm0, [rsp+20h]
        movaps [rbx+1230h], xmm0
        movsxd rax, dword ptr [rbp+8]
        mov ecx, [rax+rbp+74h]
        mov [rbx+1250h], ecx
    ; new
        ; initialization
        xor r14d, r14d
        xorps xmm0, xmm0
        ; laser sight
        movaps [rbx+12A0h], xmm0
        ;
        movaps [rbx+12D0h], xmm0
        movaps [rbx+12E0h], xmm0
        movaps [rbx+12F0h], xmm0

    ; check laser sight
        cmp dword ptr [rbp+4], 11
        jl ofs39B026 ; If it exists, read node9

        lea r8, vftable_WeaponHeavyShoot+8 ; load new virtual table
        mov [rbx], r8

        movsxd rcx, dword ptr [rbp+8]
        mov eax, [rcx+rbp+120+8] ; 12-bytes per node, get value
        cmp dword ptr [rcx+rbp+120], 2 ;check node type
        je indicatorCircleFixedRange ; if is float
        cmp eax, 1
        jne indicator ; if = 1, enable laser sight
    ; laser sight start
        sub rsp, 10h
        mov edx, 10h
        mov ecx, 170h
        call aligned_mallocAddr
        mov [rsp+80h], rax
        test rax, rax
        je laserSight3984D8
        mov rcx, rax
        call rva38AEF0
        jmp laserSight3984DB
    laserSight3984D8:
        mov rax, r14
    laserSight3984DB:
        xorps xmm0, xmm0
        movups [rsp+28h], xmm0
        mov rdx, rax
        lea rcx, [rsp+28h]
        call rva399760

        lea rdx, [rsp+28h]
        lea rcx, [rbx+12A0h]
        call rva949E0

        mov rcx, [rsp+30h]
        test rcx, rcx
        je laserSight39853D
        or esi, -1
        mov eax, esi
        lock xadd [rcx+8], eax
        cmp eax, 1
        jne laserSight39853D
        mov rbx, [rsp+30h]
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        lock xadd [rbx+0Ch], esi
        cmp esi, 1
        jne laserSight39853D
        mov rcx, [rsp+30h]
        mov rax, [rcx]
        call qword ptr [rax+8]
    laserSight39853D:
        mov rcx, vedf125AB30
        mov rax, [rcx]
        mov ecx, [rax+38h]
        cmp ecx, -1
        je laserSight3985A3
        mov rax, [rax+20h]
        mov rcx, [rax+rcx*8]
        mov rax, [rcx+10h]
        movsxd rcx, dword ptr [rax+8]
        cmp dword ptr [rcx+rax+5Ch], 0
        je laserSight3985A3
        mov rcx, qword ptr [rbx+0C0h]
        test rcx, rcx
        je laserSight39858D
        add rcx, 0C0h
        mov rax, [rcx]
        mov rdx, rbx
        call qword ptr [rax+40h]
        test rax, rax
        je laserSight39858D
        movzx ecx, byte ptr [rax+18h]
        and cl, 1
        jmp laserSight39858F
    laserSight39858D:
        mov cl, 1
    laserSight39858F:
        mov rax, [rbx+12A0h]
        mov byte ptr [rax+161h], 1
        mov byte ptr [rax+162h], cl
    laserSight3985A3:
        movss xmm2, dword ptr [rbx+1BCh]
        movzx r9d, r14b
        movss xmm1, _Common_F0P01
        mov rcx, [rbx+12A0h]
        call rva38B100
        jmp rspAdd10h
    ; laser sight end

    indicatorCircleFixedRange:
        test eax, eax
        mov edx, _Common_F5P0 ; get default
        cmovne edx, eax ; if eax != 0
        mov rcx, rbx
        call ASMweaponHeavyShootSetupIndicator
        jmp indicatorEnter
    
    indicator:
        cmp eax, 2
        jne ofs39B026
    indicatorCircle:
        mov edx, [rbx+6A8h] ; AmmoExplosion
        test edx, edx
        je indicatorEnter ; check is 0
        mov rcx, rbx
        call ASMweaponHeavyShootSetupIndicator
    ; indicator
    indicatorEnter:
        sub rsp, 10h
        mov edx, 10h
        mov ecx, 140h+10h ; set extra memory
        call aligned_mallocAddr
        mov [rsp+78h], rax
        test rax, rax
        je indicator3AB13D
        ; initialize
        mov rcx, rax
        call rva38A4F0
        mov rdi, rax
        mov [rdi+140h], r14
        mov [rdi+148h], r14
    indicator3AB0F6:
        lea rcx, [rbx+12D8h]
        call rva25A20
        test rdi, rdi
        je indicator3AB13D
        mov edx, 10h
        mov ecx, edx
        call aligned_mallocAddr
        test rax, rax
        je indicator3AB12F
        mov dword ptr [rax], 1
        mov dword ptr [rax+4], 1
        mov [rax+8], rdi
    indicator3AB12F:
        mov [rbx+12D8h], rax
        mov [rbx+12E0h], rdi
        ; +110 is color
        mov eax, _Common_F0P025
        mov dword ptr [rdi+130h], eax
    indicator3AB13D:
        mov dword ptr [rbx+12E8h], 3DCCCCCDh
        ; get AmmoAlive
        mov eax, [rbx+698h]
        mov edx, 180
        cmp eax, edx
        cmovg eax, edx
        jge indicatorAlive
        mov edx, 60
        cmp eax, edx
        cmovl eax, edx
    indicatorAlive:
        mov [rbx+12ECh], eax
        mov [rdi+140h], eax ; set laser length
        ; +144 is hit
        ; +148 is collision length
        lea rdx, vftable_WeaponIndicatorLine+8
        mov [rdi], rdx
    ; indicator end
    rspAdd10h:
        add rsp, 10h

    ofs39B026:
        mov rdi, [rsp+58h]
        mov rsi, [rsp+50h]
        mov rax, rbx
        mov rbp, [rsp+48h]
        mov rbx, [rsp+40h]
        add rsp, 30h
        pop r14
        ret 
        int 3

ASMweaponHeavyShootSetup ENDP

align 16

ASMweaponHeavyShootSetupIndicator proc

        push rbx
        sub rsp, 60h

        mov [rsp+50h], edx
        mov rbx, rcx

        mov edx, 10h
        mov ecx, 210h
        call aligned_mallocAddr
        mov [rbx+12F0h], rax
        test rax, rax
        je returnFunc
        mov rcx, rax
        call ASMrva3B4F40mod

        movaps xmm0, _CommonRVA_EE7460
        movaps [rsp+30h], xmm0
        movaps [rsp+40h], xmm0
        mov byte ptr [rsp+20h], 1 ; no horizontal
        movss xmm3, dword ptr [rsp+50h]
        movaps xmm0, xmm3
        mulss xmm0, dword ptr [rbx+308h] ; FireAccuracy
        addss xmm3, xmm0
        lea r8, [rsp+30h]
        lea rdx, [rsp+40h]
        mov rcx, [rbx+12F0h]
        call rva3B5460
        
    returnFunc:
        add rsp, 60h
        pop rbx
        ret 
        int 3

ASMweaponHeavyShootSetupIndicator ENDP

align 16

ASMweaponHeavyShootFuncP0 proc

        push rbx
        sub rsp, 20h
        mov rbx, rcx
    ; laser sight
        mov rcx, [rcx+12A0h]
        test rcx, rcx
        je Indicator
        movzx r8d, byte ptr [rbx+5ACh]
        xor edx, edx
        mov rcx, [rcx+10h]
        call jmp_Umbra_Object_set
        mov rcx, [rbx+12A0h]
        call rva38B6A0

    Indicator:
        cmp dword ptr [rbx+12E0h], 0
        je ofs398875
        mov rcx, rbx
        call ASMweaponHeavyShootFuncP0Indicator

        mov rcx, [rbx+12F0h]
        test rcx, rcx
        je ofs398875
        call rva3B67C0

    ofs398875:
        cmp qword ptr [rbx+0C60h], 0
        je ofs38F3A6
        lea rcx, [rbx+0C50h]
        add rsp, 20h
        pop rbx
        jmp weaponHeavyShootFuncP0Ret
    ofs38F3A6:
        add rsp, 20h
        pop rbx
        ret 
        int 3

ASMweaponHeavyShootFuncP0 ENDP

align 16

ASMweaponHeavyShootFuncP0Indicator proc

        push rbx
        sub rsp, 0A0h
        mov [rsp+90h], rsi
        mov [rsp+98h], rdi
        movaps [rsp+60h], xmm7
        movaps [rsp+50h], xmm8

        mov rbx, rcx
        mov rsi, [rcx+12E0h]
        mov rcx, rsi
        xor eax, eax
        cmp [rbx+8E8h], eax
        jle ofs3AB1FE
        cmp [rbx+0B94h], al
        je ofs3AB1FE
        ;cmp [rbx+0B40h], eax
        ;jg ofs3AB1FE
    ofs3AB1EF:
        cmp [rbx+0DEh], al ; check is current weapon
        setne al
    ofs3AB1FE:
        mov [rcx+134h], al
        test al, al
        je indicatorDraw

        xorps xmm2, xmm2
        ;lea rsi, [rbx+620h]
        mov rdi, [rbx+168h]
        movaps xmm7, [rdi+70h]
        movaps xmm0, xmm7
        movaps [rsp+50h], xmm7
        mulps xmm0, xmm7
        movaps xmm1, xmm0
        shufps xmm1, xmm0, 55h
        addss xmm1, xmm0
        shufps xmm0, xmm0, 0AAh
        addss xmm1, xmm0
        ucomiss xmm1, xmm2
        jp ofs3AB26A
        jne ofs3AB26A
        movss xmm8, dword ptr [rsp+5Ch]
        jmp ofs3AB2BF
    ofs3AB26A:
        movaps [rsp+70h], xmm6
        movaps xmm8, xmm7
        movaps xmm0, xmm1
        shufps xmm8, xmm7, 0FFh
        ;lea rax, [rbx+694h]
        ;mov ecx, 44h
        ;test rsi, rsi
        ;cmove rax, rcx
        ;movss xmm6, dword ptr [rax]
        movss xmm6, dword ptr [rbx+694h] ; AmmoSpeed
        maxss xmm6, xmm2
        call __sqrtfAddr
        divss xmm6, xmm0
        movaps xmm1, xmm6
        movaps xmm6, [rsp+70h]
        shufps xmm1, xmm1, 0
        mulps xmm7, xmm1
        movaps [rsp+50h], xmm7
        movss dword ptr [rsp+5Ch], xmm8
        movaps xmm7, [rsp+50h]
    ofs3AB2BF:
        movaps xmm0, [rdi+80h]
        movaps [rsp+40h], xmm0
        movss dword ptr [rsp+5Ch], xmm8
        mov rcx, vedf125AB68
        mov rax, [rcx]
        mov rcx, [rax+18h]
        movaps xmm3, [rcx+20h]
        movaps xmm2, xmm3
        shufps xmm2, xmm3, 0FFh
        ;lea rax, [rbx+6E0h]
        ;mov ecx, 90h
        ;test rsi, rsi
        ;cmove rax, rcx
        ;movss xmm0, dword ptr [rax]
        movss xmm0, dword ptr [rbx+6E0h] ; AmmoGravityFactor
        divss xmm0, _Common_F3600P0
        movaps xmm1, xmm0
        ;movss xmm0, dword ptr [rbx+12E8h]
        ;shufps xmm0, xmm0, 0
        ;mulps xmm7, xmm0
        shufps xmm1, xmm1, 0
        mulps xmm3, xmm1
        movaps xmm0, xmm7
        movss dword ptr [rsp+50h], xmm7
        shufps xmm0, xmm7, 55h
        shufps xmm7, xmm7, 0AAh
        movaps [rsp+30h], xmm3
        movss dword ptr [rsp+58h], xmm7
        movss dword ptr [rsp+3Ch], xmm2
        movss dword ptr [rsp+54h], xmm0

        mov eax, [rbx+12ECh]
        mov [rsp+20h], eax
        lea r9, [rsp+30h]
        lea r8, [rsp+50h]
        lea rdx, [rsp+40h]
        mov rcx, rsi
        call ASMrva38A960mod

    indicatorDraw:
        ; +1d8 is bottom color
        mov rcx, [rbx+12F0h]
        test rcx, rcx
        je ofs3AB37F
        xor r8d, r8d
        cmp [rsi+144h], r8d ; check hit
        je indicatorDrawCall
        movaps xmm7, [rsi+40h]
        movaps [rcx+0B0h], xmm7 ; => +130
        movzx r8d, byte ptr [rsi+134h]
    indicatorDrawCall:
        xor edx, edx
        mov rcx, [rcx+10h]
        call jmp_Umbra_Object_set

    ofs3AB37F:
        movaps xmm8, [rsp+50h]
        movaps xmm7, [rsp+60h]
        mov rdi, [rsp+98h]
        mov rsi, [rsp+90h]
        add rsp, 0A0h
        pop rbx
        ret 
        int 3

ASMweaponHeavyShootFuncP0Indicator ENDP

align 16

ASMweaponHeavyShootFuncP10 proc

        mov qword ptr [rsp+8], rbx
        push rdi
        sub rsp, 70h
        mov rdi, rdx
        mov rbx, rcx
    ; original
    ofs39B0BA:
        movaps xmm1, [rcx+1210h]
        movss xmm3, dword ptr [rcx+121Ch]
        movaps xmm2, [rcx+11F0h]
        movss xmm0, dword ptr [rcx+1220h]
        subps xmm2, xmm1
        shufps xmm0, xmm0, 0
        mulps xmm2, xmm0
        addps xmm2, xmm1
        movaps [rcx+1210h], xmm2
        movss dword ptr [rcx+121Ch], xmm3

        movss xmm1, dword ptr [rcx+124Ch]
        movss xmm0, dword ptr [rcx+1250h]
        shufps xmm0, xmm0, 0
        mulps xmm0, [rcx+1240h]
        movaps [rcx+1240h], xmm0
        movss dword ptr [rcx+124Ch], xmm1

        mov eax, [rcx+1210h]
        mov [rcx+0C0Ch], eax
        or dword ptr [rcx+0C00h], 4
        mov eax, [rcx+1214h]
        mov [rcx+0C04h], eax
        or dword ptr [rcx+0C00h], 1
        mov eax, [rcx+1218h]
        mov [rcx+0C08h], eax
        or dword ptr [rcx+0C00h], 2
        movups xmm0, [rcx+1240h]
        movups [rcx+0C20h], xmm0
        or dword ptr [rcx+0C00h], 20h

        call weaponHeavyShootFuncP10Call

        mov rcx, [rbx+11A8h]
        test rcx, rcx
        je indicator3AB3E3
        mov rax, [rcx]
        xor r9d, r9d
        mov r8, rdi
        lea edx, [r9+1]
        call qword ptr [rax+8]
    ofs39B193:
        movss xmm0, dword ptr [rdi]
        addss xmm0, dword ptr [rbx+11A0h]
        inc dword ptr [rbx+11A4h]
        movss dword ptr [rbx+11A0h], xmm0

    indicator3AB3E3:
        mov rax, qword ptr [rbx+12E0h]
        test rax, rax
        je laserSight3988EB
        movss xmm0, dword ptr [rax+10Ch]
        ;divss xmm0, dword ptr [rbx+12E8h]
        addss xmm0, dword ptr [rax+108h]
        comiss xmm0, _Common_F1P0
        movss dword ptr [rax+108h], xmm0
        jbe laserSight3988EB
        mov dword ptr [rax+108h], 0

    ; laser sight
    laserSight3988EB:
        mov rcx, [rbx+12A0h]
        test rcx, rcx
        je ofs39B1AD

        mov rax, [rbx+168h]
        lea r9, [rsp+50h]
        movaps xmm1, _CommonRVA_EE7060
        lea r8, [rsp+20h]
        mov rdx, rdi

        movaps xmm2, [rax+0A0h]
        movaps xmm3, [rax+0C0h]
        movaps xmm0, [rax+90h]
        mulps xmm1, xmm2
        movaps [rsp+30h], xmm0
        movaps xmm0, [rax+0B0h]
        movaps [rsp+50h], xmm0

        movaps xmm0, xmm1
        movss dword ptr [rsp+20h], xmm1
        shufps xmm0, xmm1, 55h
        shufps xmm1, xmm1, 0AAh
        movss dword ptr [rsp+28h], xmm1
        movaps xmm1, xmm3
        movss dword ptr [rsp+24h], xmm0
        movaps [rsp+40h], xmm2
        shufps xmm2, xmm2, 0FFh
        movss dword ptr [rsp+2Ch], xmm2
        subps xmm1, [rsp+20h]
        movaps [rsp+60h], xmm3

        shufps xmm3, xmm3, 0FFh
        movss dword ptr [rsp+2Ch], xmm3
        movaps xmm0, xmm1
        movss dword ptr [rsp+20h], xmm1
        shufps xmm0, xmm1, 55h
        shufps xmm1, xmm1, 0AAh
        movss dword ptr [rsp+28h], xmm1
        movss dword ptr [rsp+24h], xmm0

        call rva38B390

        mov rcx, vedf125AB30
        mov rax, [rcx]
        mov ecx, [rax+38h]
        cmp ecx, -1
        je ofs398A0F
        mov rax, [rax+20h]
        mov rcx, [rax+rcx*8]
        mov rdx, [rcx+10h]
        movsxd rax, dword ptr [rdx+8]
        cmp dword ptr [rax+rdx+5Ch], 0
        je ofs398A0F
        mov rcx, [rbx+0C0h]
        test rcx, rcx
        je ofs3989F9
        add rcx, 0C0h
        mov rdx, rbx
        mov rax, qword ptr [rcx]
        call qword ptr [rax+40h]
        test rax, rax
        je ofs3989F9
        movzx ecx, byte ptr [rax+18h]
        and cl, 1
        jmp ofs3989FB
    ofs3989F9:
        mov cl, 1
    ofs3989FB:
        mov rax, [rbx+12A0h]
        mov byte ptr [rax+161h], 1
        mov byte ptr [rax+162h], cl
    ofs398A0F:
        mov rcx, [rbx+12A0h]
        mov edx, [rcx+98h]
        test edx, edx
        je ofs398A5A
        sub edx, 1
        je ofs398A42
        cmp edx, 1
        jne ofs39B1AD
        mov dword ptr [rbx+5B8h], 3
        mov rbx, [rsp+80h]
        add rsp, 70h
        pop rdi
        ret 
    ofs398A42:
        mov dword ptr [rbx+5B8h], 2
        mov rbx, [rsp+80h]
        add rsp, 70h
        pop rdi
        ret 
    ofs398A5A:
        mov dword ptr [rbx+5B8h], 1
    ofs39B1AD:
        mov rbx, [rsp+80h]
        add rsp, 70h
        pop rdi
        ret 
        int 3

ASMweaponHeavyShootFuncP10 ENDP

align 16

ASMweaponHeavyShootFuncFree proc

        mov [rsp+8], rbx
        push rdi
        sub rsp, 30h
        mov [rsp+28h], rdx
        mov [rsp+20h], rcx
    ; indicator
        mov rbx, rcx
        lea rcx, [rbx+12D8h]
        call rva25A20
    indicatorCircle:
        mov rcx, [rbx+12F0h]
        test rcx, rcx
        je laserSight
        call rva181DA0
        mov rcx, [rbx+12F0h]
        call aligned_freeAddr
        
    laserSight:
        mov rbx, [rbx+12A8h]
        test rbx, rbx
        je ofs39B06A
        or edi, -1
        mov eax, edi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne ofs39B06A
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        lock xadd [rbx+0Ch], edi
        cmp edi, 1
        jne ofs39B06A
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]

    ofs39B06A:
        mov rdx, [rsp+28h]
        mov rcx, [rsp+20h]
        add rsp, 10h
        jmp weaponHeavyShootFuncFreeRet
        int 3

ASMweaponHeavyShootFuncFree ENDP

END
