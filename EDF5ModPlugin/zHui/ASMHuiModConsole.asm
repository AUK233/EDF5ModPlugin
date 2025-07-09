.data

extern module_ActivateModConsole : proto

extern getChatWideStrinRetAddr : qword

.code

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
