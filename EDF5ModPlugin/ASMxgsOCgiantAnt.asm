.data
; Use other asm functions
extern edf5BDF30 : proto
;extern edf6136C0 : proto

; L"ant_BulletClass"
antBulletClass db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,108,0,97,0,115,0,115,0,0,0
; L"ant_BulletAlive"
antBulletAlive db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"ant_BulletColor"
antBulletColor db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
; L"ant_BulletExSet"
antBulletExSet db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0
; L"ant_BulletFireHeight"
antBulletHeight db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,70,0,105,0,114,0,101,0,72,0,101,0,105,0,103,0,104,0,116,0,0,0

.code

ASMxgsOCgiantAnt proc

    AmmoAliveBlock:
        lea rdx, antBulletAlive
        mov rcx, rsi
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoColorBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rsi]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+12D8h], ecx ; AmmoAlive

    AmmoColorBlock:
        lea rdx, antBulletColor
        mov rcx, rsi
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rsi]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address
        ; change ammo color
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+8]
        movss dword ptr [rbx+1310h], xmm0 ;r
        movss xmm1, dword ptr [rax+rcx+20]
        movss dword ptr [rbx+1314h], xmm1 ;g
        movss xmm0, dword ptr [rax+rcx+32]
        movss dword ptr [rbx+1318h], xmm0 ;b
        movss xmm1, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+131Ch], xmm1 ;a

    AmmoExSetBlock:
        lea rdx, antBulletExSet
        mov rcx, rsi
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je ShotHeightBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rsi]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rcx, [rax+rcx*4] ; store ptr address

        ; change shot count
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        imul eax, [rbx+13B0h] ; old * this
        ; check 0
        mov edx, 1
        cmp eax, edx
        cmovl eax, edx ; if < 1
        ; write value
        mov [rbx+13B0h], eax

        ; change ammo speed
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+20]
        mulss xmm0, dword ptr [rbx+12D4h] ; old * this
        movss dword ptr [rbx+12D4h], xmm0
        ; change ammo damage
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+32]
        mulss xmm0, dword ptr [rbx+12DCh] ; old * this
        movss dword ptr [rbx+12DCh], xmm0
        ; change ammo explosion range
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+44]
        movss dword ptr [rbx+12E8h], xmm0
        ; change ammo penetration
        movsxd rax, dword ptr [rcx+8]
        lea rax, [rax+rcx+48] ; store address
        cmp dword ptr [rax+8],0
        setne al
        mov byte ptr [rbx+12ECh], al
        ; change ammo size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+68]
        movss dword ptr [rbx+12F4h], xmm0
        ; change ammo hit size
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+80]
        movss dword ptr [rbx+12F8h], xmm0
        ; change ammo hit impulse
        movsxd rax, dword ptr [rcx+8]
        movss xmm0, dword ptr [rax+rcx+92]
        movss dword ptr [rbx+12FCh], xmm0

    ShotHeightBlock:
        lea rdx, antBulletHeight
        mov rcx, rsi
        call edf5BDF30 ; read sgo node
        cmp eax, -1
        je OriginalEndBlock ; if node does not exist, jump
        ; read float in node 
        mov r8, [rsi]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        movss xmm0, dword ptr [rax+rcx*4+8]
        movss dword ptr [rbx+1320h], xmm0 ; shot height

    OriginalEndBlock:
        movups xmm6, xmmword ptr [rsp+90h]
        add rsp, 0A8h
        pop rsi
        pop rbx
        ret

; Useless
;lea rdx, antBulletClass
;mov rcx, rsi
;call edf5BDF30 ; read sgo node
;cmp eax, -1
;je AmmoAliveBlock
;mov r8, [rsi]
;cdqe
;movsxd rdx, dword ptr [r8+0Ch]
;lea rcx, [rax+rax*2]
;lea rax, [r8+rdx]
;lea rax, [rax+rcx*4]
;movsxd rdx, dword ptr [rax+8]
;add rdx, rax
;mov rcx, vedf125AB68
;call edf6136C0
;mov [rbx+1250h], rax

ASMxgsOCgiantAnt ENDP

END