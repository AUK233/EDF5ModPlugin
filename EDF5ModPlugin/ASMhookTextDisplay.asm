.data

extern ftimeBeginPeriod : proto
extern ftimeEndPeriod : proto
extern hookSleep : proto

extern eTextForWeaponReloadTime : proto
;extern eDisplaySoldierWeaponAmmo : proto
extern eDisplaySoldierWeaponDamage : proto
extern eDisplayVehicleWeaponDamage : proto
extern ASMgetCurrentMissionClassModelType : proto

extern HUiHudWeaponUpdateVehicleTextRet : qword
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

ASMHUiHudWeaponUpdateAmmoText111 proc

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

ASMHUiHudWeaponUpdateAmmoText111 ENDP

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
