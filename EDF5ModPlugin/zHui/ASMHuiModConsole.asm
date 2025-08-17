.data

extern module_ActivateModConsole : proto

extern getChatWideStrinRetAddr : qword

.code

ASMModConsole_GetPlayerAddress proc

        mov r8, 10000h
        mov rax, [rcx]
        cmp rax, r8
        jb returnZero
        mov rax, [rax+238h]
        cmp rax, r8
        jb returnZero
        mov rax, [rax+290h]
        ; +1d0 is camera position
        cmp rax, r8
        jb returnZero
        ; mov rax, [rax+10h]
        ; cmp rax, r8
        ; jb returnZero
        ret
        
    returnZero:
        xor rax, rax
        ret
        int 3

ASMModConsole_GetPlayerAddress ENDP

align 16

ASMModConsole_GetCameraPostion proc

        mov rax, [rcx]
        test rax, rax
        jz returnZero
        mov rax, [rax+238h]
        test rax, rax
        jz returnZero
        lea rax, [rax+1D0h]
    returnZero:
        ret
        int 3

ASMModConsole_GetCameraPostion ENDP

align 16

ASMgetChatWideString proc

	mov rcx, r12 ; r12 is wstring
	call module_ActivateModConsole
	mov qword ptr [rbp+10h], 7
	xor edi, edi
	mov [rbp+8], rdi
	mov [rbp-8], di
	jmp getChatWideStrinRetAddr
	int 3

ASMgetChatWideString ENDP

END
