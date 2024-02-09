.data
; Use other asm functions
extern edf8C8C0 : proto
extern edf5BDF30 : proto
extern edfSoldierWeaponCharge : proto

extern debugGetWeaponName : proto
extern ModLogStatus : dword

extern rva391230 : qword

extern weaponReloadEXRetAddr : qword
extern weaponStartReloadRetAddr : qword

; L"ReloadInit"
wReloadInit db 82,0,101,0,108,0,111,0,97,0,100,0,73,0,110,0,105,0,116,0,0,0
; L"ReloadPadType"
wReloadPadType db 82,0,101,0,108,0,111,0,97,0,100,0,80,0,97,0,100,0,84,0,121,0,112,0,101,0,0,0
; 0.99f
wReloadInitFloat dd 3F7D70A4h

.code

ASMrva391230 proc
        push rdi
        mov rdi, rcx
        call rva391230
        test al, al
        je chargeWeapon
        mov eax, 1
        pop rdi
        ret
    chargeWeapon:
        mov rcx, rdi
        call edfSoldierWeaponCharge
        xor eax, eax
        pop rdi
        ret
ASMrva391230 ENDP

ASMweaponReloadEX proc
    ; add star to "ReloadInit"
        lea rdx, wReloadInit
        mov rcx, r14
        call edf5BDF30
        movsxd rcx, eax
        cmp ecx, -1
        jne ofs38E2F9
        ;mov rax,rdi ; old
        mov rdx, rbx
        jmp ofs38E30B
    ofs38E2F9:
        mov rax, qword ptr [r14]
        movsxd rdx, dword ptr [rax+0Ch]
        add rdx, rax
        ;lea rcx, qword ptr [rcx+rcx*2] ; old
        ;lea rax, qword ptr [rdx+rcx*4] ; old
        lea rcx, qword ptr [rcx+rcx*2]
        lea rdx, qword ptr [rdx+rcx*4]
    ofs38E30B:
        ;movss xmm1, dword ptr [rax+8] ; old
        ; backup start
        movups xmm6, xmmword ptr [rbp+200h]
        movups xmm0, xmmword ptr [rbp+210h]
        movups xmmword ptr [rbp+200h], xmm0
        ; backup end
        mov r8, r12
        lea rcx, qword ptr [rbp+200h] ; note that it cannot be replaced here
        call edf8C8C0
        movss xmm1, dword ptr [rax] ; get value
        movups xmmword ptr [rbp+200h], xmm6 ; recovery
        ; old line
        movss xmm0, wReloadInitFloat
        comiss xmm0, xmm1
        jbe ofs38E33C
        mov dword ptr [rsi+8E8h], edi
        movd xmm0, dword ptr [rsi+1A4h]
        cvtdq2ps xmm0, xmm0
        mulss xmm0, xmm1
        cvttss2si eax, xmm0
        sub dword ptr [rsi+0B90h], eax
    ofs38E33C:
        mov dword ptr [rsi+590h], 3F800000h
    ; debug
        cmp ModLogStatus, 1
        jne ofsNewFN
        mov rcx, [rsi+148h]
        call debugGetWeaponName
    ofsNewFN:
    ; initialize memory
        xorps xmm0, xmm0
        movups [rsi+2500h], xmm0
        ;mov qword ptr [rsi+2500h], 0
        ;mov qword ptr [rsi+2508h], 0
    ; read new function "ReloadPadType"
        lea rdx, wReloadPadType
        mov rcx, r14
        call edf5BDF30
        movsxd rcx, eax
        cmp ecx, -1
        je EndBlock ; if node does not exist, jump
    ; read ptr in node
        mov rax, qword ptr [r14]
        movsxd rdx, dword ptr [rax+12]
        add rdx, rax
        lea rcx, qword ptr [rcx+rcx*2]
        lea rcx, qword ptr [rdx+rcx*4]
    ; node0 decides on midsection reload
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        mov [rsi+2500h], eax ; midsection type
    ; node1
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+20]
        mov [rsi+2504h], eax ; extra time
    ; node2
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+32]
        mov [rsi+2508h], eax ; charge interval
        mov [rsi+250Ch], eax

    EndBlock:
        jmp weaponReloadEXRetAddr
        int 3

ASMweaponReloadEX ENDP

ASMweaponStartReload proc

        mov eax, dword ptr [rbx+2500h]
        cmp eax, 0
        je originalBlock
        cmp eax, 3 ; no manual reload
        je ofs3905EC
        ; current reload time = (totalAmmo - remainAmmo) * reloadTime  / totalAmmo
        mov eax, dword ptr [rbx+1D0h]
        mov ecx, eax ; totalAmmo
        sub eax, dword ptr [rbx+8E8h] ; - remainAmmo
    ; calculate current energy requirement
        cmp dword ptr [rbx+1C4h], 0BF800000h ; check -1
        je normalReloadBlock
        cvtsi2ss xmm0, eax
        cvtsi2ss xmm1, ecx ; int to float
        divss xmm0, xmm1
        mulss xmm0, dword ptr [rbx+1C4h] ; total energy need
        movss dword ptr [rbx+0BA4h], xmm0 ; current energy need
    ; calculate current reload time
    normalReloadBlock:
        imul eax, dword ptr [rbx+1A4h] ; * reloadTime
        ; use float division
        cvtsi2ss xmm0, eax
        cvtsi2ss xmm1, ecx ; int to float
        divss xmm0, xmm1 ; / totalAmmo
        cvttss2si eax, xmm0 ; float to int
        add eax, dword ptr [rbx+2504h] ; + extra time
        mov dword ptr [rbx+0B90h], eax
    ; original
    originalBlock:
        mov dword ptr [rbx+8E8h], 0
        mov dword ptr [rbx+0B40h], 4
        jmp weaponStartReloadRetAddr
    ofs3905EC:
        add rsp, 20h
        pop rbx
        ret
        int 3

ASMweaponStartReload ENDP

END