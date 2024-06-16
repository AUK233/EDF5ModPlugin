.data

extern edf185510Address : qword
extern vftable_SolidBullet01 : qword
extern vftable_SolidBullet01A_C0 : qword
extern vftable_SolidBullet01A570 : qword

extern rva1BD600 : qword
extern rva5C0C50 : qword
extern ammoSolidBullet01FuncP0Ret : qword

extern rva1BDA30 : qword
extern ammoSolidBullet01FuncP10Ret : qword

extern rva140470 : qword
extern ammoSolidBullet01A570FuncP0Ret : qword

extern rva1BDC20 : qword
extern m4x4_118C710 : qword
extern ammoSolidBullet01A570FuncP8Ret : qword

extern _Common_F0P9 : dword
extern _Common_F1P1 : dword
extern _CommonRVA_EE80D0 : xmmword
extern _Common_2PI : dword
extern _Common_F0P05 : dword
extern _Common_F0P005 : dword
extern _Common_F0P2 : dword
extern _Common_F0P004 : dword
extern _Common_F0P01 : dword
extern _Common_F0P1 : dword
extern _Common_F0P3 : dword
extern _Common_F0P5 : dword
extern _Common_F5P0 : dword

extern _CommonRVA_EE7350 : xmmword
extern _CommonRVA_EE6BE0 : xmmword

extern rva1BCD40 : qword
extern rva1BCBA0 : qword
extern vedf125ABA0 : qword
extern rva1ACA80 : qword
extern rva6E160 : qword
extern rva6E0D0 : qword
extern rva70730 : qword
extern RtlEnterCriticalSection : qword
extern RtlLeaveCriticalSection : qword
extern vedfE940C0 : qword
extern vedfE94130 : qword
extern rva5C8410 : qword
extern rva5C8660 : qword
extern rva707B0 : qword
extern rva70850 : qword
extern rva1BCDF0 : qword
extern rva5C6C80 : qword

align 16
AmmoTailSmoke2Texture DB 74h, 56h, 04h, 5Ch, 59h, 71h, 5Fh, 00h, 30h, 00h, 31h, 00h, 2Eh, 00h, 64h, 00h, 64h, 00h, 73h, 00h, 00h, 00h, 00h, 00h


.code

ASMammoSolidBullet01 proc
    ; original
        test rax, rax
        je ofs1846B8
        mov rdx, rbx
        mov rcx, rax
        call edf185510Address
        mov rdx, rbx
        mov rcx, rax
        call ASMammoSolidBullet01Init
    ofs1846B8:
        add rsp, 30h
        pop rbx
        ret 
        int 3

ASMammoSolidBullet01 ENDP

ASMammoSolidBullet01Init proc

        push rbx
        push rdi
        push rsi
        sub rsp, 30h
        mov rbx, rcx
    ; initialization
        xorps xmm0, xmm0
        xorps xmm1, xmm1
        movaps [rcx+710h], xmm0
        movaps [rcx+720h], xmm1
        movaps [rcx+730h], xmm0
        movaps [rcx+740h], xmm1
        ; Set tail smoke default to 0
        movaps [rcx+800h], xmm0
        movaps [rcx+0A00h], xmm1
    ; read Ammo_CustomParameter
        mov rdi, [rbx+3A8h] ; get pointer
        movsxd rdx, dword ptr [rdi+8]
        mov esi, [rdi+4]
    ; node3
        cmp esi, 3 ; this is 4th node
        jle returnAddr
        mov eax, [rdx+rdi+36+8]
        test eax, eax
        je node4 ; = 0 no building destruction
        or word ptr [rbx+20Ch], 3
    node4:
        cmp esi, 4 ; this is 5th node
        jle returnAddr
        ; load new virtual table
        lea r8, vftable_SolidBullet01+8
        mov [rbx], r8
        lea r8, vftable_SolidBullet01A_C0+8
        mov [rbx+0C0h], r8
        lea r8, vftable_SolidBullet01A570+8
        mov [rbx+570h], r8
        ;
        lea rax, [rdx+rdi+48] 
        cmp dword ptr [rax], 1 ;check node type
        jne node4hasTexture
        mov r8d, [rax+8]
        test r8, r8
        je returnAddr
        lea rdx, [rbx+800h]
        mov rcx, rbx
        call ASMinitializeAmmoTailSmoke2
        jmp returnAddr
    node4hasTexture:
        movsxd r11, dword ptr [rax+8]
        add r11, rax
        ; read ptr node0, get length
        mov eax, [r11+8]
        test eax, eax ; skip if it does not exist
        je returnAddr
        ; read ptr node1, get texture name
        movsxd r9, dword ptr [r11+12+8]
        test r9, r9
        je returnAddr
        ; read ptr node2, get tail speed
        mov edx, [r11+24+8]
        mov [rsp+20h], edx
        ; initialize tail smoke
        add r9, r11
        add r9, 12
        mov r8d, eax
        lea rdx, [rbx+800h]
        mov rcx, rbx
        call ASMinitializeAmmoTailSmoke1
    returnAddr:
        add rsp, 30h
        pop rsi
        pop rdi
        pop rbx
        ret 
        int 3

ASMammoSolidBullet01Init ENDP

align 16

ASMammoSolidBullet01FuncP0 proc

        cmp byte ptr [rcx+800h+204h], 0
        je ofs185540 ; check tail smoke exists
        push rbx
        sub rsp, 30h
        mov rbx, rcx
        ;
        add rcx, 800h
        call rva1BD600
        lea rdx, [rbx+800h+1E0h]
        lea rcx, [rsp+10h]
        call rva5C0C50
        ;
        mov rcx, rbx
        add rsp, 30h
        pop rbx
    ofs185540:
        jmp ammoSolidBullet01FuncP0Ret
        int 3

ASMammoSolidBullet01FuncP0 ENDP

align 16

ASMammoSolidBullet01FuncP10 proc

        cmp byte ptr [rcx+800h+204h], 0
        je ofs185730 ; check tail smoke exists
        push rbx
        sub rsp, 40h
        mov rbx, rcx
        ;
        lea rcx, [rbx+800h]
        movaps xmm0, [rbx+0A0h]
        movaps [rcx+50h], xmm0
        movaps xmm2, [rbx+90h]
        movaps xmm1, xmm2
        shufps xmm1, xmm2, 0FFh
        mulps xmm2, _CommonRVA_EE80D0
        movss dword ptr [rsp+3Ch], xmm1
        movaps xmm0, xmm2
        movss dword ptr [rsp+30h], xmm2
        shufps xmm0, xmm2, 055h
        shufps xmm2, xmm2, 0AAh
        movss dword ptr [rsp+38h], xmm2
        movss dword ptr [rsp+34h], xmm0
        movaps xmm0, [rsp+30h]
        movaps [rcx+60h], xmm0
        call rva1BDA30
        ;
        mov rcx, rbx
        add rsp, 40h
        pop rbx
    ofs185730:
        jmp ammoSolidBullet01FuncP10Ret
        int 3

ASMammoSolidBullet01FuncP10 ENDP

align 16

ASMammoSolidBullet01FuncP48 proc

        cmp byte ptr [rcx+800h+204h], 0
        je ofs184E90 ; check tail smoke exists
        ;
        push rdx
        push rcx
        sub rsp, 28h

        lea rcx, [rcx+800h]
        call rva140470

        add rsp, 28h
        pop rcx
        pop rdx
    ofs184E90:
        jmp ammoSolidBullet01A570FuncP0Ret
        int 3

ASMammoSolidBullet01FuncP48 ENDP

align 16

ASMammoSolidBullet01A_C0FuncP0 proc

        sub rcx, 0C0h
        jmp ASMammoSolidBullet01FuncP48
        int 3

ASMammoSolidBullet01A_C0FuncP0 ENDP

align 16

ASMammoSolidBullet01A570FuncP0 proc

        sub rcx, 570h
        jmp ASMammoSolidBullet01FuncP48
        int 3

ASMammoSolidBullet01A570FuncP0 ENDP

align 16

ASMammoSolidBullet01A570FuncP8 proc

        cmp r8d, 2
        jne ofs186827
        cmp byte ptr [rcx-570h+800h+204h], 0
        je ofs1857FA ; check tail smoke exists
        ;
        push rcx
        push rdx
        sub rsp, 28h

        mov r8, m4x4_118C710
        lea rcx, [rcx-570h+800h]
        call rva1BDC20

        add rsp, 28h
        pop rdx
        pop rcx
    ofs1857FA:
        jmp ammoSolidBullet01A570FuncP8Ret
    ofs186827:
        ret
        int 3

ASMammoSolidBullet01A570FuncP8 ENDP

align 16

ASMinitializeAmmoTailSmoke2 proc

        push rbp
        push rsi
        push rdi
        lea rbp, [rsp-0C0h]
        sub rsp, 1C0h
        mov qword ptr [rsp+70h], -2
        ;mov qword ptr [rsp+78h], -2
        mov [rsp+1F0h], rbx
        mov rsi, rdx
        mov rdi, rcx
        mov ebx, r8d
        ; end
        mov rcx, rdx
        call rva1BCD40
        ;
        lea rcx, [rbp-80h]
        call rva1BCBA0
        ;
        movd xmm1, ebx
        cvtdq2ps xmm1, xmm1
        movaps xmm0, xmm1
        mulss xmm0, _Common_F0P9
        cvttss2si eax, xmm0
        mov [rbp-7Ch], eax
        mulss xmm1, _Common_F1P1
        cvttss2si eax, xmm1
        mov [rbp-78h], eax
        ;mov dword ptr [rbp-80h], 78
        ;mov dword ptr [rbp-7Ch], 36
        ;mov dword ptr [rbp-78h], 42
        mov dword ptr [rbp-74h], 1
        mov byte ptr [rbp-40h], 1
        movups xmm0, [rdi+0A0h]
        movaps [rbp-30h], xmm0
        movaps xmm1, [rdi+90h]
        movaps xmm0, xmm1
        shufps xmm0, xmm1, 0FFh
        mulps xmm1, _CommonRVA_EE80D0
        movaps [rsp+20h], xmm1
        movss dword ptr [rsp+2Ch], xmm0
        movaps xmm0, [rsp+20h]
        movdqa [rbp-20h], xmm0
        xorps xmm1, xmm1
        movss dword ptr [rbp], xmm1
        movss xmm0, _Common_F0P2
        movss dword ptr [rbp+4], xmm0
        movss xmm1, _Common_F0P05
        movss dword ptr [rbp+14h], xmm1
        mov dword ptr [rbp+18h], 3F000000h
        movss xmm1, _Common_F0P005
        movss dword ptr [rbp+1Ch], xmm1
        movss xmm2, dword ptr [rdi+354h]
        movaps xmm0, xmm2
        mulss xmm0, _Common_F0P004
        movss dword ptr [rbp+8], xmm0
        mulss xmm2, _Common_F0P01
        movss dword ptr [rbp+0Ch], xmm2
        movaps xmm0, _CommonRVA_EE7350
        movaps [rbp+20h], xmm0
        movaps xmm1, _CommonRVA_EE6BE0
        movaps [rbp+30h], xmm1
        mov word ptr [rbp+40h], 101h
        lea r8, AmmoTailSmoke2Texture
        mov rcx, vedf125ABA0
        lea rdx, [rsp+30h]
        mov rcx, [rcx]
        call rva1ACA80
        mov rbx, rax
        lea rcx, [rbp+50h]
        mov rax, [rbp+50h]
        call qword ptr [rax+8]
        mov rax, [rbx+8]
        mov [rbp+58h], rax
        lea rdx, [rbx+10h]
        lea rcx, [rbp+60h]
        call rva6E160
        lea rdx, [rbx+20h]
        lea rcx, [rbp+70h]
        call rva6E0D0
        lea rcx, [rbp+80h]
        call rva70730
        mov rax, [rbx+30h]
        mov [rbp+80h], rax
        mov rbx, [rbx+38h]
        mov [rbp+88h], rbx
        test rbx, rbx
        je ofs13F741
        add rbx, 28h
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov rax, [rbp+80h]
        inc dword ptr [rax+58h]
        test rbx, rbx
        je ofs13F741
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs13F741:
        mov rbx, vedfE940C0
        mov [rsp+30h], rbx
        lea rcx, [rsp+60h]
        call rva70730
        mov rax, vedfE94130
        mov [rsp+30h], rax
        mov rcx, [rsp+38h]
        test rcx, rcx
        je ofs13F790
        lea rdx, [rsp+40h]
        call rva5C8410
        lea rdx, [rsp+50h]
        mov rcx, [rsp+38h]
        call rva5C8660
        mov qword ptr [rsp+38h], 0
    ofs13F790:
        lea rcx, qword ptr [rsp+50h]
        call rva707B0
        lea rcx, qword ptr [rsp+40h]
        call rva70850
        mov dword ptr [rbp+98h], 0
        mov dword ptr [rbp+9Ch], 3F000000h
        movss xmm0, _Common_F0P3
        movss dword ptr [rbp+0A4h], xmm0
        mov byte ptr [rbp+0A8h], 1
        lea rdx, qword ptr [rbp-80h]
        mov rcx, rsi
        call rva1BCDF0
        mov [rbp+50h], rbx
        lea rcx, [rbp+80h]
        call rva70730
        lea rcx, [rbp+50h]
        call rva5C6C80
        ;
        mov rbx, [rsp+1F0h]
        add rsp, 1C0h
        pop rdi
        pop rsi
        pop rbp
        ret 
        int 3

ASMinitializeAmmoTailSmoke2 ENDP

align 16

ASMinitializeAmmoTailSmoke1 proc

        mov rax, rsp
        push rbp
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        lea rbp, [rax-158h]
        sub rsp, 220h
        mov qword ptr [rsp+40h], -2
        mov [rax+18h], rbx
        movaps [rax-48h], xmm6
        mov rsi, rdx
        mov rdi, rcx
        mov r13d, r8d
        xor r14d, r14d
        mov r15, r9
        movss xmm6, dword ptr [rax+20h+8]
        ; save registers end
        mov rcx, rdx
        call rva1BCD40
        ; initialize pointer end
        lea rcx, [rbp-30h]
        call rva1BCBA0
        ;mov [rbp-30h], r13d
        movd xmm1, r13d
        cvtdq2ps xmm1, xmm1
        movaps xmm0, xmm1
        mulss xmm0, _Common_F0P9
        cvttss2si eax, xmm0
        mov [rbp-2Ch], eax
        mulss xmm1, _Common_F1P1
        cvttss2si eax, xmm1
        mov [rbp-28h], eax
        mov dword ptr [rbp-24h], 1 ; is alpha texture?
        mov byte ptr [rbp+10h], 1
        movaps xmm0, [rdi+0A0h]
        movaps [rbp+20h], xmm0
        movaps xmm1, [rdi+90h]
        movaps xmm0, xmm1
        shufps xmm0, xmm1, 0FFh
        mulps xmm1, _CommonRVA_EE80D0
        movaps [rsp+30h], xmm1
        movss dword ptr [rsp+3Ch], xmm0
        movaps xmm0, [rsp+30h]
        movdqa [rbp+30h], xmm0
        mov dword ptr [rbp+50h], 0
        movss xmm1, _Common_2PI
        movss dword ptr [rbp+54h], xmm1
        movss xmm2, dword ptr [rdi+374h] ; ammo size
        mulss xmm2, _Common_F5P0
        movaps xmm0, xmm2
        mulss xmm0, _Common_F0P05
        movss dword ptr [rbp+64h], xmm0
        movaps xmm1, xmm2
        mulss xmm1, _Common_F0P1
        movss dword ptr [rbp+68h], xmm1
        movss xmm0, _Common_F0P005
        movss dword ptr [rbp+6Ch], xmm0
        mov dword ptr [rbp+58h], 0 ; Smoke dispersal H
        mulss xmm2, xmm6
        movss dword ptr [rbp+5Ch], xmm2
        mov dword ptr [rbp-18h], 3C23D70Ah ; Smoke dispersal W
        movaps xmm0, _CommonRVA_EE7350
        movaps [rbp+70h], xmm0
        movaps xmm1, _CommonRVA_EE6BE0
        movaps [rbp+80h], xmm1
        mov word ptr [rbp+90h], 101h
        ;lea r8, AmmoTailSmoke2Texture
        mov r8, r15
        mov rcx, vedf125ABA0
        lea rdx, [rbp-70h]
        mov rcx, [rcx]
        call rva1ACA80
        mov rbx, rax
        lea rcx, [rbp+0A0h]
        mov rax, [rbp+0A0h]
        call qword ptr [rax+8]
        mov rax, [rbx+8]
        mov [rbp+0A8h], rax
        lea rdx, [rbx+10h]
        lea rcx, [rbp+0B0h]
        call rva6E160
        lea rdx, [rbx+20h]
        lea rcx, [rbp+0C0h]
        call rva6E0D0
        lea rcx, [rbp+0D0h]
        call rva70730
        mov rax, [rbx+30h]
        mov [rbp+0D0h], rax
        mov rbx, [rbx+38h]
        mov [rbp+0D8h], rbx
        test rbx, rbx
        je ofs14F536
        add rbx, 28h
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov rax, [rbp+0D0h]
        inc dword ptr [rax+58h]
        test rbx, rbx
        je ofs14F536
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs14F536:
        mov rbx, vedfE940C0
        mov [rbp-70h], rbx
        lea rcx, [rbp-40h]
        call rva70730
        mov rax, vedfE94130
        mov [rbp-70h], rax
        mov rcx, [rbp-68h]
        test rcx, rcx
        je ofs14F579
        lea rdx, [rbp-60h]
        call rva5C8410
        lea rdx, [rbp-50h]
        mov rcx, [rbp-68h]
        call rva5C8660
        mov [rbp-68], r14
    ofs14F579:
        lea rcx, [rbp-50h]
        call rva707B0
        lea rcx, [rbp-60h]
        call rva70850
        mov dword ptr [rbp+0E8h], 0
        mov dword ptr [rbp+0ECh], 3F000000h
        mov dword ptr [rbp+0F4h], 3F000000h
        mov byte ptr [rbp+0F8h], 1
        lea rdx, [rbp-30h]
        mov rcx, rsi
        call rva1BCDF0
        mov [rbp+0A0h], rbx
        lea rcx, [rbp+0D0h]
        call rva70730
        lea rcx, [rbp+0A0h]
        call rva5C6C80

        mov rbx, [rsp+270h]
        movaps xmm6, [rsp+210h]
        add rsp, 220h
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbp
        ret 
        int 3

ASMinitializeAmmoTailSmoke1 ENDP

END