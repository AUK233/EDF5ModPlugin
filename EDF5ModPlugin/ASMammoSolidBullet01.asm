.data

extern ammoSolidBullet01RetAddr : qword

extern edf1BCBA0Address : qword
extern edf1ACA80Address : qword
extern edf6E160Address : qword
extern edf6E0D0Address : qword
extern edf70730Address : qword
extern RtlEnterCriticalSection : qword
extern RtlLeaveCriticalSection : qword
extern vedfE940C0 : qword
extern vedfE94130 : qword
extern edf5C8410Address : qword
extern edf707B0Address : qword
extern edf70850Address : qword
extern edf1BCDF0Address : qword
extern edf5C6C80Address : qword
extern edf5C8660Address : qword
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
        call edf1BCBA0Address

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
        call edf1ACA80Address
        mov rbx, rax
        lea rcx, qword ptr [rbp+0A0h]
        mov rax, qword ptr [rbp+0A0h]
        call qword ptr [rax+8]
        mov rax, qword ptr [rbx+8]
        mov qword ptr [rbp+0A8h], rax
        lea rdx, qword ptr [rbx+10h]
        lea rcx, qword ptr [rbp+0B0h]
        call edf6E160Address
        lea rdx, qword ptr [rbx+20h]
        lea rcx, qword ptr [rbp+0C0h]
        call edf6E0D0Address
        lea rcx, qword ptr [rbp+0D0h]
        call edf70730Address
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
        call edf70730Address
        mov rax, vedfE94130
        mov qword ptr [rbp-70h], rax
        mov rcx, qword ptr [rbp-68h]
        test rcx, rcx
        je ofs14F579
        lea rdx, qword ptr [rbp-60h]
        call edf5C8410Address
        lea rdx, qword ptr [rbp-50h]
        mov rcx, qword ptr [rbp-68h]
        call edf5C8660Address
        mov qword ptr [rbp-68h], 0
    ofs14F579:
        lea rcx, qword ptr [rbp-50h]
        call edf707B0Address
        lea rcx, qword ptr [rbp-60h]
        call edf70850Address
        mov dword ptr [rbp+0E8h], 0
        mov dword ptr [rbp+0ECh], 3F000000h
        mov dword ptr [rbp+0F4h], 0
        mov byte ptr [rbp+0F8h], 1
        lea rdx, qword ptr [rbp-30h]
        lea rcx, qword ptr [rdi+630h]
        call edf1BCDF0Address
        ;mov byte ptr [rdi+878h],1
        mov qword ptr [rbp+0A0h], rbx
        lea rcx, qword ptr [rbp+0D0h]
        call edf70730Address
        lea rcx, qword ptr [rbp+0A0h]
        call edf5C6C80Address
        
        add rsp, 200h
        pop rbx
        pop rbp
        ret

ASMammoSolidBullet01t ENDP

END