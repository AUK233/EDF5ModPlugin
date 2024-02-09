.data

extern ftimeBeginPeriod : proto
extern ftimeEndPeriod : proto
extern hookSleep : proto

extern setDamageString : proto
extern eTextForWeaponReloadTime : proto
extern dhaihdiwa : proto

extern hookTextDisplayRetAddr : qword
extern hookSleepRet : qword
extern rva9C6E40 : qword
extern rva27380 : qword
extern rva27570 : qword
extern rvaB7220 : qword
extern rva4D86D0 : qword
extern rva4CA990 : qword

; TextNumericType2
wstrTextNumericType2 db 84,0,101,0,120,0,116,0,78,0,117,0,109,0,101,0,114,0,105,0,99,0,84,0,121,0,112,0,101,0,50,0,0,0
; TextDamage
wstrTextDamage db 84,0,101,0,120,0,116,0,68,0,97,0,109,0,97,0,103,0,101,0,0,0
; TextDamage_UP
wstrTextDamageUP db 84,0,101,0,120,0,116,0,68,0,97,0,109,0,97,0,103,0,101,0,95,0,85,0,80,0,0,0

.code

; [rbx+60h] pstr
ASMhookTextDisplay proc
        ; check size is required value
        mov rcx, qword ptr [rbx+80h]
        cmp ecx, 96
        jl ofs4B09B4 ; if false, return to original
        mov rcx, qword ptr [r14+270h]
        cmp ecx, 3F800000h
        je ofs4B09B4 ; check ahead of time
        mov r8, rsp ; save rsp
        mov rdx, r14
        mov rcx, rbx
        call setDamageString ; call c++ function
        ;mov rsp, rax ; restore rsp
        ; original
    ofs4B09B4:
        lea rax, qword ptr [rbx+60h]
        cmp qword ptr [rax+18h], 8
        jb ofs4B09C2
        mov rax, qword ptr [rax]
    ofs4B09C2:
        mov rcx, qword ptr [rbx+80h]
        jmp hookTextDisplayRetAddr
        int 3

ASMhookTextDisplay ENDP

ASMgetHUiHudTextContentPointer proc
        ; there have no call functions
        ; so stack does not need to be aligned
        ;sub rsp, 8
        mov rdx, qword ptr [rcx]
        mov rax, qword ptr [rdx]
        ;add rsp, 8
        ret
ASMgetHUiHudTextContentPointer ENDP

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
        sub rsp, 100h

        mov qword ptr [rsp+28h], 15 ; text size
        mov [rsp+20h], r13 ; text end pos
        movups [rsp+10h], xmm6
        movups [rsp], xmm6 ; text start pos
        mov r8d, 10 ; string length
        lea rdx, wstrTextDamage
        lea rcx, [rsp]
        call rva27380
        lea r8, [rsp]
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
    node2_3:
        mov rdx, [rsp+28h]
        cmp rdx, 16
        jb node3
        inc rdx
        mov r8d, 2
        mov rcx, [rsp]
        call rva27570
    ; TextDamage_UP
    node3:
        mov qword ptr [rsp+0E8h], 111 ; text size
        mov [rsp+0E0h], r13 ; text end pos
        movups [rsp+0D0h], xmm6
        movups [rsp+0C0h], xmm6
        movups [rsp+0B0h], xmm6
        movups [rsp+0A0h], xmm6
        movups [rsp+90h], xmm6
        movups [rsp+80h], xmm6
        movups [rsp+70h], xmm6
        movups [rsp+60h], xmm6
        movups [rsp+50h], xmm6
        movups [rsp+40h], xmm6
        movups [rsp+30h], xmm6
        movups [rsp+20h], xmm6
        movups [rsp+10h], xmm6
        movups [rsp], xmm6 ; text start pos
        mov r8d, 13 ; string length
        lea rdx, wstrTextDamageUP
        lea rcx, [rsp]
        call rva27380
        lea r8, [rsp]
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
    node3_3:
        mov rdx, [rsp+0E8h]
        cmp rdx, 8
        jb original
        inc rdx
        mov r8d, 2
        mov rcx, [rsp]
        call rva27570

    original:
        add rsp, 100h

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

ASMreadHUiHudWeapon ENDP

ASMHUiHudWeaponUpdateAmmoText proc

        mov rdi, [rsi+0D08h]
        mov r14, [rsi+0D00h]
        test rdi, rdi
        je ShowDamage
        ; get weapon pointer
        mov rax, [rsi+7E8h]
        cmp byte ptr [rax+5ACh], 0
        jne AmmoToStr
        ; no text alpha
        mov dword ptr [r14+27Ch], 0
        jmp ShowDamage
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
    ofs4D64DE:
        mov [rbp-41h], rdi
        mov [rbp-49h], r14
        lea r8, [rbp-1h]
        lea rdx, [rbp-49h]
        call rva4CA990
        mov rdx, [rbp+17h] ; check text length
        cmp rdx, 8
        jb ShowDamage
        inc rdx
        mov r8d, 2
        mov rcx, [rbp-1h]
        call rva27570
    
    ShowDamage:
        cmp qword ptr [rsi+0D28h], 0
        je original
        mov rax, [rsi+7E8h]
        cmp byte ptr [rax+5ACh], 0
        je original
        mov rcx, rsi
        ;call dhaihdiwa

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

ASMHUiHudWeaponUpdateAmmoText ENDP

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