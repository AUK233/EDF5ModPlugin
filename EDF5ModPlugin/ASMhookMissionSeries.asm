.data

extern edf5BDF30Address : qword
extern _Common_F25P0 : dword

extern readMissionSavaDataRetAddr : qword
extern pickupBoxRangeFRetAddr : qword
extern pickupBoxRangeTRetAddr : qword


extern vedf125AB30 : qword
extern loadMissionTimelineLayerRetAddr : qword
extern pMissionCustomData : qword
extern CustomMissionData_initialization : proto
extern CustomMissionData_SetToCurrentMission : proto
extern CustomMissionData_MissionClearSetup : proto

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
        jle rangeBlock
        jmp pickupBoxRangeFRetAddr

    getBoxBlock:
        ; check total, A88h is weapon, A8Ch is armor
        cmp dword ptr [rsi+0A84h], 128
        jg rangeBlock
        jmp pickupBoxRangeTRetAddr

    rangeBlock:
        ; xmm0 is player, xmm1 is item
        mulss xmm0, _Common_F25P0
        jmp pickupBoxRangeFRetAddr
        int 3

ASMpickupBoxRange ENDP

align 16

ASMloadMissionTimelineLayer proc

        mov rcx, vedf125AB30
        mov rdx, [rcx]
        ; mission table pointer
        mov rax, [rdx+178h]
        test rax, rax
        je ofs113CB3
        ; mission count
        cmp dword ptr [rax+4], 1
        jbe ofs113CB3
        ; check mission index
        mov ebx, [rax+4]
        mov ecx, [rdx+48h]
        lea eax, [rbx-1]
        cmp ecx, eax
        cmova ecx, eax
        ; end
        mov rax, [rdx+178h]
        movsxd rdx, dword ptr [rax+8]
        add rdx, rax
        movsxd rax, ecx
        lea rcx, [rax+rax*2]
        lea rbx, [rdx+rcx*4] ; get current mission sgoptr
        ; check sgo node count
        movsxd rcx, dword ptr [rbx+8] ; get node offset
        mov edx, [rbx+4] ; get node count
        add rcx, rbx
        call CustomMissionData_SetToCurrentMission
        ; get 4th value
        movsxd rcx, dword ptr [rbx+8]
        movss xmm0, dword ptr [rcx+rbx+2Ch]
        jmp loadMissionTimelineLayerRetAddr
    ofs113CB3:
        mov rcx, pMissionCustomData
        call CustomMissionData_initialization
        xorps xmm0, xmm0
        jmp loadMissionTimelineLayerRetAddr

ASMloadMissionTimelineLayer ENDP

align 16

ASMcheckMissionEndCondition proc

        cmp esi, 1
        jne ofs11408E
        mov rcx, rbx
        call CustomMissionData_MissionClearSetup
    ofs11408E:
        mov rbx, [rsp+30h]
        mov rsi, [rsp+38h]
        add rsp, 20h
        pop rdi
        ret 
        int 3

ASMcheckMissionEndCondition ENDP

END