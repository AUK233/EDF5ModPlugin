.data

extern togui_GetDXGISwapChain : proto
extern GetDXGISwapChainRetAddr : qword

extern xgsSystemSetPlayerSlotRetAddr : qword

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

align 16

ASMxgsSystemSetPlayerSlot proc

	mov rax, [rbx+10h]
	mov eax, [rax+60h]
	imul r14, rax, 188h
	jmp xgsSystemSetPlayerSlotRetAddr
	int 3

ASMxgsSystemSetPlayerSlot ENDP

END
