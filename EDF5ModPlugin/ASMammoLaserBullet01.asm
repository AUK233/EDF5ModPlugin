.data

extern ammoLaserBullet01RetAddr : qword

.code

ASMammoLaserBullet01 proc
    ; original
        cmp dword ptr [rdi+4], 11
        jle ofs155438
        movsxd rax, dword ptr [rdi+8]
        mov eax, dword ptr [rax+rdi+140]
        mov dword ptr [rsi+744h], eax
    setType:
        cmp dword ptr [rdi+4], 12 ; this is 13th node
        jle ofs155438 ; so if <= 12, jump
        movsxd rax, dword ptr [rdi+8]
        mov eax, dword ptr [rax+rdi+152]
        cmp eax, 1 ; check type
        jne ofs155438
        mov eax, -2
        and word ptr [rsi+20Ch], ax ; no building destruction
        mov dword ptr [rsi+400h], 1 ; no explosive knockout

    ofs155438:
        jmp ammoLaserBullet01RetAddr
        int 3

ASMammoLaserBullet01 ENDP

END