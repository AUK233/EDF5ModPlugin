.data
; Use other asm functions
extern edf5BDF30Address : qword
extern InsectAmmoType_Func : qword
extern InsectAmmoType_Size : dword
;extern edf6136C0 : proto
extern vedf125AB68 : qword
extern GiantAntNormalShotRetAddr : qword
extern InsectAmmoType : qword

extern aligned_mallocAddr : qword
extern GiantAntNormalShotFireRetAddr : qword

; L"ant_BulletClass"
antBulletClass db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,108,0,97,0,115,0,115,0,0,0
align 16
; L"ant_BulletAlive"
antBulletAlive db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
align 16
; L"ant_BulletColor"
antBulletColor db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
align 16
; L"ant_BulletExSet"
antBulletExSet db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0
align 16
; L"ant_BulletFireHeight"
antBulletHeight db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,70,0,105,0,114,0,101,0,72,0,101,0,105,0,103,0,104,0,116,0,0,0

.code

ASMxgsOCgiantAnt proc

        test rax, rax
        je ofs4694E8
        mov rdx, rbx
        mov rcx, rax
        call ASMGiantAntInitEX
    ofs4694E8:
        add rsp, 30h
        pop rbx
        ret 
        int 3

ASMxgsOCgiantAnt ENDP

align 16

ASMGiantAntInitEX proc

        mov [rsp+8], rbx
        push rsi
        sub rsp, 50h
        mov rbx, rcx
        lea rsi, [rcx+0B0h]
        ; initialization
        xor rax, rax
        mov [rcx+1930h], rax ; ammo call address
        mov [rcx+1938h], rax ; ammo size
        mov dword ptr [rcx+1940h], 3DCCCCCDh ; accuracy, 0.1f
        mov dword ptr [rcx+1944h], 90 ; burst count
        mov [rcx+1948h], rax
        ; end
    AmmoTypeBlock:
        lea rdx, antBulletClass
        mov rcx, rsi
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je ReturnAddress
        mov r8, [rsi]
        cdqe ; eax => rax
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        movsxd rax, dword ptr [rdx+8]
        add rdx, rax
        ; node 0 is ammo type
        movsxd rcx, dword ptr [rdx+8]
        lea rax, InsectAmmoType_Func
        mov rax, [rax+rcx*8]
        mov [rbx+1930h], rax ; address
        lea rax, InsectAmmoType_Size
        mov eax, [rax+rcx*4]
        mov [rbx+1938h], eax ; size
        ; node 1 is accuracy
        mov eax, dword ptr [rdx+12+8]
        mov [rbx+1940h], eax
        ; node 2 is burst count
        mov eax, dword ptr [rdx+24+8]
        mov [rbx+1944h], eax
        
    ReturnAddress:
        add rsp, 50h
        pop rsi
        mov rbx, [rsp+8]
        ret
        int 3

ASMGiantAntInitEX ENDP

align 16

ASMGiantAntUpdateAttack proc

    AmmoAliveBlock:
        lea rdx, antBulletAlive
        mov rcx, rsi
        call edf5BDF30Address ; read sgo node
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
        call edf5BDF30Address ; read sgo node
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
        call edf5BDF30Address ; read sgo node
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
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je ofs1FF113 ; if node does not exist, jump
        ; read float in node 
        mov r8, [rsi]
        cdqe
        movsxd rdx, dword ptr [r8+0Ch]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        movss xmm0, dword ptr [rax+rcx*4+8]
        movss dword ptr [rbx+1320h], xmm0 ; shot height

    ofs1FF113:
        movaps xmm6, [rsp+90h]
        add rsp, 0A8h
        pop rsi
        pop rbx
        ret 
        int 3

ASMGiantAntUpdateAttack ENDP

align 16

ASMGiantAntNormalShot proc

        mov rax, vedf125AB68
        lea r9, [rbp+50h]
        lea r8, [rbp-80h]
        lea rdx, [rsp+40h]
        mov rcx, [rax]
        cmp qword ptr [rsi+1930h], 0
        je ofs1FF6FF
        mov [rsp+20h], rsi
        call ASMGiantAntNormalShotFire
        jmp GiantAntNormalShotRetAddr
    ofs1FF6FF:
        call InsectAmmoType+8;AcidBullet01
        jmp GiantAntNormalShotRetAddr
        int 3

ASMGiantAntNormalShot ENDP

align 16

ASMGiantAntNormalShotFire proc

        mov rax, rsp
        push rsi
        push rdi
        push r12
        push r14
        push r15
        sub rsp, 60h
        mov qword ptr [rax-60h], -2
        mov [rax+8h], rbx
        mov [rax+10h], rbp
        mov r15, [rax+28h]
        mov rdi, r9
        mov rsi, rdx
        mov r14, rcx
        xor r12d, r12d
        mov dword ptr [rsp+20h], r12d
        xorps xmm0, xmm0
        movdqu [rax-58h], xmm0
        mov [r9+8h], r8
        mov [r9+10h], r12
        mov [r9+18h], r12
        lea rax, [rax-58h]
        mov [r9+20h], rax
        mov edx, 10h
        mov ecx, [r15+1938h] ; get size
        call aligned_mallocAddr
        mov [rsp+0A0h], rax
        test rax, rax
        je ofs204439
        mov rdx, rdi
        mov rcx, rax
        call qword ptr [r15+1930h]
        mov rbp, rax
        jmp ofs20443C
    ofs204439:
        mov rbp, r12
    ofs20443C:
        mov rcx, [rsp+30h]
        jmp GiantAntNormalShotFireRetAddr
        int 3

ASMGiantAntNormalShotFire ENDP

END