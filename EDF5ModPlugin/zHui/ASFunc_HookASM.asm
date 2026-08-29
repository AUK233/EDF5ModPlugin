.data

extern ASFunc_Hook_ResetControllerAssign : proto

.code

ASMResetControllerAssign proc

	mov dword ptr [rbx+9940h], 0
	add rsp, 20h
	pop rbx
	jmp ASFunc_Hook_ResetControllerAssign
	int 3

ASMResetControllerAssign ENDP

END
