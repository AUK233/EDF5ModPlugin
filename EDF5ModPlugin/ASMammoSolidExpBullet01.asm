.data

extern ammoSolidExpBullet01RetAddr : qword

extern _Common_F0P2 : dword
extern _Common_F0P8 : dword

.code

ASMammoSolidExpBullet01 proc
    ; read base damage
        movss xmm1, dword ptr [rdi+200h]
        movaps xmm0, xmm1
    ; check damage settings
        cmp dword ptr [r14+4], 2
        jle ofs187642 ; if <= 2, because this is 3rd parameter
    ; get data address
        movsxd rax, dword ptr [r14+8]
        add rax, r14
        add rax, 24
        movsxd rcx, dword ptr [rax+8]
        ; get hit damage
        movss xmm6, dword ptr [rax+rcx+8]
        mulss xmm1, xmm6
        movss dword ptr [rdi+200h], xmm1
        ; get explosion damage
        movss xmm6, dword ptr [rax+rcx+20]
        mulss xmm0, xmm6
        movss dword ptr [rdi+5D4h], xmm0
        ; return
        jmp ammoSolidExpBullet01RetAddr

    ofs187642:
        mulss xmm0, _Common_F0P8
        movss dword ptr [rdi+5D4h], xmm0 ; explosion damage
        mulss xmm1, _Common_F0P2
        movss dword ptr [rdi+200h], xmm1 ; hit damage
        jmp ammoSolidExpBullet01RetAddr
        int 3

ASMammoSolidExpBullet01 ENDP

END