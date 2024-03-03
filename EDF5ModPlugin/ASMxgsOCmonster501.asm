.data
; Use other asm functions
extern edf3AE530 : proto
extern edf5BDF30Address : qword

; L"ChangeModelParam"
m501ChangeModel db 67,0,104,0,97,0,110,0,103,0,101,0,77,0,111,0,100,0,101,0,108,0,80,0,97,0,114,0,97,0,109,0,0,0
align 16
; L"BulletAlive"
m501BulletAlive db 66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
align 16
; L"BulletColor"
m501BulletColor db 66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
align 16
; L"BulletExSet"
m501BulletExSet db 66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCmonster501 proc

        lea rdx, m501ChangeModel
        lea rcx, qword ptr [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        movsxd rcx, eax
        cmp ecx, -1
        je AmmoAliveBlock
    ChangeModelBlock:
        mov rax, qword ptr [rbx+0B0h]
        movsxd rdx, dword ptr [rax+0Ch]
        add rdx, rax
        lea rcx, [rcx+rcx*2]
        lea rdx, [rdx+rcx*4]
        lea rcx, qword ptr [rbx+680h]
        call edf3AE530

    AmmoAliveBlock:
        lea rdx, m501BulletAlive
        lea rcx, qword ptr [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        movsxd rcx, eax
        cmp ecx, -1
        je AmmoColorBlock
    AmmoAliveBlock2:
        mov rax, qword ptr [rbx+0B0h]
        movsxd rdx, dword ptr [rax+0Ch]
        add rdx, rax
        lea rcx, [rcx+rcx*2]
        mov eax, [rdx+rcx*4+8]
        mov [rbx+1638h], eax ; AmmoAlive

    AmmoColorBlock:
        lea rdx, m501BulletColor
        lea rcx, qword ptr [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        movsxd rcx, eax
        cmp ecx, -1
        je AmmoExSetBlock
    AmmoColorBlock2:
        mov rax, qword ptr [rbx+0B0h]
        movsxd rdx, dword ptr [rax+0Ch]
        add rdx, rax
        lea rcx, [rcx+rcx*2]
        lea rcx, [rdx+rcx*4] ; store ptr address
        ; change ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+8]
        movss dword ptr [rbx+1670h], xmm0 ;r
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+1674h], xmm0 ;g
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+1678h], xmm0 ;b
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+167Ch], xmm0 ;a

    AmmoExSetBlock:
        lea rdx, m501BulletExSet
        lea rcx, qword ptr [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        movsxd rcx, eax
        cmp ecx, -1
        je OriginalEndBlock
    AmmoExSetBlock2:
        mov rax, qword ptr [rbx+0B0h]
        movsxd rdx, dword ptr [rax+0Ch]
        add rdx, rax
        lea rcx, [rcx+rcx*2]
        lea rcx, [rdx+rcx*4] ; store ptr address
        ; change shot interval
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        mov dword ptr [rbx+1714h], eax
        ; change ammo speed
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+1634h], xmm0
        ; change ammo damage, invalid, overwrite when attack
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+163Ch], xmm0
        ; change ammo explosion range
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+1648h], xmm0
        ; change ammo penetration
        movsxd rax, dword ptr [rcx+8]
        lea rax, [rax+rcx+48] ; store address
        cmp dword ptr [rax+8],0
        setne al
        mov byte ptr [rbx+164Ch], al
        ; change ammo size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+68]
        movss dword ptr [rbx+1654h], xmm0
        ; change ammo hit size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+80]
        movss dword ptr [rbx+1658h], xmm0
        ; change ammo hit impulse
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+92]
        movss dword ptr [rbx+165Ch], xmm0

    OriginalEndBlock:
        mov rax, rbx
        mov rbx, qword ptr [rsp+78h]
        add rsp, 50h
        pop rdi
        pop rsi
        pop rbp
        ret 
        int 3

ASMxgsOCmonster501 endp

END