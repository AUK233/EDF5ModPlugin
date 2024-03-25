.data

extern edf5BDF30Address : qword
extern InsectAmmoType : qword

extern _AnimationEvent_BurstFire : word
extern _AnimationEvent_BurstFireStop : word
extern rva27380 : qword
extern rva211070 : qword
extern rva27570 : qword
extern GiantBeeAnimationEventRetAddr : qword

extern vedf125AB68 : qword
extern GiantBeeNormalShotRetAddr : qword

extern GiantBeeNormalShotAddr : qword

;L"bee_BulletSet"
beeBulletSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,83,0,101,0,116,0,0,0
align 16
; L"bee_BulletAccuracy"
beeBulletAccuracy db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,99,0,99,0,117,0,114,0,97,0,99,0,121,0,0,0
align 16
; L"bee_BulletAlive"
beeBulletAlive db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
align 16
; L"bee_BulletExSet"
beeBulletExSet db 98,0,101,0,101,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0
align 16
; L"bee_BurstShot"
beeBurstShot db 98,0,101,0,101,0,95,0,66,0,117,0,114,0,115,0,116,0,83,0,104,0,111,0,116,0,0,0

.code

ASMxgsOCgiantBee proc

        ;mov dword ptr [rbx+12A0h], 0 ; ammo type
        ; initialization
        xor rax, rax
        mov [rbx+1C40h], rax ; ammo call address
        mov [rbx+1C48h], eax ; shot count in burst
        mov dword ptr [rbx+1C4Ch], 3F333333h ; shot accuracy, 0.7f
        mov [rbx+1C50h], rax ; burst state, current burst count
        mov [rbx+1C58h], rax ; burst count, burst interval
        mov [rbx+1C60h], rax ; burst interval count,
        mov [rbx+1C68h], rax
        ; end
    AmmoSetBlock:
        lea rdx, beeBulletSet
        lea rcx, [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoAliveBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        movsxd rax, dword ptr [rdx+8]
        add rdx, rax ; store ptr address
        ; set ammo type
        movsxd rcx, dword ptr [rdx+8]
        lea rax, InsectAmmoType
        mov rax, [rax+rcx*8]
        mov [rbx+1C40h], rax ; address
        ; set ammo color
        mov eax, dword ptr [rdx+12+8]
        mov [rbx+1350h], eax ;r
        mov eax, dword ptr [rdx+24+8]
        mov [rbx+1354h], eax ;g
        mov eax, dword ptr [rdx+36+8]
        mov [rbx+1358h], eax ;r
        mov eax, dword ptr [rdx+48+8]
        mov [rbx+135Ch], eax ;g

    AmmoAliveBlock:
        lea rdx, beeBulletAlive
        lea rcx, [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoAccuracyBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        mov ecx, [rax+rcx*4+8]
        mov [rbx+1318h], ecx ; AmmoAlive

    AmmoAccuracyBlock:
        lea rdx, beeBulletAccuracy
        lea rcx, [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je AmmoExSetBlock ; if node does not exist, jump
        ; read int in node 
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        movss xmm0, dword ptr [rax+rcx*4+8]
        movss dword ptr [rbx+1C4Ch], xmm0 ; shot accuracy

    AmmoExSetBlock:
        lea rdx, beeBulletExSet
        lea rcx, [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je BurstShotBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+12]
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

    BurstShotBlock:
        lea rdx, beeBurstShot
        lea rcx, [rbx+0B0h]
        call edf5BDF30Address ; read sgo node
        cmp eax, -1
        je OriginalEndBlock ; if node does not exist, jump
        ; read ptr in node
        mov r8, [rbx+0B0h]
        cdqe
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdx, [rax+rcx*4]
        movsxd rax, dword ptr [rdx+8]
        add rdx, rax ; store ptr address
        ; get burst count
        mov eax, [rdx+8]
        mov [rbx+1C58h], eax
        ; get shot count in burst
        mov eax, [rdx+12+8]
        mov [rbx+1C48h], eax
        ; get burst interval
        mov eax, [rdx+24+8]
        mov [rbx+1C5Ch], eax

    OriginalEndBlock:
        mov rbx, qword ptr [rsp+78h]
        movaps xmm6, xmmword ptr [rsp+50h]
        movaps xmm8, xmmword ptr [rsp+40h]
        add rsp, 60h
        pop rdi
        ret 
        int 3
        ;cmp dword ptr [rbx+418],1
        ;mov dword ptr [rbx+1360],3F800000
        ;mov byte ptr [rbx+132E],1

ASMxgsOCgiantBee ENDP

align 16

ASMGiantBeeBurstFire proc

        xor edx, edx
        mov [rcx+1C60h], edx ; set burst interval count to 0
        mov dword ptr [rcx+1C50h], 1 ; set burst
        mov eax, [rcx+1C58h]
        mov [rcx+1C54h], eax ; set burst count
        mov eax, [rcx+1C48h]
        cmp eax, edx
        jle ReturnAddress ; if <= 0
        mov [rcx+13F0h], eax ; set new shot count
    ReturnAddress:
        ret 
        int 3

ASMGiantBeeBurstFire ENDP

align 16

ASMGiantBeeBurstStop proc

        xor rax, rax
        mov [rcx+1C50h], rax
        mov [rcx+1C60h], eax
        ret 
        int 3

ASMGiantBeeBurstStop ENDP

align 16

ASMGiantBeeAnimationEvent proc

    BurstFireBlock:
        lea rax, ASMGiantBeeBurstFire
        mov [rbp-40h], rax
        mov [rbp-38h], r15d
        mov [rbp-30h], r15w
        mov [rbp-20h], r15
        mov qword ptr [rbp-18h], 7
        mov r8d, 9 ; string length
        lea rdx, _AnimationEvent_BurstFire ; BurstFire
        lea rcx, [rbp-30h]
        call rva27380
        movaps xmm0, [rbp-40h]
        movdqa [rbp-40h], xmm0
        lea r9, [rbp-40h]
        mov r8, rdi
        lea rdx, [rbp-30h]
        mov rcx, rsi
        call rva211070
        mov rdx, [rbp-18h]
        cmp rdx, 8
        jb BurstStopBlock
        mov rcx, [rbp-30h]
        inc rdx
        mov r8d, 2
        call rva27570

    BurstStopBlock:
        lea rax, ASMGiantBeeBurstStop
        mov [rbp-40h], rax
        mov [rbp-38h], r15d
        mov [rbp-30h], r15w
        mov [rbp-20h], r15
        mov qword ptr [rbp-18h], 7
        mov r8d, 14 ; string length
        lea rdx, _AnimationEvent_BurstFireStop ; BurstFire
        lea rcx, [rbp-30h]
        call rva27380
        movaps xmm0, [rbp-40h]
        movdqa [rbp-40h], xmm0
        lea r9, [rbp-40h]
        mov r8, rdi
        lea rdx, [rbp-30h]
        mov rcx, rsi
        call rva211070
        mov rdx, [rbp-18h]
        cmp rdx, 8
        jb ofs2082BB
        mov rcx, [rbp-30h]
        inc rdx
        mov r8d, 2
        call rva27570

    ofs2082BB:
        mov eax, [rdi+430h]
        mov r8d, r15d
        mov rbx, [rdi+7B0h]
        jmp GiantBeeAnimationEventRetAddr
        int 3

ASMGiantBeeAnimationEvent ENDP

align 16

ASMGiantBeeNormalShot proc

        mov rax, vedf125AB68
        lea r9, [rbp+60h]
        lea r8, [rbp-70h]
        lea rdx, [rsp+40h]
        mov rcx, [rax]
        mov rax, [r14+1C40h]
        test rax, rax
        je ofs20A6F4
        call rax
        jmp GiantBeeNormalShotRetAddr
    ofs20A6F4:
        call InsectAmmoType+80 ;NeedleBullet01
        jmp GiantBeeNormalShotRetAddr
        int 3

ASMGiantBeeNormalShot ENDP

align 16

ASMGiantBeeFuncP10 proc

        xor r11d, r11d
        cmp [rdi+1C50h], r11d
        je ofs2092C8 ; if == 0
        ; check current burst count
        mov eax, [rdi+1C54h]
        cmp eax, r11d
        jle ofs2092C8 ; if <= 0
        ; check burst interval count
        mov ecx, [rdi+1C60h]
        cmp ecx, r11d
        jle FireBlock ; if <= 0
        dec ecx
        mov [rdi+1C60h], ecx
        jmp ofs2092C8
    FireBlock:
        dec eax
        mov [rdi+1C54h], eax
        mov eax, [rdi+1C5Ch]
        mov [rdi+1C60h], eax
        mov rcx, rdi
        call GiantBeeNormalShotAddr
    ofs2092C8:
        lea r11, [rsp+0A0h]
        mov rbx, [r11+10h]
        mov rdi, [r11+18h]
        mov rsp, r11
        pop rbp
        ret 
        int 3

ASMGiantBeeFuncP10 ENDP

END