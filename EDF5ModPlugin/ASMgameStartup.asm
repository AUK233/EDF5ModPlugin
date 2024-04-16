.data

extern vedf125AB90 : qword
extern gameStartupReadVoiceFileRetAddr : qword
extern LoadNewVoiceFilesCPP : proto

.code

ASMgameStartupReadVoiceFile proc

        mov rcx, vedf125AB90
        mov rcx, [rcx]
        call LoadNewVoiceFilesCPP
        mov qword ptr [rbp+98h], 7
        mov [rbp+90h], rdi
        mov [rbp+80h], di
        jmp gameStartupReadVoiceFileRetAddr
        int 3

ASMgameStartupReadVoiceFile ENDP

END
