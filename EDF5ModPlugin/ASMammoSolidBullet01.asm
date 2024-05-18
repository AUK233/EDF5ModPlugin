.data

extern ammoSolidBullet01RetAddr : qword

extern rva1BCBA0 : qword
extern rva1ACA80 : qword
extern rva6E160 : qword
extern rva6E0D0 : qword
extern rva70730 : qword
extern RtlEnterCriticalSection : qword
extern RtlLeaveCriticalSection : qword
extern vedfE940C0 : qword
extern vedfE94130 : qword
extern rva5C8410 : qword
extern rva707B0 : qword
extern rva70850 : qword
extern rva1BCDF0 : qword
extern rva5C6C80 : qword
extern rva5C8660 : qword
extern vedf125AB68 : qword

sb01smoke DB 59h, 71h, 5Fh, 00h, 30h, 00h, 31h, 00h, 2Eh, 00h, 64h, 00h, 64h, 00h, 73h, 00h, 00h, 00h

.code

ASMammoSolidBullet01 proc
    ; original
        movsxd rax, dword ptr [rdx+8]
        mov eax, dword ptr [rax+rdx+32]
        mov dword ptr [rdi+70Ch], eax
        ;call ASMammoSolidBullet01t
    ofs184BF0:
        jmp ammoSolidBullet01RetAddr
        int 3

ASMammoSolidBullet01 ENDP

ASMammoSolidBullet01t proc
        push rbp
        push rbx
        sub rsp, 200h
        lea rbp, [rsp+100h]
    ; 14F361
        lea rcx, qword ptr [rbp-30h]
        call rva1BCBA0

        mov eax, 60
        mov dword ptr [rbp-2Ch],eax
        add eax, 10
        mov dword ptr [rbp-28h], eax
        mov dword ptr [rbp-24h], 1
        mov byte ptr [rbp+10h], 1
        movups xmm0, xmmword ptr [rdi+0A0h]
        movaps xmmword ptr [rbp+20h], xmm0
        movaps xmm1, xmmword ptr [rdi+90h]
        movaps xmm0, xmm1
        shufps xmm0, xmm1, 0FFh
        ;mulps xmm1,xmmword ptr [1404A80D0]
        movaps xmmword ptr [rsp], xmm1
        ;movss dword ptr [rsp+3Ch], xmm0
        movaps xmm0,xmmword ptr [rsp]
        movdqa xmmword ptr [rbp+30h], xmm0
        mov dword ptr [rbp+50h], 0
        mov dword ptr [rbp+54h], 40C90FDBh
        mov dword ptr [rbp+64h], 3D4CCCCDh
        mov dword ptr [rbp+68h], 3DCCCCCDh
        mov dword ptr [rbp+6Ch], 3BA3D70Ah
        mov dword ptr [rbp+58h], 0
        mov dword ptr [rbp+5Ch], 3DCCCCCDh
        mov dword ptr [rbp-18h], 3F666666h
        mov dword ptr [rbp+70h], 3F800000h
        mov dword ptr [rbp+74h], 3F800000h
        mov dword ptr [rbp+78h], 3F800000h
        mov dword ptr [rbp+7Ch], 3F800000h
        mov dword ptr [rbp+80h], 3F800000h
        mov dword ptr [rbp+84h], 3F800000h
        mov dword ptr [rbp+88h], 3F800000h
        mov dword ptr [rbp+8Ch], 0
        mov word ptr [rbp+90h], 101h
        lea r8, sb01smoke
        lea rdx, [rbp-70h]
        mov rcx, vedf125AB68
        mov rcx, [rcx]
        call rva1ACA80
        mov rbx, rax
        lea rcx, qword ptr [rbp+0A0h]
        mov rax, qword ptr [rbp+0A0h]
        call qword ptr [rax+8]
        mov rax, qword ptr [rbx+8]
        mov qword ptr [rbp+0A8h], rax
        lea rdx, qword ptr [rbx+10h]
        lea rcx, qword ptr [rbp+0B0h]
        call rva6E160
        lea rdx, qword ptr [rbx+20h]
        lea rcx, qword ptr [rbp+0C0h]
        call rva6E0D0
        lea rcx, qword ptr [rbp+0D0h]
        call rva70730
        mov rax, qword ptr [rbx+30h]
        mov qword ptr [rbp+0D0h], rax
        mov rbx, qword ptr [rbx+38h]
        mov qword ptr [rbp+0D8h], rbx
        test rbx, rbx
        je ofs14F536
        add rbx, 28h
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov rax, qword ptr [rbp+0D0h]
        inc dword ptr [rax+58h]
        test rbx, rbx
        je ofs14F536
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs14F536:
        mov rbx, vedfE940C0
        mov qword ptr [rbp-70h], rbx
        lea rcx, qword ptr [rbp-40h]
        call rva70730
        mov rax, vedfE94130
        mov qword ptr [rbp-70h], rax
        mov rcx, qword ptr [rbp-68h]
        test rcx, rcx
        je ofs14F579
        lea rdx, qword ptr [rbp-60h]
        call rva5C8410
        lea rdx, qword ptr [rbp-50h]
        mov rcx, qword ptr [rbp-68h]
        call rva5C8660
        mov qword ptr [rbp-68h], 0
    ofs14F579:
        lea rcx, qword ptr [rbp-50h]
        call rva707B0
        lea rcx, qword ptr [rbp-60h]
        call rva70850
        mov dword ptr [rbp+0E8h], 0
        mov dword ptr [rbp+0ECh], 3F000000h
        mov dword ptr [rbp+0F4h], 0
        mov byte ptr [rbp+0F8h], 1
        lea rdx, qword ptr [rbp-30h]
        lea rcx, qword ptr [rdi+630h]
        call rva1BCDF0
        ;mov byte ptr [rdi+878h],1
        mov qword ptr [rbp+0A0h], rbx
        lea rcx, qword ptr [rbp+0D0h]
        call rva70730
        lea rcx, qword ptr [rbp+0A0h]
        call rva5C6C80
        
        add rsp, 200h
        pop rbx
        pop rbp
        ret

ASMammoSolidBullet01t ENDP

END