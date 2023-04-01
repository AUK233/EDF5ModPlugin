.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto
extern edf6136C0 : proto

extern vedf125AB68 : qword

;L"spd_ChangeModelParam"
spdChangeModel db 115,0,112,0,100,0,95,0,67,0,104,0,97,0,110,0,103,0,101,0,77,0,111,0,100,0,101,0,108,0,80,0,97,0,114,0,97,0,109,0,0,0
;L"spd_AmmoAlive"
spdAmmoAlive db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,65,0,108,0,105,0,118,0,101,0,0,0
;L"spd_AmmoClass"
spdAmmoClass db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,67,0,108,0,97,0,115,0,115,0,0,0
;L"spd_AmmoColor"
spdAmmoColor db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,67,0,111,0,108,0,111,0,114,0,0,0
;L"spd_AmmoExSet"
spdAmmoExSet db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCgiantSpider proc

        movss dword ptr [rdi+1374h], xmm6
    ; new
        lea rdx, spdChangeModel
        lea rcx, [rdi+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        lea rcx, [rdi+480h]
        call edf3AE530

        ;AmmoClassBlock:
        ;lea rdx, spdAmmoClass
        ;lea rcx, [rdi+0B0h]
        ;call edf5BDF30 ; read sgo node
        ;cmp eax, -1
        ;je AmmoAliveBlock
        ;mov r8, [rdi+0B0h]
        ;cdqe 
        ;movsxd rdx, dword ptr [r8+0Ch]
        ;lea rcx, [rax+rax*2]
        ;lea rax, [r8+rdx]
        ;lea rax, [rax+rcx*4]
        ;movsxd rdx, dword ptr [rax+8]
        ;mov rcx, vedf125AB68
        ;add rdx, rax
        ;mov rcx, [rcx]
        ;call edf6136C0
        ;mov [rdi+1430h], rax

    AmmoAliveBlock:
        lea rdx, spdAmmoAlive
        lea rcx, [rdi+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoColorBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rdi+14A8h], ecx ; AmmoAlive
            
    AmmoColorBlock:
        lea rdx, spdAmmoColor
        lea rcx, [rdi+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        ; change ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+8]
        movss dword ptr [rdi+14E0h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+20]
        movss dword ptr [rdi+14E4h], xmm1 ;g
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rdi+14E8h], xmm0 ;b
        movss xmm1, dword ptr [rax+rcx+44]
        movss dword ptr [rdi+14ECh], xmm1 ;a

    AmmoExSetBlock:
        lea rdx, spdAmmoExSet
        lea rcx, [rdi+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je OriginalEndBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address

        ; change shot count
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        imul eax, [rdi+15D0h] ; old * this
        ; check 0
        mov edx, 1
        cmp eax, edx
        cmovl eax, edx ; if < 1
        ; write value
        mov [rdi+15D0h], eax

        ; change ammo explosion range
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rdi+14B8h], xmm0
        ; change ammo hit size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rdi+14C8h], xmm0
        ; change ammo hit impulse
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rdi+14CCh], xmm0
        ;ammo penetration
        ;mov byte ptr [rdi+14BCh], 1

    OriginalEndBlock:
        ;lea r11, qword ptr [rsp+90h]
        ;mov rsi, qword ptr [r11+38h]
        ;movaps xmm6, xmmword ptr [r11-10h]
        movups xmm6, xmmword ptr [rsp+80h]
        ;mov rsp, r11
        add rsp, 90h
        pop r15
        pop r14
        pop rdi
        ret

ASMxgsOCgiantSpider ENDP

END