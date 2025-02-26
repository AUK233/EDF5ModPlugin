.data

extern module_LoadAccessory_ExtraWeapon : proto
extern eGetAccessoryValueAddr : qword
extern eGetAccessoryINT32Addr : qword

.code

ASMeAccessoryEnhancement proc

        mov rcx, rdi
        lea r11, [rsp+2B0h]
        mov rbx, [r11+28h]
        mov rsi, [r11+30h]
        mov rdi, [r11+38h]
        movaps xmm6, [r11-10h]
        movaps xmm7, [r11-20h]
        movaps xmm8, [r11-30h]
        mov rsp, r11
        pop r15
        pop r14
        pop rbp
        jmp module_LoadAccessory_ExtraWeapon 
        int 3

ASMeAccessoryEnhancement ENDP

align 16

ASMeGetAccessoryValue proc
    jmp eGetAccessoryValueAddr
    int 3
ASMeGetAccessoryValue ENDP

align 16

ASMeGetAccessoryINT32 proc
    jmp eGetAccessoryINT32Addr
    int 3
ASMeGetAccessoryINT32 ENDP

END
