.data

extern togui_GetDXGISwapChain : proto
extern GetDXGISwapChainRetAddr : qword

.code

ASMGetDXGISwapChain proc

	mov [rdi+0C8h], rbx
	mov [rdi+30h], eax
	mov rdx, rbx
	call togui_GetDXGISwapChain
	;test cl, cl ;old
	test al, al
	mov eax, [rsp+44h]
	mov [rdi+34h], eax
	mov eax, ebp
	jmp GetDXGISwapChainRetAddr
	int 3

ASMGetDXGISwapChain ENDP

END
