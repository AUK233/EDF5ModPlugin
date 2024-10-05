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

;
extern rva12D890 : qword

extern GetSubtitleTextAddress : proto
extern pSubtitleIndex : dword
extern pSubtitleFile : qword
extern edf5BDF30Address : qword
extern rva51960 : qword
extern vedfE918A8 : qword
extern _SubtitlePosition : dword
extern vedf125AB70 : qword
extern rva114980 : qword
extern rva51A80 : qword
extern rva61E560 : qword
extern _lyt_HuiSubtitleBox : word
;

extern script2C_FA0ret : qword
extern InitializeTalkSubtitleString : proto
extern PushTalkSubtitleString : proto
extern DisplayTalkSubtitleString : proto
extern script2C_1005ret : qword
extern script2C_1006ret : qword
extern script2C_1007ret : qword
extern script2C_1009ret : qword

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

ASMshowSubtitleByPlayingSoundOff proc

        push rbx
        sub rsp, 20h
        ; edx is input
        ; rcx is input
        call rva12D890
        mov rbx, rax
        test rax, rax
        je ReturnAddress
        mov rdx, [rax]
        mov rcx, rax
        call qword ptr [rdx+38h]
        mov rdx, [rbx]
        mov rcx, rbx
        call qword ptr [rdx+40h]
    ReturnAddress:
        add rsp, 20h
        pop rbx
        ret
        int 3

ASMshowSubtitleByPlayingSoundOff ENDP

align 16

ASMshowSubtitleByPlayingSoundOn proc

        push rbp
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        lea rbp, [rsp-0F0h]
        sub rsp, 1F0h
        mov qword ptr [rsp+40h], -2
        mov [rsp+240h], rbx
        ; save register
        ;mov [rbp+0E0h], r8
        mov r14, rdx
        mov r13, rcx
        xor r12d, r12d
        ; rcx is input
    showSubtitle:
        ; initialization
        lea rcx, [rbp-70h]
        call rva51960
        mov rax, vedfE918A8
        mov [rbp-70h], rax
        mov qword ptr [rbp+90h], 7
        mov [rbp+88h], r12
        mov [rbp+78h], r12w
        mov qword ptr [rbp+0B0h], 7
        mov [rbp+0A8h], r12
        mov [rbp+98h], r12w
        mov [rbp+0B8h], r12d
        mov [rbp+0C8h], r12
        mov [rbp+0D0h], r12b
        mov [rbp+0C4h], r12d
        mov [rbp-48h], r12d
        mov qword ptr [rbp-78h], 7
        mov [rbp-80h], r12
        mov [rsp+70h], r12w
        mov rsi, -1
        ; end
        mov r8d, [r14+12] ; string length
        mov rdx, [r14]  ; string pointer
        lea rcx, [rbp+78h]
        call rva27380
        ;
        mov dword ptr [rbp+0B8h], 18h
        mov eax, [r14+8] ; text hui pos x
        mov ecx, _SubtitlePosition+4
        mov rdx, vedf125AB70
        mov [rbp+0BCh], eax
        mov [rbp+0C0h], ecx
        mov rcx, [rdx]
        test rcx, rcx
        je ofs1268A6
        add rcx, -136
        jmp ofs1268A9
    ofs1268A6:
        mov rcx, r12
    ofs1268A9:
        mov dword ptr [rsp+20h], 0
        lea r9, [rbp-70h]
        lea r8, _lyt_HuiSubtitleBox
        lea rdx, [rsp+60h]
        call rva61E560
        ;
        lea rax, [rsp+60h]
        mov rdx, [rax]
        mov [rsp+30h], rdx
        mov rcx, [rax+8]
        mov [rax], r12
        mov [rax+8], r12
        mov [rsp+48h], rcx
        mov r8, [r13+1F0h]
        mov [rsp+38h], r8
        mov [r13+1F0h], rcx
        mov [r13+1E8h], rdx
        test r8, r8
        je ofs126906
        mov eax, esi
        lock xadd [r8+12], eax
        cmp eax, 1
        jne ofs126906
        mov rcx, [rsp+38h]
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs126906:
        mov rcx, [rsp+68h]
        test rcx, rcx
        je ofs126922
        mov eax, esi
        lock xadd [rcx+12], eax
        cmp eax, 1
        jne ofs126922
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs126922:
        xorps xmm0, xmm0
        movaps [rsp+20h], xmm0
        mov rbx, [r13+1F0h]
        mov rdi, [r13+1E8h]
        test rbx, rbx
        je ofs12695C
        lock inc dword ptr [rbx+12]
        mov rcx, qword ptr [rsp+28h]
        test rcx, rcx
        je ofs12695C
        lock xadd [rcx+12], esi
        cmp esi, 1
        jne ofs12695C
        mov rax, [rcx]
        call qword ptr [rax+8]
    ofs12695C:
        mov [rsp+28h], rbx
        mov [rsp+20h], rdi
        lea rdx, [rsp+20h]
        mov rcx, r13
        call rva114980
        mov pSubtitleIndex, eax
        ;
        mov rdx, [rbp+90h]
        cmp rdx, 8
        jb ofs1269FF
        inc rdx
        mov r8d, 2
        mov rcx, [rbp+78h]
        call rva27570
    ofs1269FF:
        mov qword ptr [rbp+90h], 7
        mov [rbp+88h], r12
        mov [rbp+78h], r12w
        lea rcx, [rbp-70h]
        call rva51A80
    ReturnAddress:
        ;mov rax, [rbp+0E0h]
        mov rbx, [rsp+240h]
        add rsp, 1F0h
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbp
        ret
        int 3

ASMshowSubtitleByPlayingSoundOn ENDP

align 16

ASMshowSubtitleByPlayingSoundReady proc

        push rbx
        push rsi
        push rdi
        sub rsp, 30h
        ; save register
        mov rbx, r8
        mov rsi, rcx
        mov rdi, rdx
    ; check subtitles exist
        mov edx, pSubtitleIndex
        test edx, edx
        je getSubtitleText
        ; rcx is input
        call ASMshowSubtitleByPlayingSoundOff
        mov pSubtitleIndex, 0
    getSubtitleText:
        xor r9d, r9d
        mov r8d, ebx
        mov rdx, rdi
        lea rcx, [rsp+20h]
        call GetSubtitleTextAddress
        test eax, eax
        je ReturnAddress
        ;
        lea rdx, [rsp+20h]
        mov rcx, rsi
        call ASMshowSubtitleByPlayingSoundOn
    ReturnAddress:
        mov rax, rbx
        add rsp, 30h
        pop rdi
        pop rsi
        pop rbx
        ret

ASMshowSubtitleByPlayingSoundReady ENDP

align 16

ASMscript2C_FA0 proc

        mov qword ptr [rsp+40h], 7
        xor r8d,r8d
        mov [rsp+38h], r8
        mov [rsp+28], r8w
        cmp [rbx], r8w
        je ofs1270EC
        or r8, -1
        align 16
    ofs1270E1:
        inc r8
        cmp word ptr [rbx+r8*2], 0
        jne ofs1270E1
    ofs1270EC:
        mov rdx, rbx
        mov rcx, rdi
        call ASMshowSubtitleByPlayingSoundReady
    ;original
        mov r8, rax
        mov rdx, rbx
        jmp script2C_FA0ret
        int 3

ASMscript2C_FA0 ENDP

align 16

ASMscript2C_FA2 proc

        test al, al
        je clearSubtitle
        mov rbx, [rsi+40h]
        mov byte ptr [rbx+68h], 1
    clearSubtitle:
        xor r12d, r12d
        cmp [rbx+68h], r12b
        jne ofs116A83
        mov edx, pSubtitleIndex
        test edx, edx
        je ofs116A83
        mov rcx, rsi
        call ASMshowSubtitleByPlayingSoundOff
        mov pSubtitleIndex, r12d
    ofs116A83:    
        cmp [r14+0A0h], r12b
        jne ofs116ABD
        mov [r14+98h], rdi
    ofs116ABD:
        mov [r14+0A0h], r12b
        movaps xmm6, [rsp+0F0h]
        lea r11, [rsp+100h]
        mov rbx, [r11+30h]
        mov rsi, [r11+38h]
        mov rdi, [r11+40h]
        mov rsp, r11
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret 

ASMscript2C_FA2 ENDP

align 16

ASMscript2C_1004 proc

        add rsp, 0C0h
        pop r15
        pop r14
        pop rdi
        pop rsi
        pop rbp
        jmp InitializeTalkSubtitleString
        int 3

ASMscript2C_1004 ENDP

align 16

ASMscript2C_1005 proc

        mov rcx, rdi
        call DisplayTalkSubtitleString
        mov [rbp+2Fh], rsi
        mov [rbp+37h], rsi
        mov [rbp+3Fh], rsi
        xor r8d, r8d
        jmp script2C_1005ret
        int 3

ASMscript2C_1005 ENDP

align 16

ASMscript2C_1006 proc

        ;mov r8, rbx
        ;mov rdx, rsi
        ;mov rcx, rbp
        ;call ASMshowSubtitleByPlayingSoundReady
        mov rdx, rbx
        mov rcx, rsi
        call PushTalkSubtitleString
    ;original
        mov r8, rbx
        mov rdx, rsi
        lea rcx, [rsp+38h]
        call rva27380
        jmp script2C_1006ret
        int 3

ASMscript2C_1006 ENDP

align 16

ASMscript2C_1007 proc

        ;mov r8, rbx
        ;mov rdx, rdi
        ;mov rcx, rsi
        ;call ASMshowSubtitleByPlayingSoundReady
        mov rdx, rbx
        mov rcx, rdi
        call PushTalkSubtitleString
    ;original
        mov r8, rbx
        mov rdx, rdi
        lea rcx, [rsp+50h]
        call rva27380
        jmp script2C_1007ret
        int 3

ASMscript2C_1007 ENDP

align 16

ASMscript2C_1009 proc

        ;mov r8, rbx
        ;mov rdx, rsi
        ;mov rcx, rbp
        ;call ASMshowSubtitleByPlayingSoundReady
        mov rdx, rbx
        mov rcx, rsi
        call PushTalkSubtitleString
    ;original
        mov r8, rbx
        mov rdx, rsi
        lea rcx, [rsp+48h]
        call rva27380
        jmp script2C_1009ret
        int 3

ASMscript2C_1009 ENDP

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