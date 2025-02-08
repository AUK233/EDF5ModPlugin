.data

extern ftimeBeginPeriod : proto
extern ftimeEndPeriod : proto
extern hookSleep : proto

extern eTextForWeaponReloadTime : proto
;extern eDisplaySoldierWeaponAmmo : proto
extern eDisplaySoldierWeaponDamage : proto
extern eDisplayVehicleWeaponDamage : proto
extern eDisplayFencerBoostAndDash : proto

extern HUiHudWeaponUpdateVehicleTextRet : qword
extern readHUiHudPowerGuageRet : qword
extern updateHUiHudPowerGuageRet : qword
extern hookSleepRet : qword
extern rva9C6E40 : qword
extern rva27380 : qword
extern rva27570 : qword
extern rvaB7220 : qword
extern rva4D86D0 : qword
extern rva4CA990 : qword

; L"TextNumericType2"
wstrTextNumericType2 db 84,0,101,0,120,0,116,0,78,0,117,0,109,0,101,0,114,0,105,0,99,0,84,0,121,0,112,0,101,0,50,0,0,0
align 16
; L"TextDamage"
wstrTextDamage db 84,0,101,0,120,0,116,0,68,0,97,0,109,0,97,0,103,0,101,0,0,0
align 16
; L"TextDamage_UP"
wstrTextDamageUP db 84,0,101,0,120,0,116,0,68,0,97,0,109,0,97,0,103,0,101,0,95,0,85,0,80,0,0,0
align 16
; L"TextFencerDash"
wstrTextFencerDash db 84,0,101,0,120,0,116,0,70,0,101,0,110,0,99,0,101,0,114,0,68,0,97,0,115,0,104,0,0,0
align 16
; L"TextFencerBoost"
wstrTextFencerBoost db 84,0,101,0,120,0,116,0,70,0,101,0,110,0,99,0,101,0,114,0,66,0,111,0,111,0,115,0,116,0,0,0

.code

ASMgetHUiHudTextContentPointer proc
        ; there have no call functions
        ; so stack does not need to be aligned
        ;sub rsp, 8
        mov rdx, qword ptr [rcx]
        mov rax, qword ptr [rdx]
        ;add rsp, 8
        ret
        int 3
ASMgetHUiHudTextContentPointer ENDP

align 16

ASMreadHUiHudWeapon proc

        xor r13, r13
        xorps xmm6, xmm6
        movaps [rdi+0D00h], xmm6
        movaps [rdi+0D10h], xmm6
        movaps [rdi+0D20h], xmm6
        movaps [rdi+0D30h], xmm6
        movaps [rdi+0D40h], xmm6
    ; start
        mov qword ptr [rbp+0E0h], 7
        mov [rbp+0D8h], r13
        movups [rbp+0C8h], xmm6
        mov r8d, 16 ; string length
        lea rdx, wstrTextNumericType2
        lea rcx, [rbp+0C8h]
        call rva27380
        align 16
        lea r8, [rbp+0C8h]
        lea rdx, [rbp+28h]
        mov rcx, rdi
        call rva4D86D0
        mov rbx, [rax+8]
        mov r15, [rax]
        test rbx, rbx
        je node2
        movups xmm0, [r15+270h]
        movups [rdi+0D10h], xmm0
        lock inc dword ptr [rbx+0Ch]
    ofs4D0BE1:
        mov rcx, [rdi+0D08h]
        test rcx, rcx
        je ofs4D0BFF
        mov eax, esi
        lock xadd [rcx+0Ch], eax
        cmp eax, 1
        jne ofs4D0BFF
        mov rax, [rcx]
        call qword ptr [rax+8]
        align 16
    ofs4D0BFF:
        mov [rdi+0D08h], rbx
        mov [rdi+0D00h], r15
        mov rbx, [rbp+30h]
        test rbx, rbx
        je ofs4D0C40
        mov eax, esi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne ofs4D0C40
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        mov eax, esi
        lock xadd [rbx+0Ch], eax
        cmp eax, 1
        jne ofs4D0C40
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]
        align 16
    ofs4D0C40:
        mov rdx, [rbp+0E0h]
        cmp rdx, 8
        jb node2
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+0C8h]
        call rva27570

    ;TextDamage
    node2:
        mov qword ptr [rbp+0E0h], 7
        mov [rbp+0D8h], r13
        movups [rbp+0C8h], xmm6
        mov r8d, 10 ; string length
        lea rdx, wstrTextDamage
        lea rcx, [rbp+0C8h]
        call rva27380
        align 16
        lea r8, [rbp+0C8h]
        lea rdx, [rbp+28h]
        mov rcx, rdi
        call rva4D86D0
        mov rbx, [rax+8]
        mov r15, [rax]
        test rbx, rbx
        je original
        lock inc dword ptr [rbx+0Ch]
        mov rcx, [rdi+0D28h]
        test rcx, rcx
        je node2_2
        mov eax, esi
        lock xadd [rcx+0Ch], eax
        cmp eax, 1
        jne node2_2
        mov rax, [rcx]
        call qword ptr [rax+8]
        align 16
    node2_2:
        mov [rdi+0D28h], rbx
        mov [rdi+0D20h], r15
        ; copy fontSize
        mov rcx, [r15+228h]
        call ASMgetHUiHudTextContentPointer
        mov rax, [rax+18h]
        mov [rdi+0D30h], rax
        ; end
        mov rbx, [rbp+30h]
        test rbx, rbx
        je node2_3
        mov eax, esi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne node2_3
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        mov eax, esi
        lock xadd [rbx+0Ch], eax
        cmp eax, 1
        jne node2_3
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]
        align 16
    node2_3:
        mov rdx, [rbp+0E0h]
        cmp rdx, 16
        jb node3
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+0C8h]
        call rva27570
    ; TextDamage_UP
    node3:
        mov qword ptr [rbp+0E0h], 7
        mov [rbp+0D8h], r13
        movups [rbp+0C8h], xmm6
        mov r8d, 13 ; string length
        lea rdx, wstrTextDamageUP
        lea rcx, [rbp+0C8h]
        call rva27380
        align 16
        lea r8, [rbp+0C8h]
        lea rdx, [rbp+28h]
        mov rcx, rdi
        call rva4D86D0
        mov rbx, [rax+8]
        mov r15, [rax]
        test rbx, rbx
        je original
        lock inc dword ptr [rbx+0Ch]
        mov rcx, [rdi+0D48h]
        test rcx, rcx
        je node3_2
        mov eax, esi
        lock xadd [rcx+0Ch], eax
        cmp eax, 1
        jne node3_2
        mov rax, [rcx]
        call qword ptr [rax+8]
        align 16
    node3_2:
        mov [rdi+0D48h], rbx
        mov [rdi+0D40h], r15
        mov rbx, [rbp+30h]
        test rbx, rbx
        je node3_3
        mov eax, esi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne node3_3
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        mov eax, esi
        lock xadd [rbx+0Ch], eax
        cmp eax, 1
        jne node3_3
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]
        align 16
    node3_3:
        mov rdx, [rbp+0E0h]
        cmp rdx, 8
        jb original
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+0C8h]
        call rva27570

    original:
        mov rax, rdi
        mov rcx, [rbp+128h]
        xor rcx, rsp
        call rva9C6E40
        mov rbx, [rsp+290h]
        movaps xmm6, [rsp+230h]
        add rsp, 240h
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbp
        ret 
        int 3

ASMreadHUiHudWeapon ENDP

align 16

ASMHUiHudWeaponUpdateVehicleText proc

        cmp qword ptr [rdi+0D20h], 0
        je ofs4D2AF0
        mov rcx, rdi
        call eDisplayVehicleWeaponDamage
    ofs4D2AF0:
        mov rbx, [rdi+0C50h]
        mov qword ptr [rbp+98h], 7
        jmp HUiHudWeaponUpdateVehicleTextRet
        int 3

ASMHUiHudWeaponUpdateVehicleText ENDP

align 16

ASMHUiHudWeaponUpdateAmmoText proc

        mov rdi, [rsi+0D08h]
        mov r14, [rsi+0D00h]
        test rdi, rdi
        je showDamage

        ;mov rcx, rsi
        ;call eDisplaySoldierWeaponAmmo
        ;jmp showDamage

        ; get weapon pointer
        mov rax, [rsi+7E8h]
        ; check use_extraShotType
        cmp dword ptr [rax+2510h], 16
        je AmmoToStr
        ; check current
        cmp byte ptr [rax+5ACh], 0
        jne AmmoToStr
        ; no text alpha
        mov dword ptr [r14+27Ch], 0
        jmp showDamage
    AmmoToStr:
        xorps xmm0, xmm0
        movups [rbp-49h], xmm0
        mov edx, [rax+8E8h]
        cmp edx, 0
        jle ReloadTimeToStr
        xor rax, rax
        mov [rbp+0Fh], rax
        movups [rbp-1h], xmm0
        movups xmm0, [rsi+0D10h]
        movups [r14+270h], xmm0
        lea rcx, [rbp-1h]
        call rvaB7220 ; converting int to text
        align 16
        jmp AmmoToStrEnd
    ReloadTimeToStr:
        lea r8, [r14+270h] ; font_color
        lea rdx, [rbp-1h]
        mov rcx, rax
        call eTextForWeaponReloadTime
        mov [rbp+17h], rax
    AmmoToStrEnd:
        lock inc dword ptr [rdi+0Ch]
        mov rcx, [rbp-41h]
        test rcx, rcx
        je ofs4D64DE
        lock xadd [rcx+0Ch], ebx
        cmp ebx, 1
        jne ofs4D64DE
        mov rax, qword ptr [rcx]
        call qword ptr [rax+8]
        align 16
    ofs4D64DE:
        mov [rbp-41h], rdi
        mov [rbp-49h], r14
        lea r8, [rbp-1h]
        lea rdx, [rbp-49h]
        call rva4CA990
        mov rdx, [rbp+17h] ; check text length
        cmp rdx, 8
        jb showDamage
        inc rdx
        mov r8d, 2
        mov rcx, [rbp-1h]
        call rva27570

    showDamage:
        cmp qword ptr [rsi+0D20h], 0
        je original
        mov rcx, rsi
        call eDisplaySoldierWeaponDamage
        
    original:
        mov rcx, qword ptr [rbp+3Fh]
        xor rcx, rsp
        call rva9C6E40
        lea r11, qword ptr [rsp+0B0h]
        mov rbx, qword ptr [r11+28h]
        mov rsi, qword ptr [r11+30h]
        mov rsp, r11
        pop r14
        pop rdi
        pop rbp
        ret 
        int 3

ASMHUiHudWeaponUpdateAmmoText ENDP

align 16

ASMreadHUiHudPowerGuage proc
        xorps xmm0, xmm0
        movaps [rdi+0C00h], xmm0
        movaps [rdi+0C10h], xmm0
        movaps [rdi+0C20h], xmm0
        movaps [rdi+0C30h], xmm0
        movaps [rdi+0C40h], xmm0
    ; TextFencerDash
        mov qword ptr [rbp+0C0h], 7
        mov [rbp+0B8h], r15
        mov [rbp+0B0h], r15
        mov [rbp+0A8h], r15
        mov r8d, 14
        lea rdx, wstrTextFencerDash
        lea rcx, [rbp+0A8h]
        call rva27380
        align 16
        lea r8, [rbp+0A8h]
        lea rdx, [rbp-28h]
        mov rcx, rdi
        call rva4D86D0
        mov rbx, [rax+8]
        mov r14, [rax]
        test rbx, rbx
        je fencerBoost
        movups xmm0, [r14+270h]
        movups [rdi+0C10h], xmm0
        mov dword ptr [rdi+0C1Ch], 3F800000h
        lock inc dword ptr [rbx+0Ch]
    ofs4CA869:
        mov [rdi+0C08h], rbx
        mov [rdi+0C00h], r14
        mov rbx, [rbp-20h]
        test rbx, rbx
        je ofs4CA8AA
        mov eax, esi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne ofs4CA8AA
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        mov eax, esi
        lock xadd [rbx+0Ch], eax
        cmp eax, 1
        jne ofs4CA8AA
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]
        align 16
    ofs4CA8AA:
        mov rdx, [rbp+0C0h]
        cmp rdx, 8
        jb fencerBoost
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+0A8h]
        call rva27570

    ; TextFencerDash
    fencerBoost:
        mov qword ptr [rbp+0C0h], 7
        mov [rbp+0B8h], r15
        mov [rbp+0B0h], r15
        mov [rbp+0A8h], r15
        mov r8d, 15
        lea rdx, wstrTextFencerBoost
        lea rcx, [rbp+0A8h]
        call rva27380
        align 16
        lea r8, [rbp+0A8h]
        lea rdx, [rbp-28h]
        mov rcx, rdi
        call rva4D86D0
        mov rbx, [rax+8]
        mov r14, [rax]
        test rbx, rbx
        je ofs4CA8CC
        movups xmm0, [r14+270h]
        movups [rdi+0C30h], xmm0
        mov dword ptr [rdi+0C3Ch], 3F800000h
        lock inc dword ptr [rbx+0Ch]
    fencerBoost_1:
        mov [rdi+0C28h], rbx
        mov [rdi+0C20h], r14
        mov rbx, [rbp-20h]
        test rbx, rbx
        je fencerBoost_2
        mov eax, esi
        lock xadd [rbx+8], eax
        cmp eax, 1
        jne fencerBoost_2
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax]
        mov eax, esi
        lock xadd [rbx+0Ch], eax
        cmp eax, 1
        jne fencerBoost_2
        mov rax, [rbx]
        mov rcx, rbx
        call qword ptr [rax+8]
        align 16
    fencerBoost_2:
        mov rdx, [rbp+0C0h]
        cmp rdx, 8
        jb ofs4CA8CC
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+0A8h]
        call rva27570

    ofs4CA8CC:
        mov qword ptr [rbp+0E0h], 7
        mov [rbp+0D8h], r15
        jmp readHUiHudPowerGuageRet
        int 3

ASMreadHUiHudPowerGuage ENDP

align 16

ASMupdateHUiHudPowerGuage proc
    ; check fencer
        mov rax, [rbx+768h]
        cmp dword ptr [rax+470h], 0
        je ofs4CBCA6
        lea rdx, [rax+1BA0h]
        mov rcx, rbx
        call eDisplayFencerBoostAndDash
    ofs4CBCA6:
        lea rdx, [rbx+920h]
        lea rcx, [rsp+50h]
        jmp updateHUiHudPowerGuageRet
        int 3

ASMupdateHUiHudPowerGuage ENDP

align 16

ASMhookSleep proc

        ;mov rcx,1
        ;call ftimeBeginPeriod
        comiss xmm0, xmm7
        jbe ofs60F20F
        comiss xmm0, xmm8
        jbe ofs60F201
        subss xmm0, xmm8
        minss xmm0, xmm9
        jmp ofs60F204
    ofs60F201:
        movaps xmm0, xmm7
    ofs60F204:
        cvttss2si rcx, xmm0
        call hookSleep
    ofs60F20F:
        ;mov rcx,1
        ;call ftimeEndPeriod
        jmp hookSleepRet

ASMhookSleep ENDP

END
