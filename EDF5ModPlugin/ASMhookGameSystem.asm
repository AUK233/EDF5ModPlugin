.data

extern newSaveDataUnlock : dword
extern gameStartupUnlockAllWeaponRetAddr : qword

extern gameStartupSetMinClassArmorRetAddr : qword

.code

ASMgameStartupUnlockAllWeapon proc
        
    ofs8BF20:
        mov eax, ecx
        add rax, 1381h
        lea rax, [rax+rax*2]
        test byte ptr [r15+rax*4], 1
        jne ofs8BF37
        cmp newSaveDataUnlock, 0
        je ofs8BF39
        mov byte ptr [r15+rax*4], 1
    ofs8BF37:
        inc edi
        ;lea eax, [rdi+1]
        ;cmove eax, edi
        ;mov edi, eax
    ofs8BF39:
        inc ecx
        cmp ecx, edx
        jne ofs8BF20
        mov newSaveDataUnlock, 0 ; activate only once per game run
        jmp gameStartupUnlockAllWeaponRetAddr

ASMgameStartupUnlockAllWeapon ENDP

align 16

ASMgameStartupSetMinClassArmor proc

        mov eax, [r10+r9*4+6D24h]
        cmp eax, 30000
        jae ofs508600
        mov eax, 30000
        mov [r10+r9*4+6D24h], eax
    ofs508600:
        mov ebx, [r10+rdx*4+6E44h]
        jmp gameStartupSetMinClassArmorRetAddr

ASMgameStartupSetMinClassArmor ENDP

END