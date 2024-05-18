.data

extern aligned_mallocAddr : qword
extern vftable_SentryGunBulletOpen01 : qword
extern ammoSentryGunBullet01Initialize : qword

extern ammoSentryGunBulletOpen01InitRetAddr : qword

extern ammoSentryGunBullet01FuncP0 : qword

extern ammoSentryGunBulletOpen01ShotTRetAddr : qword
extern ammoSentryGunBulletOpen01ShotFRetAddr : qword

.code

ASMammoSentryGunBulletOpen01 proc

        push rbx
        sub rsp, 30h
        mov qword ptr [rsp+20h], -2
        mov rbx, rdx
        mov edx, 10h
        mov ecx, 6F0h ; old is 6E0
        call aligned_mallocAddr
        mov [rsp+50h], rax
        test rax, rax
        je ofs46A348
        mov rdx, rbx
        mov rcx, rax
        call ammoSentryGunBullet01Initialize
    ; new
        mov r11, rax
        lea rax, vftable_SentryGunBulletOpen01+8
        mov [r11], rax
        mov dword ptr [r11+6E0h], 0 ; is active

    ofs46A348:
        add rsp, 30h
        pop rbx
        ret

ASMammoSentryGunBulletOpen01 ENDP

align 16

ASMammoSentryGunBulletOpen01Init proc

        movsxd rax, dword ptr [r14+8]
        lea rdx, [rax+120]
        add rdx, r14
        mov eax, [rdx+8] ; get node10
        cmp dword ptr [rdx], 0
        jne ofs174678
        cdqe
        mov eax, [rdx+rax+8] ; get weapon index
        cmp eax, 3
        ja ofs174678

        mov rcx, [rdi+50h]
        cdqe
        mov rcx, [rcx+rax*8]
        mov rdx, [rcx+18h]
        mov rdx, [rdx+10h] ; get weapon pointer

        lea rcx, [rdx+8E8h] ; get ammo
        lock inc dword ptr [rcx]
        mov [rsi+8F0h], eax
        mov [rsi+0B70h], rcx
        jmp ammoSentryGunBulletOpen01InitRetAddr
    ofs174678:
        mov [rsi+8F0h], eax ; set AmmoCount
        mov [rsi+0B70h], r13 ; set to 0
    ofs17467E:
        jmp ammoSentryGunBulletOpen01InitRetAddr
        int 3

ASMammoSentryGunBulletOpen01Init ENDP

align 16

ASMammoSentryGunBulletOpen01FuncP0 proc

        cmp dword ptr [rcx+6E0h], 0
        jne ofs173050 ; check is active
        cmp dword ptr [rcx+610h], 0
        jg ofs173050 ; if time count > 0
        sub rsp, 28h
        mov dword ptr [rcx+6E0h], 1
        mov rax, [rcx]
        lea r8, [rcx+6E0h]
        mov edx, 2100h
        call qword ptr [rax+30h]
        add rsp, 28h
        ret
    ofs173050:
        jmp ammoSentryGunBullet01FuncP0
        int 3

ASMammoSentryGunBulletOpen01FuncP0 ENDP

align 16

ASMammoSentryGunBulletOpen01Shot proc

        mov rdx, [rdi+0B70h] ; get weapon ammo pointer
        test rdx, rdx
        je ofs174AEF
        mov eax, [rdx]
        cmp eax, 0
        jle AmmoEmpty
        dec eax
        mov [rdi+8E8h], r14d
        mov rcx, rdi
        mov [rdx], eax
        jmp ammoSentryGunBulletOpen01ShotTRetAddr
    AmmoEmpty:
        mov dword ptr [rdi+8ECh], 16
        jmp ammoSentryGunBulletOpen01ShotFRetAddr

    ofs174AEF:
        mov eax, [rdi+8ECh]
        cmp eax, [rdi+8F0h]
        jae ofs174BA1 ; check self ammo count
        inc eax
        mov [rdi+8E8h], r14d
        mov rcx, rdi
        mov [rdi+8ECh], eax
        jmp ammoSentryGunBulletOpen01ShotTRetAddr
    ofs174BA1:
        jmp ammoSentryGunBulletOpen01ShotFRetAddr
        int 3

ASMammoSentryGunBulletOpen01Shot ENDP

END