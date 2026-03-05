.data
extern Vehicle403TankMainFireRetAddr : qword

.code

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