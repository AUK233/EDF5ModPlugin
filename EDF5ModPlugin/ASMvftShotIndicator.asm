.data

extern snd_ShotIndicatorCircle : byte

extern rva616F30 : qword
extern vedfEBBFB8 : qword

extern rva94A70 : qword
extern _CommonRVA_EE7320 : xmmword
extern rva6170C0 : qword
extern vedf125B080 : qword
extern jmp_Umbra_Object_create : qword
extern jmp_Umbra_ReferenceCount_release : qword
extern jmp_Umbra_ReferenceCount_setUserPointer : qword
extern jmp_Umbra_Object_setBitmask : qword
extern jmp_Umbra_Object_setCell : qword
extern jmp_Umbra_Object_setRenderCost : qword
extern jmp_Umbra_Object_set : qword
extern _CommonRVA_EE80E0 : xmmword
extern _CommonRVA_EE7DE0 : xmmword
extern _CommonRVA_EE79C0 : xmmword
extern _CommonRVA_EE7C10 : xmmword
extern _CommonRVA_EE7DD0 : xmmword
extern _CommonRVA_EE7940 : xmmword
extern _Common_F1P0N : dword
extern rva5E0660 : qword
extern _CommonRVA_EE7AB0 : xmmword
extern __sqrtfAddr : qword
extern rva42460 : qword
extern vedf1259680 : qword
extern rva5CC830 : qword
extern rva27570 : qword
extern vedf1153550 : qword
extern vedf1256BD0 : qword
extern rva5EE9E0 : qword
extern _Common_F0P05 : dword
extern _ShotIndicatorCircleColor1 : xmmword
extern _ShotIndicatorCircleColor2 : xmmword
extern _ShotIndicatorCircleEdgeColor : xmmword

extern vedf125AB68 : qword
extern _CommonRVA_EE7700 : xmmword
extern hktCEAF78 : qword
extern rvaAB720 : qword
extern rvaABED0 : qword
extern rva10EA000 : qword

extern _Common_2PI : dword
extern _Common_F60P0 : dword
extern _Common_F8P0 : dword
extern __sinfAddr : qword
extern _Common_F0P8 : dword
extern _Common_F5P0 : dword
extern vedf1259640 : qword
extern rva9DD10 : qword
extern m4x4_118C710 : qword
extern rva5EE2A0 : qword

; L"_Shot_Indicator_Circle_"
_ShotIndicatorCircle db 95,0,83,0,104,0,111,0,116,0,95,0,73,0,110,0,100,0,105,0,99,0,97,0,116,0,111,0,114,0,95,0,67,0,105,0,114,0,99,0,108,0,101,0,95,0,0,0

.code

ASMrva3B4F40mod proc

        mov rax, rsp
        push rbp
        push r12
        push r13
        push r14
        push r15
        lea rbp, [rsp-60h]
        sub rsp, 160h
        mov qword ptr [rbp-80], -2
        mov [rax+10h], rbx
        mov [rax+18h], rsi
        mov [rax+20h], rdi
        movaps [rax-38h], xmm6
        mov rbx, rcx
        mov [rbp-78h], rcx
        call rva616F30
        mov rax, vedfEBBFB8
        mov [rbx], rax
        ;
        xor r12d, r12d
        xorps xmm6, xmm6
        mov [rbx+68h], r12
        mov [rbx+70h], r12
        mov [rbx+78h], r12
        mov qword ptr [rbx+80h], 3F800000h
        mov [rbx+88h], r12
        mov [rbx+90h], r12d
        mov qword ptr [rbx+94h], 3F800000h
        mov [rbx+9Ch], r12d
        mov [rbx+0A0h], r12
        mov dword ptr [rbx+0A8h], 3F800000h
        mov [rbx+0B4h], r12
        mov [rbx+0ACh], r12
        mov dword ptr [rbx+0BCh], 3F800000h
        mov [rbx+0C0h], r12
        mov [rbx+0C8h], r12
        mov [rbx+0E8h], r12
        mov [rbx+0F0h], r12
        mov [rbx+0F8h], r12
        mov [rbx+108h], r12
        mov qword ptr [rbx+100h], 3F800000h
        mov [rbx+110h], r12d
        mov qword ptr [rbx+114h], 3F800000h
        mov [rbx+11Ch], r12d
        mov [rbx+120h], r12
        mov dword ptr [rbx+128h], 3F800000h
        mov [rbx+12Ch], r12
        mov [rbx+134h], r12
        mov dword ptr [rbx+13Ch], 3F800000h
        mov [rbx+190h], r12
        mov [rbx+198h], r12
        mov [rbx+1A0h], r12
        mov [rbx+1A8h], r12
        mov [rbx+1B0h], r12
        mov [rbx+1B8h], r12

        mov qword ptr [rsp+40h], 3F800000h
        mov [rsp+48h], r12
        mov [rsp+50h], r12d
        mov qword ptr [rsp+54h], 3F800000h
        mov [rsp+5Ch], r12d
        mov [rsp+60h], r12
        mov dword ptr [rsp+68h], 3F800000h
        mov [rsp+6Ch], r12
        mov [rsp+74h], r12
        mov dword ptr [rsp+7Ch], 3F800000h
        ; end
        mov rcx, vedf125AB68
        mov r13, [rcx]
        mov rdx, vedf125B080
        mov rax, [rdx]
        mov rcx, [rax+50h]
        call jmp_Umbra_Object_create

        mov r14, rax
        mov rcx, [rbx+10h]
        test rcx, rcx
        je ofs3B450A
        call jmp_Umbra_ReferenceCount_release
        mov [rbx+10h], r12
    ofs3B450A:
        test r14, r14
        je ofs3B4513
        mov [rbx+10h], r14
    ofs3B4513:
        mov rdx, rbx
        mov rcx, [rbx+10h]
        call jmp_Umbra_ReferenceCount_setUserPointer
        mov dword ptr [rbx+20h], 10000000h
        lea rdx, [rsp+40h]
        mov rcx, rbx
        call rva6170C0
        mov edx, [rbx+20h]
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_setBitmask
        mov rdx, [r13+20h]
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_setCell
        and dword ptr [rbx+20h], 0EFFFFFFFh
        mov edx, [rbx+20h]
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_setBitmask
        or dword ptr [rbx+20h], 20000000h
        mov edx, [rbx+20h]
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_setBitmask
        mov edx, 1
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_setRenderCost
        xor r8d, r8d
        mov r13d, 2
        mov edx, r13d
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_set
        xor r8d, r8d
        xor edx, edx
        mov rcx, [rbx+10h]
        call jmp_Umbra_Object_set
        ;
        movaps xmm0, _CommonRVA_EE80E0
        movaps [rbp-40h], xmm0
        movaps xmm1, _CommonRVA_EE7DE0
        movaps [rbp-30h], xmm1
        movaps xmm0, _CommonRVA_EE79C0
        movaps [rbp-20h], xmm0
        movaps xmm1, _CommonRVA_EE7C10
        movaps [rbp-10h], xmm1
        movaps xmm0, _CommonRVA_EE7DD0
        movaps [rbp], xmm0
        movaps xmm1, _CommonRVA_EE7940
        movaps [rbp+10h], xmm1
        xorps xmm0, xmm0
        movaps [rbp+20h], xmm0
        movss xmm1, _Common_F1P0N
        movss dword ptr [rbp+30h], xmm1
        lea ecx, [r13+2]
        mov dword ptr [rbp+34h], 0
        mov dword ptr [rbp-70h], 40000h
        mov dword ptr [rbp-6Ch], 10001h
        mov dword ptr [rbp-68h], 30004h
        mov dword ptr [rbp-64h], 40003h
        mov dword ptr [rbp-60h], 20002h
        mov dword ptr [rbp-5Ch], ecx
        mov dword ptr [rbp-58h], 20000h
        mov dword ptr [rbp-54h], 10001h
        mov dword ptr [rbp-50h], 30002h
        mov dword ptr [rsp+30h], ecx
        mov dword ptr [rsp+28h], 12h
        lea rax, [rbp-70h]
        mov qword ptr [rsp+20h], rax
        ; end
        lea r9d, [r13+3]
        lea r8, [rbp-40h]
        lea rcx, [rbx+0C0h]
        call rva5E0660

        movaps xmm0, _CommonRVA_EE7AB0
        movaps xmm1, xmm0
        shufps xmm1, xmm0, 055h
        addss xmm1, xmm0
        shufps xmm0, xmm0, 0AAh
        addss xmm1, xmm0
        ucomiss xmm1, xmm6
        jp ofs3B4691
        je ofs3B469C
    ofs3B4691:
        movaps xmm0, xmm1
        call __sqrtfAddr
        movaps xmm6, xmm0
    ofs3B469C:
        movss dword ptr [rbx+1CCh], xmm6
        mov dword ptr [rbx+1D0h], 41080000h
        lea rdx, _ShotIndicatorCircle
        lea rcx, [rbp-70h]
        call rva42460

        mov rdx, vedf1259680
        mov rax, [rdx]
        add rax, 550h
        mov [rsp+20h], rax
        lea r9, snd_ShotIndicatorCircle
        lea r8, [rbp-70h]
        lea rcx, [rbx+1A8h]
        call rva5CC830

        mov rdx, [rbp-58h]
        cmp rdx, 8
        jb ofs3B4702
        inc rdx
        mov r8, r13
        mov rcx, [rbp-70h]
        call rva27570

    ofs3B4702:
        mov r10, [rbx+1B0h]
        mov rax, [r10]
        mov rdx, [rax+58h]
        mov rax, [rdx+8h]
        imul r8, [rax+20h], 0B8h
        mov rax, [r10]
        add r8, [rax+0B8h]
        mov rdx, [r8+20h]
        mov rax, [r8+30h]
        mov [rsp+28h], rax
        mov [rsp+20h], rdx
        mov r9d, r13d
        mov r8, vedf1153550
        mov rdx, vedf1256BD0
        lea rcx, [rbx+190h]
        call rva5EE9E0
        ;
        mov dword ptr [rbx+0D8h], r12d
        mov dword ptr [rbx+1C0h], 41A00000h
        mov dword ptr [rbx+1C4h], 3F000000h
        mov eax, _Common_F0P05
        mov [rbx+1C8h], eax ; +1C8 is g_edge_size => +148
        ;mov byte ptr [rbx+1D4h], 0 ; => +150
        ;mov byte ptr [rbx+1D5h], 0 ; no scroll animation
        ;mov byte ptr [rbx+1D6h], 0 ; is not a ring.
        mov [rbx+1D4h], r12d
        movaps xmm0, _ShotIndicatorCircleColor1
        movups [rbx+1D8h], xmm0
        movaps xmm1, _ShotIndicatorCircleColor2
        movups [rbx+1E8h], xmm1
        movaps xmm2, _ShotIndicatorCircleEdgeColor
        movups [rbx+1F8h], xmm2
        ; end
        mov rax, rbx
        lea r11, [rsp+160h]
        mov rbx, [r11+38h]
        mov rsi, [r11+40h]
        mov rdi, [r11+48h]
        movaps xmm6, [r11-10h]
        mov rsp, r11
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret 
        int 3

ASMrva3B4F40mod ENDP

align 16

ASMrva38A960mod proc

        mov rax, rsp
        mov [rax+8h], rbx
        mov [rax+10h], rsi
        mov [rax+18h], rdi
        push rbp
        push r12
        push r13
        push r14
        push r15
        lea rbp, [rax-6Fh]
        sub rsp, 0D0h
        movaps [rax-38h], xmm6
        movaps [rax-48h], xmm7
        movaps [rax-58h], xmm8
        movaps [rax-68h], xmm9

        mov rsi, r9
        movaps xmm6, [rdx]
        mov r15, rdx
        mov r14, rcx
        movaps xmm7, [r8]
        mov edi, [rax+20h+8]
        mov r13, rdi
        movaps [rbp-49h], xmm7
        movss xmm9, dword ptr [rbp-3Dh]
        movaps [rbp-69h], xmm6
        movss xmm8, dword ptr [rbp-5Dh]
        mov qword ptr [rcx+78h], 0

        mov rdx, rdi
        shr rdx, 1
        jmp ofs389DCC

        align 16
    ofs389DC2:
        mov rdx, [r14+70h]
        cmp [r14+78h], rdx
        jb ofs389DE1
    ofs389DCC:
        lea rdx, [rdx*2+2]
        lea rcx, [r14+60h]
        call rva94A70
        test al, al
        mov r12, [r14+68h]
        je ofs389DFE
    ofs389DE1:
        mov rax, [r14+78h]
        mov rcx, rax
        shl rcx, 4
        lea rax, [rax+1]
        mov [r14+78h], rax
        add rcx, [r14+68h]
        je ofs389DFE
        movaps [rcx], xmm6
        mov r12, rcx
    ofs389DFE:
        addps xmm6, xmm7
        addps xmm7, [rsi]
        movaps [rbp-69h], xmm6
        movaps [rbp-49h], xmm7
        movss dword ptr [rbp-3Dh], xmm9
        movss dword ptr [rbp-5Dh], xmm8
    ; check collision
        mov rdx, r12
        lea rcx, [rbp-69h]
        call ASMLaserCollisionDetection
        test eax, eax
        mov [r14+144h], eax ; hit status
        je ofs389E18
        sub r13, rdi
        cmp r13d, 1
        jg ofs389E28
        add r13, 2
        movaps [r12+10h], xmm6
        movaps [r12+20h], xmm6
        mov [r14+78h], r13
        jmp ofs389E28

    ofs389E18:
        sub rdi, 1
        je ofs389E28
        movaps xmm6, [rbp-69h]
        movaps xmm7, [rbp-49h]
        jmp ofs389DC2
        
    ofs389E28:
        mov [r14+148h], r13d ; collision length
        movaps xmm5, [r15]
        movss xmm4, dword ptr [rbp-61h]
        movaps xmm2, xmm5
        movss xmm7, dword ptr [rbp-69h]
        movss xmm6, dword ptr [rbp-65h]
        mov eax, [r14+108h]
        movss dword ptr [rbp-41h], xmm4
        movss dword ptr [rbp-49h], xmm7
        movss dword ptr [rbp-45h], xmm6
        movss dword ptr [rbp-3Dh], xmm8
        movaps xmm1, [rbp-49h]
        subps xmm1, xmm5
        movss dword ptr [rbp-3Dh], xmm8
        mov [r14+0D8h], eax
        mov qword ptr [rbp-35h], 0
        mov qword ptr [rbp-2Dh], 0
        mov qword ptr [rbp-21h], 0
        movss dword ptr [rbp-49h], xmm1
        movaps xmm0, xmm1
        shufps xmm0, xmm1, 55h
        movss dword ptr [rbp-45h], xmm0
        shufps xmm1, xmm1, 0AAh
        movss dword ptr [rbp-41h], xmm1
        movaps xmm1, [rbp-49h]
        ;mulps xmm1, _CommonRVA_EE7320
        mov qword ptr [rbp-19h], 0
        movss dword ptr [rbp-3Dh], xmm8
        mov dword ptr [rbp-0Dh], 0
        movss dword ptr [rbp-49h], xmm1
        movaps xmm0, xmm1
        shufps xmm0, xmm1, 55h
        movss dword ptr [rbp-45h], xmm0
        shufps xmm1, xmm1, 0AAh
        movss dword ptr [rbp-41h], xmm1
        movaps xmm1, xmm5
        addps xmm2, [rbp-49h]
        shufps xmm1, xmm5, 0FFh
        movss dword ptr [rbp-5Dh], xmm1
        movss dword ptr [rbp-41h], xmm4
        movss dword ptr [rbp-49h], xmm7
        movaps xmm0, xmm2
        movss dword ptr [rbp-69h], xmm2
        shufps xmm0, xmm2, 55h
        movss dword ptr [rbp-65h], xmm0
        shufps xmm2, xmm2, 0AAh
        movss dword ptr [rbp-61h], xmm2
        movss dword ptr [rbp-45h], xmm6
        movss dword ptr [rbp-3Dh], xmm8
        movaps xmm1, [rbp-49h]
        subps xmm1, xmm5
        movss dword ptr [rbp-3Dh], xmm8
        movaps xmm0, xmm1
        movss dword ptr [rbp-49h], xmm1
        shufps xmm0, xmm1, 55h
        movss dword ptr [rbp-45h], xmm0
        shufps xmm1, xmm1, 0AAh
        movss dword ptr [rbp-41h], xmm1
        movss xmm1, dword ptr [r14+130h]
        movaps xmm2, [rbp-49h]
        ;mulps xmm2, _CommonRVA_EE7320
        movaps xmm0, xmm2
        addss xmm0, xmm1
        movss dword ptr [rbp-39h], xmm0
        movaps xmm0, xmm2
        shufps xmm0, xmm2, 55h
        addss xmm0, xmm1
        shufps xmm2, xmm2, 0AAh
        addss xmm2, xmm1
        movss dword ptr [rbp-25h], xmm0
        movaps xmm0, [rbp-69h]
        movaps [rbp-9h], xmm0 ; rbx+40h
        movss dword ptr [rbp-11h], xmm2

        ;mov rdx, r15
        ;lea rcx, [rbp-9h]
        ;call ASMLaserCollisionDetection

        lea rdx, [rbp-39h]
        mov rcx, r14
        call rva6170C0
        
        lea r11, [rsp+0D0h]
        mov rbx, [r11+30h]
        mov rsi, [r11+38h]
        mov rdi, [r11+40h]
        movaps xmm6, [r11-10h]
        movaps xmm7, [r11-20h]
        movaps xmm8, [r11-30h]
        movaps xmm9, [r11-40h]
        mov rsp, r11
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret 

ASMrva38A960mod ENDP

align 16

ASMLaserCollisionDetection proc

        mov rax, rsp
        push rbp
        push rbx
        push rsi
        push rdi
        push r14
        lea rbp, [rsp-80h]
        sub rsp, 180h
        mov qword ptr [rbp-80h], -2
        movaps [rax-38h], xmm6
        movaps [rax-48h], xmm8

        ; initialization
        mov rbx, rcx
        movaps xmm8, [rdx]
        movaps xmm6, [rbx]
        xor r14d, r14d
        xor edi, edi
        ; initialization detection
        xor edx, edx
        lea rcx, [rsp+40h]
        call rvaAB720
        mov dword ptr [rsp+50h], 10

        movaps xmm1, _CommonRVA_EE7700 
        mov [rsp+60h], r14d
        mov rcx, vedf125AB68
        mov rax, [rcx]
        mov rax, [rax+18h]
        mov rcx, [rax+88h]

        ;int 3
        movaps [rbp-70h], xmm8 ; start point
        movaps [rbp-60h], xmm6 ; end point
        movaps [rbp-50h], xmm1
        mov [rbp-40h], r14
        mov rax, [rcx]
        lea r8, [rsp+40h]
        lea rdx, [rbp-70h]
        call qword ptr [rax+0E8h]

        mov r8d, dword ptr [rsp+60h]
        test r8d, r8d
        je ofs38AA08
        mov [rbp-48h], r14
        movaps [rbp-70h], xmm8
        movaps [rbp-60h], xmm6
        mov byte ptr [rbp-50h], 1
        mov eax, [rsp+50h]
        mov [rbp-4Ch], eax
        mov rax, hktCEAF78
        mov [rbp-30h], rax
        mov rax, r14
        mov [rbp+30h], rax
        mov dword ptr [rbp-10h], 3F800000h
        mov dword ptr [rbp-0Ch], -1
        mov [rbp+20h], r14d
        mov dword ptr [rbp], -1
        mov dword ptr [rbp-28h], 3F800000h
        mov rdx, [rsp+58h]
        test rdx, rdx
        je ofs38A959
        lea r9, [rbp-30h]
        lea rcx, [rbp-70h]
        call rvaABED0
        mov rax, [rbp+30h]
    ofs38A959:
        test rax, rax
        je ofs38AA08
        mov edi, 1
        movss xmm1, dword ptr [rbx+0Ch]
        movss xmm0, dword ptr [rbp-10h]
        shufps xmm0, xmm0, 0
        subps xmm6, xmm8
        mulps xmm6, xmm0
        addps xmm6, xmm8
        movaps [rbx], xmm6
        movss dword ptr [rbx+0Ch], xmm1

    ofs38AA08:
        mov [rsp+60h], r14d
        mov r8d, [rsp+64h]
        test r8d, r8d
        js ofs38AA72
        mov rax, rva10EA000
        and r8d, 3FFFFFFFh
        shl r8d, 3
        mov r9, [rax]
        mov rdx, [rsp+58h]
        mov rcx, rax
        call qword ptr [r9+20h]

    ofs38AA72:
        mov eax, edi
        lea r11, [rsp+180h]
        movaps xmm6, [r11-10h]
        movaps xmm8, [r11-20h]
        mov rsp, r11
        pop r14
        pop rdi
        pop rsi
        pop rbx
        pop rbp
        ret 
        int 3

ASMLaserCollisionDetection ENDP

align 16

ASMweaponIndicatorLineFuncP8 proc

        mov r11, rsp
        push rbx
        push r15
        sub rsp, 0C48h+3000h

        cmp byte ptr [rcx+134h], 0
        mov r15, rdx
        mov rbx, rcx
        je ofs38A2DD
        cmp r8d, 2
        jne ofs38A2DD

        mov [r11+18h], rbp
        mov [r11+20h], rsi
        mov [r11-18h], rdi
        mov [r11-20h], r12
        mov [r11-28h], r14
        movaps [r11-38h], xmm6
        movaps [r11-48h], xmm7
        movaps [r11-58h], xmm8
        movaps [r11-68h], xmm9
        movaps [r11-78h], xmm10
        movaps [r11-88h], xmm11
        movaps [r11-98h], xmm12
        movaps [r11-0A8h], xmm13
        movaps [r11-0B8h], xmm14
        movaps [r11-0C8h], xmm15

        lea edx, [r8+3Ah]
        lea rax, [rsp+48h]
        xor r12d, r12d

        align 16
    ofs38A070:
        mov [rax-8], r12
        mov [rax], r12d
        mov dword ptr [rax+4], 3F800000h
        lea rax, [rax+30h]
        sub rdx, 1
        jne ofs38A070
        ; initialize
        movss xmm8, _Common_2PI
        lea rdi, [rsp+58h]
        mov rbp, [rcx+68h]
        mov esi, r12d
        xorps xmm7, xmm7
        movss xmm6, dword ptr [rcx+0D8h]
        movss xmm9, dword ptr [rcx+0DCh]
        movss xmm14, dword ptr [rcx+110h]
        movss xmm15, dword ptr [rcx+114h]
        movss xmm11, dword ptr [rcx+120h]
        movss xmm12, dword ptr [rcx+124h]
        movss xmm13, dword ptr [rcx+128h]
        movss xmm10, dword ptr [rcx+130h]
        mulss xmm6, xmm8
        mov r12d, [rbx+148h] ; length
        mov eax, 300
        cmp r12d, eax
        cmovg r12d, eax
        ; initialize end
        align 16
    ofs38A0F0:
        xorps xmm1, xmm1
        mov eax, esi
        movaps xmm0, xmm6
        cvtsi2ss xmm1, rax
        divss xmm1, _Common_F60P0
        mulss xmm1, _Common_F8P0
        mulss xmm1, xmm8
        subss xmm0, xmm1
        call __sinfAddr
        subss xmm0, _Common_F0P8
        movaps xmm1, xmm7
        movaps xmm2, xmm11
        movaps xmm3, xmm12
        movaps xmm4, xmm13
        movaps xmm5, xmm9

        inc esi
        mulss xmm0, _Common_F5P0
        maxss xmm1, xmm0
        movups xmm0, [rbp]
        add rbp, 10h
        movups [rdi-18h], xmm0
        mulss xmm3, xmm1
        movaps xmm0, xmm15
        mulss xmm5, xmm1
        mulss xmm2, xmm1
        addss xmm0, xmm3
        mulss xmm4, xmm1
        movaps xmm1, xmm14
        addss xmm1, xmm2
        movss dword ptr [rdi-4], xmm0
        movss xmm0, dword ptr [rbx+11Ch]
        addss xmm0, xmm5
        movss dword ptr [rdi-8], xmm1
        movss xmm1, dword ptr [rbx+118h]
        addss xmm1, xmm4
        movss dword ptr [rdi+4], xmm0
        movss dword ptr [rdi], xmm1
        movss dword ptr [rdi+0Ch], xmm10
        mov dword ptr [rdi+8], 3F000000h
        add rdi, 30h
        cmp esi, r12d
        jb ofs38A0F0

        movaps xmm15, [rsp+0B90h+3000h]
        movaps xmm14, [rsp+0BA0h+3000h]
        movaps xmm13, [rsp+0BB0h+3000h]
        movaps xmm12, [rsp+0BC0h+3000h]
        movaps xmm11, [rsp+0BD0h+3000h]
        movaps xmm10, [rsp+0BE0h+3000h]
        movaps xmm9, [rsp+0BF0h+3000h]
        movaps xmm8, [rsp+0C00h+3000h]
        movaps xmm6, [rsp+0C20h+3000h]
        mov rdi, [rsp+0C40h+3000h]
        mov rsi, [rsp+0C78h+3000h]
        mov rbp, [rsp+0C70h+3000h]

        xor r12d, r12d
        mov rax, vedf1259640
        mov rax, [rax]
        cmp [rax+0FB0h], r12b
        je ofs38A23B
        or dword ptr [rax+0FA8h], 4
        mov [rax+0FB0h], r12b
    ofs38A23B:
        cmp dword ptr [rax+0FB4h], 1
        je ofs38A255
        or dword ptr [rax+0FA8h], 10
        mov dword ptr [rax+0FB4h], 1
    ofs38A255:
        cmp dword ptr [rax+0FB8h], 2
        je ofs38A26F
        or dword ptr [rax+0FA8h], 20
        mov dword ptr [rax+0FB8h], 2
    ofs38A26F:
        lea r8, [rbx+0C0h]
        lea r9, [rbx+80h]
        call rva9DD10

        mov edx, [rbx+148h]
        mov [rsp+20h], edx
        ;mov dword ptr [rsp+20h], 60
        mov rax, vedf1259640
        mov rcx, [rax]
        lea r9, [rsp+40h]
        mov [rsp+38h], r12
        mov r8, m4x4_118C710
        movss dword ptr [rsp+30h], xmm7
        mov rdx, r15
        mov [rsp+28h], r12b
        call rva5EE2A0

        movaps xmm7, [rsp+0C10h+3000h]
        mov r14, [rsp+0C30h+3000h]
        mov r12, [rsp+0C38h+3000h]
    ofs38A2DD:
        add rsp, 0C48h+3000h
        pop r15
        pop rbx
        ret 
        int 3

ASMweaponIndicatorLineFuncP8 ENDP

END