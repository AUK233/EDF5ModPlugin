.data
extern playerDmgRetAddress : qword
extern displayDamageIndex : dword
extern damageTempValue : dword
extern playerAddress : qword

.code
ASMrecordPlayerDamage proc

        cmp displayDamageIndex, 1
        jl ofs2DAA59
        mov rax, [rsi+10h]
        cmp rax, playerAddress
        je getDamageBlock
        cmp dword ptr [rsi+24h], 0
        jne ofs2DAA59
        mov rcx, playerAddress
        mov rcx, [rcx+1168h]
        cmp rax, rcx
        jne ofs2DAA59
    getDamageBlock:
        movd ecx, xmm0
        addss xmm0, damageTempValue
        movd eax, xmm0
        xchg damageTempValue, eax
        inc eax
        xchg damageTempValue+4, eax
        ;movss damageTempValue, xmm0
        movd xmm0, ecx

    ofs2DAA59:
        addss xmm0, dword ptr [rdi+1FCh]
        minss xmm0, dword ptr [rdi+1F8h]
        maxss xmm0, dword ptr [rdi+1F4h]
        jmp playerDmgRetAddress
        int 3

ASMrecordPlayerDamage ENDP

ASMresetPlayerDamageTemp proc

    xchg [rcx], rdx
    ret

ASMresetPlayerDamageTemp ENDP

END