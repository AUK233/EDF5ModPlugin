.data


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
extern edf15FD90Address : qword
extern edf155B80Address : qword
extern edf1727E0Address : qword
extern edf139950Address : qword

.code

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectPlasmaBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectSolidBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectPulseBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectSolidExpBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectHomingLaserBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectLaserBullet02 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectLightningBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectLaserBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectRocketBullet01 ENDP

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
        jmp giantBeeAmmoSetRetAddr
    ofs210609:
        mov rbp, r12
        jmp giantBeeAmmoSetRetAddr
        int 3

ASMInsectBeamBullet01 ENDP

END