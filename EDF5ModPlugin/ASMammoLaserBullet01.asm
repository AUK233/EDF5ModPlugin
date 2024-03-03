.data

extern ammoLaserBullet01RetAddr : qword
extern ammoLaserBullet01HitRetAddr : qword

;L"Particle3_k.dds"
aLaserBullet01t01 DB 50h, 0, 61h, 0, 72h, 0, 74h, 0, 69h, 0, 63h, 0, 6Ch, 0, 65h, 0, 33h, 0, 5Fh, 0, 6Bh, 0, 2Eh, 0, 64h, 0, 64h, 0, 73h, 0, 0, 0
align 16
;L"火炎_02.dds"
aLaserBullet01t02 DB 6Bh, 70h, 8Eh, 70h, 5Fh, 0, 30h, 0, 32h, 0, 2Eh, 0, 64h, 0, 64h, 0, 73h, 0, 0, 0

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
        ; adjust hit FX size
        movss xmm0, dword ptr [rsi+368h]
        ;movaps xmm1, xmm0
        ;mulss xmm1, dword ptr [rsi+740h]
        ;movss dword ptr [rsi+740h], xmm1
        mulss xmm0, dword ptr [rsi+714h]
        movss dword ptr [rsi+714h], xmm0

    ofs155438:
        jmp ammoLaserBullet01RetAddr
        int 3

ASMammoLaserBullet01 ENDP

align 16

ASMammoLaserBullet01Hit proc
    ; check flag
        cmp dword ptr [r14+78h], -1
        jne original
        xor eax, eax ; set 0
        mov [r14+78h], eax
        mov [r12+198h], eax
        mov [r12+1B0h], eax
        ; set texture
        lea r8, aLaserBullet01t02 ; 火炎_02
        jmp ofs15894B

    original:
        lea r8, aLaserBullet01t01 ; Particle3_k
    ofs15894B:
        lea rdx, [rbp+130h]
        jmp ammoLaserBullet01HitRetAddr
        int 3

ASMammoLaserBullet01Hit ENDP

END