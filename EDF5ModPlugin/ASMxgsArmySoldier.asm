.data

extern rva9C6E40 : qword

extern noThrowAnime : dword
extern HUDEnhanceStatus : dword

extern edf2E0270Address : qword
extern edf2E07C0Address : qword
extern edf2E18A0Address : qword
extern eArmySoldierUseAuxiliaryRetAddr : qword
extern edf5F8C40Address : qword

extern eSoldierCallSupportRetAddr : qword
extern hudShowSupportSlot2RetAddr : qword

extern eEngineerUseAuxiliaryRetAddr : qword
extern edf2E2E30Address : qword

extern eAccessoryEnhancementRetAddr : qword
extern eAccessoryEnhancement : proto
extern eGetAccessoryValueAddr : qword
extern eGetAccessoryINT32Addr : qword

; L"support_call"
eSoldierCallSupport db 115,0,117,0,112,0,112,0,111,0,114,0,116,0,95,0,99,0,97,0,108,0,108,0,0,0
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR.sgo"
lyt_HudSubWeaponGuageR1 db 61h, 00h, 70h, 00h, 70h, 00h, 3Ah, 00h, 2Fh, 00h, 75h, 00h, 69h, 00h, 2Fh, 00h, 6Ch, 00h, 79h, 00h, 74h, 00h, 5Fh, 00h, 48h, 00h, 75h, 00h, 64h, 00h, 53h, 00h
db 75h, 00h, 62h, 00h, 57h, 00h, 65h, 00h, 61h, 00h, 70h, 00h, 6Fh, 00h, 6Eh, 00h, 47h, 00h, 75h, 00h, 61h, 00h, 67h, 00h, 65h, 00h, 52h, 00h, 2Eh, 00h, 73h, 00h, 67h, 00h, 6Fh, 00h, 00h, 00h
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR_E.sgo"
lyt_HudSubWeaponGuageR1E db 97,0,112,0,112,0,58,0,47,0,117,0,105,0,47,0,108,0,121,0,116,0,95,0,72,0,117,0,100,0,83,0,117,0,98,0,87,0,101,0,97,0,112,0
db 111,0,110,0,71,0,117,0,97,0,103,0,101,0,82,0,95,0,69,0,46,0,115,0,103,0,111,0,0,0
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR2.sgo"
lyt_HudSubWeaponGuageR2 db 97,0,112,0,112,0,58,0,47,0,117,0,105,0,47,0,108,0,121,0,116,0,95,0,72,0,117,0,100,0,83,0,117,0,98,0,87,0,101,0,97,0
db 112,0,111,0,110,0,71,0,117,0,97,0,103,0,101,0,82,0,50,0,46,0,115,0,103,0,111,0,0,0

.code

ASMeAssultSoldierInitialization proc

    ; initialize memory
        xorps xmm0, xmm0
        movaps [rbx+1BE0h], xmm0
    ofs2DF10D:
        mov rcx, qword ptr [rsp+68h]
        xor rcx, rsp
        call rva9C6E40
        mov rax, rbx
        mov rbx, qword ptr [rsp+0A0h]
        add rsp, 70h
        pop rdi
        pop rsi
        pop rbp
        ret 
        int 3

ASMeAssultSoldierInitialization ENDP

align 16

ASMeArmySoldierUseAuxiliary proc

        cmp byte ptr [rbx+0B5Eh], 0 ; old is +B5D, but we now need the hold-on version
        mov eax, [rbx+1BE0h] ; throw button timer
        je throwTimer
        inc eax
        mov [rbx+1BE0h], eax
        cmp eax, 20
        jbe ofs2DF473
        mov dword ptr [rbx+1BE0h], 0
        ;Get a weapon with support slot 1
        mov rax, qword ptr [rbx+1600h]
        movsxd rcx, dword ptr [rax]
        cmp ecx, dword ptr [rbx+15A0h]
        jge ofs2DF473
        test ecx, ecx
        js ofs2DF473
        mov rax, qword ptr [rbx+1590h]
        mov rcx, qword ptr [rax+rcx*8]
        test rcx, rcx
        je ofs2DF473
        cmp dword ptr [rcx+2500h], -1
        jne ofs2DF473
        mov dword ptr [rbx+1BE4h], 10 ; throw button cd
        mov rax, [rcx]
        call qword ptr [rax+80h] ; Request to reload
        jmp ofs2DF473

    throwCD:
        dec dword ptr [rbx+1BE4h]
        jmp ofs2DF473

    throwTimer:
        cmp dword ptr [rbx+1BE4h], 0
        jg throwCD
        test eax, eax
        je ofs2DF473
        mov dword ptr [rbx+1BE0h], 0 ; clear timer
        cmp eax, 10
        ja ofs2DF473 ; if it <= 10
        cmp qword ptr [rbx+1610h], 0
        jbe ofs2DF4C1
        ;Get a weapon with support slot 1
        mov rax, qword ptr [rbx+1600h]
        movsxd rcx, dword ptr [rax]
        ;inc rcx ; support slot 2
        cmp ecx, dword ptr [rbx+15A0h]
        jge ofs2DF4C1
        test ecx, ecx
        js ofs2DF4C1
        mov rax, qword ptr [rbx+1590h]
        mov rdx, qword ptr [rax+rcx*8]
        test rdx, rdx
        je ofs2DF4C1
        ; check ROF allowed to fire
        cmp dword ptr [rdx+0B40h], 0
        jne ofs2DF4C1
        cmp dword ptr [rdx+8E8h], 0
        jbe ofs2DF4C1
        ; use support slot 1
        cmp noThrowAnime, 0
        jne noThrowAnimeBlock
        lea rax, ASMeSoldierCallSupport
        jmp ofs2DF496
    noThrowAnimeBlock:
        mov byte ptr [rdx+0D9h], 1
        jmp ofs2DF4C1

    ofs2DF473:
        cmp byte ptr [rbx+0B5Bh], 0
        je ofs2DF4C1
        ; check in air
        cmp dword ptr [rbx+560h], 2
        jne ofs2DF4C1
        ;
        cmp qword ptr [rbx+1618h], 0
        je ofs2DF48F
        cmp dword ptr [rbx+1620h], 1
        jge ofs2DF4C1
    ofs2DF48F:
        ;Get a weapon with support slot 2
        mov rax, qword ptr [rbx+1600h]
        movsxd rcx, dword ptr [rax]
        inc rcx ; support slot 2
        cmp ecx, dword ptr [rbx+15A0h]
        jge useDash
        test ecx, ecx
        js useDash
        mov rax, qword ptr [rbx+1590h]
        mov rdx, qword ptr [rax+rcx*8]
        test rdx, rdx
        je useDash
        ; check is it a weapon
        cmp dword ptr [rdx+698h], 0 ;AmmoAlive
        je useDash
        cmp dword ptr [rdx+8E8h], 0
        jbe ofs2DF4C1
        ; use support slot 2
        mov rax, edf2E0270Address
        jmp ofs2DF496
    useDash:
        ; use dash
        mov rax, edf2E07C0Address
    ofs2DF496:
        mov [rbp-69h], rax
        lea rcx, [rbx+1A00h]
        cmp qword ptr [rcx+28h], 0
        mov dword ptr [rbp-61h], 0
        movaps xmm0, [rbp-69h]
        movdqa [rbp-69h], xmm0
        je ofs2DF4C1
        lea rdx, qword ptr [rbp-69h]
        call edf2E18A0Address
    ofs2DF4C1:
        jmp eArmySoldierUseAuxiliaryRetAddr
        int 3

ASMeArmySoldierUseAuxiliary ENDP

align 16

ASMeSoldierCallSupport proc

        push rbx
        sub rsp, 30h
        mov rbx, rcx
        test edx, edx
        jne ofs2DF768
        mov dword ptr [rcx+420h], 81h
        xor r8d, r8d
        mov eax, [rcx+1050h]
        imul rdx, rax, 1A0h
        mov rax, [rcx+9D0h]
        mov [rsp+48h], rsi
        mov r10, [rdx+rax+118h]
        mov r9d, [r10+4]
        test r9d, r9d
        je ofs2DF73E
        movsxd r11, dword ptr [r10+8]
        mov [rsp+40h], rbp
        lea rbp, eSoldierCallSupport
        
        align 16
    ofs2DF6D0:
        lea rcx, qword ptr [r8+r8*8]
        lea rax, qword ptr [r11+rcx*4]
        add rax, r10
        movsxd rdx, dword ptr [rax]
        add rdx, rax
        ;or rcx, -1
        mov rcx, -4

        align 16
    ofs2DF6F0:
        ;movzx eax, word ptr [rdx+rcx*2+2]
        ;cmp ax, word ptr [rbp+rcx*2+2]
        ;jne ofs2DF711
        ;add rcx, 2
        ;cmp rcx, 13
        ;je ofs2DF71B
        ;movzx eax, word ptr [rdx+rcx*2]
        ;cmp ax, word ptr [rbp+rcx*2]
        ;je ofs2DF6F0

        add rcx, 4
        cmp rcx, 12
        jge checkNull
        mov eax, [rdx+rcx*2]
        cmp eax, [rbp+rcx*2]
        je ofs2DF6F0
    checkNull:
        movzx eax, word ptr [rdx+rcx*2]
        test eax, eax
        je ofs2DF71B

    ofs2DF711:
        inc r8d
        cmp r8d, r9d
        jne ofs2DF6D0
        jmp ofs2DF739
    ofs2DF71B:
        cmp r8d, -1
        je ofs2DF739
        mov edx, [rbx+1050h]
        lea rcx, [rbx+970h]
        mov r9d, 1
        call edf5F8C40Address
    ofs2DF739:
        mov rbp, [rsp+40h]
    ofs2DF73E:
        mov eax, [rbx+1050h]
        mov rsi, [rsp+48h]
        imul rcx, rax, 1A0h
        mov rax, [rbx+9D0h]
        mov dword ptr [rcx+rax+138h], 3F800000h
        add rsp, 30h
        pop rbx
        ret
    ofs2DF768:
        jmp eSoldierCallSupportRetAddr
        int 3

ASMeSoldierCallSupport ENDP

align 16

ASMhudShowSupportSlot2 proc

        lea r9, qword ptr [rbp-60h]
        lea rdx, qword ptr [rbp+48h]
        cmp ebx, 1
        je ShowSupportSlot2
        cmp HUDEnhanceStatus, 0
        jne SupportSlot1Enhance
        lea r8, lyt_HudSubWeaponGuageR1
        jmp hudShowSupportSlot2RetAddr
    SupportSlot1Enhance:
        ;test rbx, rbx
        ;jne SupportSlot1Enhance2
        ;mov rax, [r15+1590h]
        ;mov rax, [rax+r10*8]
        ;mov dword ptr [rax+2510h], 16 ; use_extraShotType
    SupportSlot1Enhance2:
        lea r8, lyt_HudSubWeaponGuageR1E
        jmp hudShowSupportSlot2RetAddr
    ShowSupportSlot2:
        lea r8, lyt_HudSubWeaponGuageR2
        jmp hudShowSupportSlot2RetAddr
        int 3

ASMhudShowSupportSlot2 ENDP

align 16

ASMeEngineerUseAuxiliary proc
    ;Get a weapon with support slot 1
        cmp byte ptr [rbx+0B5Dh], 0
        je use2Slot
        test byte ptr [rbx+420h], 4
        jne ofs2E19FC
        cmp qword ptr [rbx+1610h], 0
        jbe ofs2E19FC
        mov rax, qword ptr [rbx+1600h]
        movsxd rcx, dword ptr [rax]
        cmp ecx, dword ptr [rbx+15A0h]
        jge ofs2E19FC
        test ecx, ecx
        js ofs2E19FC
        mov rax, qword ptr [rbx+1590h]
        mov rdx, qword ptr [rax+rcx*8]
        test rdx, rdx
        je ofs2E19FC
        cmp dword ptr [rdx+8E8h], 0
        jbe ofs2E19FC
        ; use support slot 1
        cmp noThrowAnime, 0
        jne noThrowAnimeBlock
        lea rax, ASMeSoldierCallSupport
        jmp common
    noThrowAnimeBlock:
        mov byte ptr [rdx+0D9h], 1
        jmp ofs2E19FC

    use2Slot:
        ;Get a weapon with support slot 2
        cmp byte ptr [rbx+0B5Bh], 0
        je ofs2E19FC
        test byte ptr [rbx+420h], 4
        jne ofs2E19FC
        cmp qword ptr [rbx+1610h], 0
        jbe ofs2E19FC
        mov rax, qword ptr [rbx+1600h]
        movsxd rcx, dword ptr [rax]
        inc rcx ; support slot 2
        cmp ecx, dword ptr [rbx+15A0h]
        jge ofs2E19FC
        test ecx, ecx
        js ofs2E19FC
        mov rax, qword ptr [rbx+1590h]
        mov rdx, qword ptr [rax+rcx*8]
        test rdx, rdx
        je ofs2E19FC
        cmp dword ptr [rdx+8E8h], 0
        jbe ofs2E19FC
        ; use support slot 2
        mov rax, edf2E0270Address
    common:
        mov qword ptr [rsp+30h], rax
        lea rcx, qword ptr [rbx+1A00h]
        cmp qword ptr [rcx+28h], 0
        mov dword ptr [rsp+38h], 0
        movaps xmm0, xmmword ptr [rsp+30h]
        movdqa xmmword ptr [rsp+30h], xmm0
        je ofs2E19FC
        lea rdx, qword ptr [rsp+30h]
        call edf2E2E30Address
    ofs2E19FC:
        jmp eEngineerUseAuxiliaryRetAddr
        int 3

ASMeEngineerUseAuxiliary ENDP

align 16

ASMeAccessoryEnhancement proc

        mov rcx, rdi
        call eAccessoryEnhancement
        ;mov rcx, [rdi+1590h]
        ;mov rax, [rdi+15A0h]
        ;lea rdx, [rcx+rax*8]
        ;cmp rcx, rdx
        ;je Original
        ;fedawd:
        ;mov rax, [rcx]
        ;mov r8d, [rax+1D0h]
        ;add r8d, r8d
        ;mov [rax+1D0h], r8d
        ;add rcx, 8
        ;cmp rcx,rdx
        ;jne fedawd
    ; Original
        xor r9d, r9d
        movaps xmm2, xmm8
        lea edx, [r9+10h]
        mov rcx, rdi
        jmp eAccessoryEnhancementRetAddr
        int 3

ASMeAccessoryEnhancement ENDP

align 16

ASMeGetAccessoryValue proc
    jmp eGetAccessoryValueAddr
    int 3
ASMeGetAccessoryValue ENDP

align 16

ASMeGetAccessoryINT32 proc
    jmp eGetAccessoryINT32Addr
    int 3
ASMeGetAccessoryINT32 ENDP

END
