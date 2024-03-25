.data
extern edf3AE530Address : qword
extern edf5BDF30Address : qword
extern InsectAmmoType : qword

extern _AnimationEvent_BurstFire : word
extern _AnimationEvent_BurstFireStop : word
extern rva27380 : qword
extern rva225C80 : qword
extern rva27570 : qword
extern GiantSpiderAnimationEventRetAddr : qword

extern vedf125AB68 : qword
extern InsectSpiderStringBullet02 : qword
extern GiantSpiderNormalShotRetAddr : qword

extern GiantSpiderNormalShotAddr : qword

;L"spd_ChangeModelParam"
spdChangeModel db 115,0,112,0,100,0,95,0,67,0,104,0,97,0,110,0,103,0,101,0,77,0,111,0,100,0,101,0,108,0,80,0,97,0,114,0,97,0,109,0,0,0
align 16
;L"spd_AmmoAlive"
spdAmmoAlive db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,65,0,108,0,105,0,118,0,101,0,0,0
align 16
;L"spd_AmmoClass"
spdAmmoClass db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,67,0,108,0,97,0,115,0,115,0,0,0
align 16
;L"spd_AmmoColor"
spdAmmoColor db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,67,0,111,0,108,0,111,0,114,0,0,0
align 16
;L"spd_AmmoExSet"
spdAmmoExSet db 115,0,112,0,100,0,95,0,65,0,109,0,109,0,111,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCgiantSpider proc

        test rax, rax
        je ofs469538
        mov rdx, rbx
        mov rcx, rax
        call ASMGiantSpiderInitEX
    ofs469538:
        add rsp, 30h
        pop rbx
        ret 
        int 3

ASMxgsOCgiantSpider ENDP

align 16

ASMGiantSpiderInitEX proc

        mov [rsp+8], rbx
        push rsi
        sub rsp, 50h
        mov rbx, rcx
        lea rsi, [rcx+0B0h]
        ; initialization
        xor rax, rax
        mov [rcx+1C40h], rax ; ammo call address
        mov [rcx+1C48h], eax ; shot count in burst
        mov dword ptr [rcx+1C4Ch], 3DCCCCCDh ; accuracy, 0.1f
        mov [rcx+1C50h], rax ; burst state, current burst count
        mov [rcx+1C58h], rax ; burst count, burst interval
        mov [rcx+1C60h], rax ; burst interval count,
        mov [rcx+1C68h], rax
        ; end
    ChangeModelBlock:
        lea rdx, spdChangeModel
        mov rcx, rsi
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock
        mov r8, [rsi]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        lea rcx, [rbx+480h]
        call edf3AE530Address

    AmmoAliveBlock:
        lea rdx, spdAmmoClass
        mov rcx, rsi
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je ReturnAddress
        mov r8, [rsi]
        cdqe ; eax => rax
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        movsxd rax, dword ptr [rdx+8]
        add rdx, rax
        ; node 0 is ammo type
        movsxd rcx, dword ptr [rdx+8]
        lea rax, InsectAmmoType
        mov rax, [rax+rcx*8]
        mov [rbx+1C40h], rax ; address
        ; node 1 is accuracy
        mov eax, dword ptr [rdx+12+8]
        mov [rbx+1C4Ch], eax
        ; node 2 is burst count
        mov eax, dword ptr [rdx+24+8]
        mov [rbx+1C58h], eax
        ; node 3 is shot count in burst
        mov eax, dword ptr [rdx+36+8]
        mov [rbx+1C48h], eax
        ; node 4 is burst interval
        mov eax, dword ptr [rdx+48+8]
        mov [rbx+1C5Ch], eax
        
    ReturnAddress:
        add rsp, 50h
        pop rsi
        mov rbx, [rsp+8]
        ret
        int 3

ASMGiantSpiderInitEX ENDP

align 16

ASMGiantSpiderUpdateAttack proc

    ; new
        lea rdx, spdChangeModel
        lea rcx, [rdi+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        lea rcx, [rdi+480h]
        call edf3AE530Address

    AmmoAliveBlock:
        lea rdx, spdAmmoAlive
        lea rcx, [rdi+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoColorBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rdi+14A8h], ecx ; AmmoAlive
            
    AmmoColorBlock:
        lea rdx, spdAmmoColor
        lea rcx, [rdi+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
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
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je OriginalEndBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rdi+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
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
        movaps xmm6, [rsp+80h]
        add rsp, 90h
        pop r15
        pop r14
        pop rdi
        ret
        int 3

ASMGiantSpiderUpdateAttack ENDP

align 16

ASMGiantSpiderBurstFire proc

        xor edx, edx
        mov [rcx+1C60h], edx ; set burst interval count to 0
        mov dword ptr [rcx+1C50h], 1 ; set burst
        mov eax, [rcx+1C58h]
        mov [rcx+1C54h], eax ; set burst count
        mov eax, [rcx+1C48h]
        cmp eax, edx
        jle ReturnAddress ; if <= 0
        mov [rcx+15D0h], eax ; set new shot count
    ReturnAddress:
        ret 
        int 3

ASMGiantSpiderBurstFire ENDP

align 16

ASMGiantSpiderBurstStop proc

        xor rax, rax
        mov [rcx+1C50h], rax
        mov [rcx+1C60h], eax
        ret 
        int 3

ASMGiantSpiderBurstStop ENDP

align 16

ASMGiantSpiderAnimationEvent proc

    BurstFireBlock:
        lea rax, ASMGiantSpiderBurstFire
        mov [rbp-40h], rax
        mov [rbp-38h], edi
        mov [rbp-30h], di
        mov [rbp-20h], rdi
        mov qword ptr [rbp-18h], 7
        mov r8d, 9 ; string length
        lea rdx, _AnimationEvent_BurstFire ; BurstFire
        lea rcx, [rbp-30h]
        call rva27380
        movaps xmm0, [rbp-40h]
        movdqa [rbp-40h], xmm0
        lea r9, [rbp-40h]
        mov r8, rsi
        lea rdx, [rbp-30h]
        mov rcx, r14
        call rva225C80
        mov rdx, [rbp-18h]
        cmp rdx, 8
        jb BurstStopBlock
        mov rcx, [rbp-30h]
        inc rdx
        mov r8d, 2
        call rva27570

    BurstStopBlock:
        lea rax, ASMGiantSpiderBurstStop
        mov [rbp-40h], rax
        mov [rbp-38h], edi
        mov [rbp-30h], di
        mov [rbp-20h], rdi
        mov qword ptr [rbp-18h], 7
        mov r8d, 14 ; string length
        lea rdx, _AnimationEvent_BurstFireStop ; BurstFire
        lea rcx, [rbp-30h]
        call rva27380
        movaps xmm0, [rbp-40h]
        movdqa [rbp-40h], xmm0
        lea r9, [rbp-40h]
        mov r8, rsi
        lea rdx, [rbp-30h]
        mov rcx, r14
        call rva225C80
        mov rdx, [rbp-18h]
        cmp rdx, 8
        jb ofs21E765
        mov rcx, [rbp-30h]
        inc rdx
        mov r8d, 2
        call rva27570

    ofs21E765:
        mov r15d, [rsi+430h]
        mov r8d, edi
        mov rbx, [rsi+7B0h]
        jmp GiantSpiderAnimationEventRetAddr
        int 3

ASMGiantSpiderAnimationEvent ENDP

align 16

ASMGiantSpiderNormalShot proc

        mov rax, [rsp+0E0h]
        imul rax, r12
        add rax, 269EC3h
        mov [rsp+0E0h], rax
        shr rax, 20h
        shl rax, 10h
        shr rax, 20h
        mov [rbx+1560h], eax

        mov eax, [rbx+218h]
        mov [rbx+14A0h], eax

        lea r9, [rbx+1430h]
        lea r8, [rsp+60h]
        lea rdx, [rsp+30h]
        mov rax, [rbx+1C40h]
        test rax, rax
        je ofs21F99A
        call rax
        jmp GiantSpiderNormalShotRetAddr
    ofs21F99A:
        call InsectSpiderStringBullet02
        jmp GiantSpiderNormalShotRetAddr
        int 3

ASMGiantSpiderNormalShot ENDP

align 16

ASMGiantSpiderFuncP10 proc

        xor r11d, r11d
        cmp [rbx+1C50h], r11d
        je ofs21EE95 ; if == 0
        ; check current burst count
        mov eax, [rbx+1C54h]
        cmp eax, r11d
        jle ofs21EE95 ; if <= 0
        ; check burst interval count
        mov ecx, [rbx+1C60h]
        cmp ecx, r11d
        jle FireBlock ; if <= 0
        dec ecx
        mov [rbx+1C60h], ecx
        jmp ofs21EE95
    FireBlock:
        dec eax
        mov [rbx+1C54h], eax
        mov eax, [rbx+1C5Ch]
        mov [rbx+1C60h], eax
        mov rcx, rbx
        call GiantSpiderNormalShotAddr
    ofs21EE95:
        mov rbx, [rsp+80h]
        add rsp, 70h
        pop rdi
        ret 
        int 3

ASMGiantSpiderFuncP10 ENDP

END