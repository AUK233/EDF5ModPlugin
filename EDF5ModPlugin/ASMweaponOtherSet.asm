.data
extern Vehicle403TankMainFireRetAddr : qword

extern playerDmgRetAddress : qword
extern displayDamageIndex : dword
extern damageTempValue : dword
extern playerAddress : qword

.code
ASMrecordPlayerDamage proc

        cmp displayDamageIndex, 1
        jne ofs2DAA59
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
        mov damageTempValue, eax
        inc eax
        mov damageTempValue+4, eax
        ;xchg damageTempValue, eax
        ;inc eax
        ;xchg damageTempValue+4, eax
        ;movss damageTempValue, xmm0
        movd xmm0, ecx

    ofs2DAA59:
        addss xmm0, dword ptr [rdi+1FCh]
        minss xmm0, dword ptr [rdi+1F8h]
        maxss xmm0, dword ptr [rdi+1F4h]
        jmp playerDmgRetAddress
        int 3

ASMrecordPlayerDamage ENDP

ASMVehicle403TankMainFire proc

        mov r11, [rdi+448h]
        mov eax, r8d
        lea r10, [rax+rax*8]
        mov rax, [r11+r10*8+8]
        test rax, rax
        je ofs3385FF
        cmp dword ptr [rax+8], 0
        je ofs3385FF
        mov rax, [r11+r10*8+10h]
        mov byte ptr [rax+0D9h], 1
        ; check ExtraShotType
        cmp dword ptr [rax+2510h], 1
        jne ofs3385FF
        mov rax, [r11+r10*8+58h]
        test rax, rax
        je ofs3385FF
        mov byte ptr [rax+0D9h], 1
    ofs3385FF:
        jmp Vehicle403TankMainFireRetAddr

ASMVehicle403TankMainFire ENDP

END