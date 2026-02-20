.data
extern Vehicle403TankMainFireRetAddr : qword

extern playerAddress : qword

.code

ASMgetPlayerAddress proc

        mov r8, 10000h
        mov rax, [rcx]
        cmp rax, r8
        jb returnZero
        mov rax, [rax+238h]
        cmp rax, r8
        jb returnZero
        mov rax, [rax+290h]
        cmp rax, r8
        jb returnZero
        mov rax, [rax+10h]
        cmp rax, r8
        jb returnZero
        mov [rdx], rax
        mov rcx, [rax+1168h]
        mov [rdx+8], rcx
        ret
        
    returnZero:
        xor rax, rax
        mov [rdx], rax
        mov [rdx+8], rax
        ret
        int 3

ASMgetPlayerAddress ENDP

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