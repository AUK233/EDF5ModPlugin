.data

extern rva94A70 : qword
extern _CommonRVA_EE7320 : xmmword
extern rva6170C0 : qword

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

.code

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

        mov rdx, r12
        lea rcx, [rbp-69h]
        call ASMLaserCollisionDetection
        test eax, eax
        mov [r14+144h], eax ; hit status
        jne ofs389E28

    ofs389E18:
        sub rdi, 1
        je ofs389E28
        movaps xmm6, [rbp-69h]
        movaps xmm7, [rbp-49h]
        jmp ofs389DC2

    ofs389E28:
        sub r13, rdi
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