.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30 : proto
;extern edf6136C0 : proto

extern InsectAmmoType : qword

extern vedf125AB68 : qword
extern giantBeeAmmoNextAddr : qword
extern giantBeeAmmoRetAddr : qword

;L"bee_BulletSet"
beeBulletSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,83,0,101,0,116,0,0,0
; L"bee_BulletAccuracy"
beeBulletAccuracy db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,99,0,99,0,117,0,114,0,97,0,99,0,121,0,0,0
; L"bee_BulletAlive"
beeBulletAlive db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"bee_BulletExSet"
beeBulletExSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCgiantBee proc

        mov dword ptr [rbx+12A0h], 0 ; ammo type
        mov dword ptr [rbx+12A4h], 3F333333h ; shot accuracy, 0.7f
    AmmoSetBlock:
        lea rdx, beeBulletSet
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        ; set ammo type
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        mov [rbx+12A0h], eax
        ; set ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+1350h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+1354h], xmm1 ;g
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+1358h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+56]
        movss dword ptr [rbx+135Ch], xmm1 ;g

    AmmoAliveBlock:
        lea rdx, beeBulletAlive
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoAccuracyBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+1318h], ecx ; AmmoAlive

    AmmoAccuracyBlock:
        lea rdx, beeBulletAccuracy
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        movss xmm0, dword ptr [rax+rcx*4+8]
        movss dword ptr [rbx+12A4h], xmm0 ; shot accuracy

    AmmoExSetBlock:
        lea rdx, beeBulletExSet
        lea rcx, [rbx+0B0h]
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je OriginalEndBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        
        ; change shot count
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        imul eax, [rbx+13F0h] ; old * this
        ; check 0
        mov edx, 1
        cmp eax, edx
        cmovl eax, edx ; if < 1
        ; write value
        mov [rbx+13F0h], eax

        ; change ammo speed
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        mulss xmm0, dword ptr [rbx+1314h] ; old * this
        movss dword ptr [rbx+1314h], xmm0
        ; change ammo damage
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+32]
        mulss xmm0, dword ptr [rbx+131Ch] ; old * this
        movss dword ptr [rbx+131Ch], xmm0
        ; change ammo explosion range
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+1328h], xmm0
        ; change ammo penetration
        movsxd rax, dword ptr [rcx+8]
        lea rax, [rax+rcx+48] ; store address
        cmp dword ptr [rax+8],0
        setne al
        mov byte ptr [rbx+132Ch], al
        ; change ammo size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+68]
        movss dword ptr [rbx+1334h], xmm0
        ; change ammo hit size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+80]
        movss dword ptr [rbx+1338h], xmm0
        ; change ammo hit impulse
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+92]
        movss dword ptr [rbx+133Ch], xmm0

    OriginalEndBlock:
        mov rbx, qword ptr [rsp+78h]
        movups xmm6, xmmword ptr [rsp+50h]
        movups xmm8, xmmword ptr [rsp+40h]
        add rsp, 60h
        pop rdi
        ret 
        int 3
        ;cmp dword ptr [rbx+418],1
        ;mov dword ptr [rbx+1360],3F800000
        ;mov byte ptr [rbx+132E],1

ASMxgsOCgiantBee ENDP

; set ammo

ASMxgsOCgiantBeeAmmo proc

        movups xmmword ptr [rbp-50h], xmm0
        movups xmm0, xmmword ptr [rbp+10h]
        movdqu xmmword ptr [rbp-40h], xmm0
    ; set new ammo type
        mov eax, [r14+12A0h]
        cmp eax, 0
        jle returnNext
        cmp eax, 14
        jg returnNext
        ; get address
        lea rdx, InsectAmmoType
        cdqe
        mov rax, [rdx+rax*8]
        ; call
        lea r9, [rbp+60h]
        lea r8, [rbp-70h]
        mov rcx, vedf125AB68
        lea rdx, [rsp+40h]
        mov rcx, [rcx]
        call rax
        ; return to original
        jmp giantBeeAmmoRetAddr
    returnNext:
        jmp giantBeeAmmoNextAddr
        int 3

ASMxgsOCgiantBeeAmmo ENDP

END