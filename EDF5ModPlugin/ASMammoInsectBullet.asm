.data

extern giantBeeAmmoSetRetAddr : qword

extern __RTDynamicCastAddr : qword
extern rva79390 : qword
extern AVSceneObjectAddress : qword

extern aligned_mallocAddr : qword
extern RtlEnterCriticalSection : qword
extern RtlLeaveCriticalSection : qword
extern rva9C850 : qword
extern rva6E010 : qword

extern edf16DBB0Address : qword
extern AVPlasmaBullet01Address : qword

extern edf185510Address : qword
extern AVSolidBullet01Address : qword

extern edf171140Address : qword
extern AVPulseBullet01Address : qword

extern edf187EC0Address : qword
extern AVSolidExpBullet01Address : qword

extern edf150AD0Address : qword
extern AVHomingLaserBullet01Address : qword

extern edf156FF0Address : qword
extern AVLaserBullet02Address : qword

extern edf15FD90Address : qword
extern AVLightningBullet01Address : qword

extern edf155B80Address : qword
extern AVLaserBullet01Address : qword

extern edf1727E0Address : qword
extern AVRocketBullet01Address : qword

extern edf139950Address : qword
extern AVBeamBullet01Address : qword

extern edf136890Address : qword
extern edf14DE20Address : qword

.code

ASMrva2113F0mod proc

        push rdi
        sub rsp, 40h
        mov qword ptr [rsp+30h], -2
        mov [rsp+50h], rbx
        mov [rsp+58h], rsi
        mov rdi, rdx
        mov rbx, rcx
        xor esi, esi
        mov [rsp+20h], esi
        mov r9, r8
        mov r8, AVSceneObjectAddress
        xor edx, edx
        mov rcx, [rdi]
        call __RTDynamicCastAddr
        mov [rbx], rsi
        mov [rbx+8], rsi
        test rax, rax
        je ofs210854
        mov r8, [rdi+8]
        test r8, r8
        je ofs210848
        lock inc dword ptr [r8+8]
    ofs210848:
        mov rdx, rax
        mov rcx, rbx
        call rva79390
    ofs210854:
        mov rax, rbx
        mov rbx, [rsp+50h]
        mov rsi, [rsp+58h]
        add rsp, 40h
        pop rdi
        ret 
        int 3

ASMrva2113F0mod ENDP

align 16

ASMInsectPlasmaBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVPlasmaBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectPlasmaBullet01 ENDP

align 16

ASMInsectSolidBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVSolidBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectSolidBullet01 ENDP

align 16

ASMInsectPulseBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVPulseBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectPulseBullet01 ENDP

align 16

ASMInsectSolidExpBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVSolidExpBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectSolidExpBullet01 ENDP

align 16

ASMInsectHomingLaserBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVHomingLaserBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectHomingLaserBullet01 ENDP

align 16

ASMInsectLaserBullet02 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
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
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVLaserBullet02Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret 
        int 3

ASMInsectLaserBullet02 ENDP

align 16

ASMInsectLightningBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
        lea edx, [r12+10h]
        mov ecx, 880h ;LightningBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf15FD90Address
        mov rbp, rax
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVLightningBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret
        int 3

ASMInsectLightningBullet01 ENDP

align 16

ASMInsectLaserBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
        lea edx, [r12+10h]
        mov ecx, 750h ;LaserBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf155B80Address
        mov rbp, rax
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVLaserBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret
        int 3

ASMInsectLaserBullet01 ENDP

align 16

ASMInsectRocketBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
        lea edx, [r12+10h]
        mov ecx, 0A70h ;RocketBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf1727E0Address
        mov rbp, rax
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVRocketBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret
        int 3

ASMInsectRocketBullet01 ENDP

align 16

ASMInsectBeamBullet01 proc

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
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu xmmword ptr [rax-58h], xmm0
        mov qword ptr [r9+8h], r8
        mov qword ptr [r9+10h], r12
        mov qword ptr [r9+18h], r12
        lea rax, qword ptr [rax-58h]
        mov qword ptr [r9+20h],rax
    ; different
        lea edx, [r12+10h]
        mov ecx, 710h ;BeamBullet01Size
        call aligned_mallocAddr
        mov qword ptr [rsp+0A0h], rax
        test rax, rax
        je ofs210609
        mov rdx, rdi
        mov rcx, rax
        call edf139950Address
        mov rbp, rax
        jmp ofs21060C
    ofs210609:
        mov rbp, r12
    ofs21060C:
        mov rcx, [rsp+30h]
        test rcx, rcx
        sete al
        xor al, 1
        je ofs2106AB
        ; CriticalSection
        mov rax, [r14+130h]
        mov [rsp+0A0h], rax
        mov [rcx+38h], rax
        lea rbx, [r14+0C0h]
        mov [rsp+0A8h], rbx
        mov rcx, rbx
        call RtlEnterCriticalSection
        mov r15, [r14+110h]
        lea r9, [rsp+30h]
        mov r8, [r15+8h]
        mov rdx, r15
        call rva9C850
        mov rdx, rax
        mov rcx, 7FFFFFFFFFFFFFEh
        sub rcx, [r14+118h]
        cmp rcx, 1
        jae ofs21068B
        int 3 
    ofs21068B:
        inc qword ptr [r14+118h]
        mov [r15+8], rdx
        mov rax, [rax+8]
        mov [rax], rdx
        test rbx, rbx
        je ofs2106AB
        mov rcx, rbx
        call RtlLeaveCriticalSection
    ofs2106AB:
        mov [rdi+10h], r12
        mov [rdi+18h], r12
        mov [rdi+20h], r12
        test byte ptr [rbp+30h], 4
        je ofs2106DA
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rsp+38h]
        test rdi, rdi
        je ofs2107C9
        or ebx, -1
        jmp ofs2107A2
    ofs2106DA:
        lea rcx, [rbp+10h]
        lea rdx, [rsp+50h]
        call rva6E010
        mov r8, AVBeamBullet01Address
        mov rdx, rax
        lea rcx, [rsp+40h]
        call ASMrva2113F0mod
        mov [rsi], r12
        mov [rsi+8], r12
        mov rdi, [rax+8]
        mov rbp, [rax]
        test rdi, rdi
        je ofs21070C
        lock inc dword ptr [rdi+12]
    ofs21070C:
        mov rcx, [rsi+8]
        or ebx, -1
        test rcx, rcx
        je ofs21072A
        mov eax, ebx
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs21072A
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs21072A:
        mov [rsi+8], rdi
        mov [rsi], rbp
        mov rdi, [rsp+48h]
        test rdi, rdi
        je ofs210764
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210764
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210764:
        mov rdi, [rsp+58h]
        test rdi, rdi
        je ofs210798
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        mov eax, ebx
        lock xadd [rdi+12], eax
        cmp eax, 1
        jne ofs210798
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs210798:
        mov rdi, qword ptr [rsp+38h]
        test rdi, rdi
        je ofs2107C9
    ofs2107A2:
        mov eax, ebx
        lock xadd [rdi+8], eax
        cmp eax, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax]
        lock xadd [rdi+12], ebx
        cmp ebx, 1
        jne ofs2107C9
        mov rax, [rdi]
        mov rcx, rdi
        call qword ptr [rax+8]
    ofs2107C9:
        mov rax, rsi
        lea r11, [rsp+60h]
        mov rbx, [r11+30h]
        mov rbp, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop r12
        pop rdi
        pop rsi
        ret
        int 3

ASMInsectBeamBullet01 ENDP

END
