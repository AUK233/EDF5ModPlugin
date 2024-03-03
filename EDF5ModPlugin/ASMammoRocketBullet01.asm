.data

extern ammoRocketBullet01RetAddr : qword

.code

ASMammoRocketBullet01 proc
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+8]
        mov [rdi+5D0h], eax
        movsxd rax, dword ptr [rcx+8]
        mov r12d, [rax+rcx+12+8]
        movsxd rax, dword ptr [rcx+8]
        mov r15d, [rax+rcx+24+8]
        movsxd rax, dword ptr [rcx+8]
        movss xmm6, dword ptr [rax+rcx+36+8]
    ; new
        cmp dword ptr [rcx+4], 4
        jle ofs171D59 ; if <= 4
        movsxd rax, dword ptr [rcx+8]
        mov eax, [rax+rcx+48+8]
        test eax, eax
        je ofs171D59
        mov dword ptr [rdi+400h], 1 ; no explosive knockout
        cmp eax, 2
        jne ofs171D59
        and word ptr [rdi+20Ch], -2 ; no building destruction

    ofs171D59:
        jmp ammoRocketBullet01RetAddr
        int 3

ASMammoRocketBullet01 ENDP

END