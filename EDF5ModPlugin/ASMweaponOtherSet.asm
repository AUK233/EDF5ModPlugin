.data
extern Vehicle403TankMainFireRetAddr : qword

extern playerDmgRetAddress : qword
extern displayDamageIndex : dword
extern damageTempValue : dword
extern playerAddress : qword

.code
ASMrecordPlayerDamage proc

        test cl, 10
        jne ofs2DAA61
        mulss xmm0, dword ptr [rdi+294h]
        movss xmm1, dword ptr [rsp+30h] ; armor is 0, other is 1
        comiss xmm1, xmm6
        je checkDamageShow
        mulss xmm0, xmm7 ; x "friendly damage rate"
    checkDamageShow:
        cmp displayDamageIndex, 1
        jne ofs2DAA41
        mov rax, [rsi+10h]
        cmp rax, playerAddress
        je getDamageBlock
        cmp dword ptr [rsi+24h], 0
        jne ofs2DAA41
        mov rcx, playerAddress
        mov rcx, [rcx+1168h]
        cmp rax, rcx
        jne ofs2DAA41
    getDamageBlock:
        movaps xmm2, xmm0
        addss xmm2, damageTempValue
        movss damageTempValue, xmm2

    ofs2DAA41:
        comiss xmm1, xmm6
        je ofs2DAA61
        mulss xmm0, xmm1
        movss xmm1, dword ptr [rdi+1FCh]
        subss xmm1, xmm0
        minss xmm1, dword ptr [rdi+1F8h]
        maxss xmm1, dword ptr [rdi+1F4h]
        movss dword ptr [rdi+1FCh], xmm1
    ofs2DAA61:
        jmp playerDmgRetAddress
        int 3

ASMrecordPlayerDamage ENDP

align 16

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
        int 3

ASMVehicle403TankMainFire ENDP

END