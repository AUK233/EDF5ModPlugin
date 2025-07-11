.data

extern Initialize_NGX_dlss : proto
extern InitializeDLSS : proto
extern Release_NGX_dlss : proto
extern Evaluate_NGX_dlss : proto

extern dx11CreateDeviceRetAddr : qword
extern dxgiSwapChainPresentRetAddr : qword

.code

ASMdx11CreateDevice proc

	xor r9d, r9d
	xor r8d, r8d
	xor ecx, ecx
	call Initialize_NGX_dlss
	mov esi, eax
	jmp dx11CreateDeviceRetAddr
	int 3

ASMdx11CreateDevice ENDP

align 16

ASMsysExitGame proc

	add rsp, 0B0h
	pop rdi
	pop rsi
	pop rbp
	jmp Release_NGX_dlss
	int 3

ASMsysExitGame ENDP

align 16

ASMdxgiSwapChainPresent proc

	mov rdi, [rsp+20h]
	mov r8, [rdi+0C8h] 
	mov rdx, [rdi+0B8h] ; ID3D11Device*
	mov rcx, [rdi+0B8h+8] ; ID3D11DeviceContext*
	call Evaluate_NGX_dlss
	;
	xor r8d, r8d
	mov rax, dxgiSwapChainPresentRetAddr
	mov edx, [rsp+28h]
	push rax
	mov rcx, [rdi+0C8h] ; IDXGISwapChain*
	mov rax, [rcx]
	jmp qword ptr [rax+40h] ; Present
	int 3

ASMdxgiSwapChainPresent ENDP

END