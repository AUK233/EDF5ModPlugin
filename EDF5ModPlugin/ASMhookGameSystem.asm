.data

extern edf5BDF30Address : qword

extern readMissionSavaDataRetAddr : qword

extern pickupBoxRangeFRetAddr : qword
extern pickupBoxRangeTRetAddr : qword
; 25.0f
pickupRangePlusPlus dd 41C80000h
align 16
; L"PackageName"
wPackageName db 80,0,97,0,99,0,107,0,97,0,103,0,101,0,78,0,97,0,109,0,101,0,0,0

.code

ASMreadMissionSavaData proc

        lea rdx, wPackageName
        lea rcx, [r12+10h]
        call edf5BDF30Address
        movsxd rcx, eax
        cmp ecx, -1
        jne ofs8FDFB
        mov rax, r13
        jmp ofs8FE0F
    ofs8FDFB:
        mov rax, [r12+10h]
        movsxd rdx, dword ptr [rax+12]
        add rdx, rax
        lea rcx, [rcx+rcx*2]
        lea rax, [rdx+rcx*4]
    ofs8FE0F:
        movsxd r8, dword ptr [rax+8]
        add r8, rax
        mov r9d, [rbp-7Dh]

    checkMain:
        cmp r9d, 1
        jne checkMP01
        mov r9d, 0
        jmp ofs8FE1A
    checkMP01:
        cmp r9d, 3
        jne checkMP02
        mov r9d, 2
        jmp ofs8FE1A
    checkMP02:
        cmp r9d, 5
        jne ofs8FE1A
        mov r9d, 4

    ofs8FE1A:
        jmp readMissionSavaDataRetAddr
        int 3

ASMreadMissionSavaData ENDP

align 16

ASMpickupBoxRange proc

        shufps xmm0, xmm0, 0AAh
        addss xmm1, xmm0
        movaps xmm0, xmm7
        mulss xmm0, xmm7
        ; weapon = 0, armor = 1
        cmp dword ptr [rdi+0A8h], 1
        jle getBoxBlock
        jmp pickupBoxRangeFRetAddr

    getBoxBlock:
        ; check total, A88h is weapon, A8Ch is armor
        cmp dword ptr [rsi+0A84h], 128
        jg rangeBlock
        jmp pickupBoxRangeTRetAddr

    rangeBlock:
        ; xmm0 is player, xmm1 is item
        mulss xmm0, pickupRangePlusPlus
        jmp pickupBoxRangeFRetAddr
        int 3

ASMpickupBoxRange ENDP

END