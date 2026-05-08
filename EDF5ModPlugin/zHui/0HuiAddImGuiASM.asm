.data

extern module_InitializeD3D11 : proto
extern dx11CreateDeviceRetAddr : qword

extern DLSS_Release : proto

extern DLSS_Draw : proto
extern RenderBufferToScreenBufferRetAddr : qword

extern togui_GetDXGISwapChain : proto
extern GetDXGISwapChainRetAddr : qword

extern DigitProcessor_SetLocalCurrentPlayer : proto
extern xgsSystemSetPlayerSlotRetAddr : qword

extern Config_DisplayDamageType : dword
extern DigitProcessor_GetPlayerHitDamage : proto
extern getInflictDamageFromDamageFuncRetAddress : qword

.code

ASMdx11CreateDevice proc

	xor r9d, r9d
	xor r8d, r8d
	lea rcx, [rbp-49h] ; DXGI_SWAP_CHAIN_DESC
	call module_InitializeD3D11
	mov esi, eax
	jmp dx11CreateDeviceRetAddr
	int 3

ASMdx11CreateDevice ENDP

align 16

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

ASMsysExitGame proc

	add rsp, 0B0h
	pop rdi
	pop rsi
	pop rbp
	jmp DLSS_Release
	int 3

ASMsysExitGame ENDP

align 16

ASMRenderBufferToScreenBuffer proc

	mov rcx, [rdi+8]
	call DLSS_Draw
	mov rax, [rbx]
	add dword ptr [rbx+8], -8
	add dword ptr [rbx+12], 8
	movsxd rcx, dword ptr [rbx+10h]
	jmp RenderBufferToScreenBufferRetAddr
	int 3

ASMRenderBufferToScreenBuffer ENDP

align 16

ASMxgsSystemSetPlayerSlot proc

	mov edx, [rbp+228h]
	mov rcx, [rbx+10h]
	call DigitProcessor_SetLocalCurrentPlayer
	imul r14, rax, 188h
	jmp xgsSystemSetPlayerSlotRetAddr
	int 3

ASMxgsSystemSetPlayerSlot ENDP

align 16

ASMgetInflictDamageFromDamageFunc proc

		test cl, 10h
		jne ofs2DAA61
		mulss xmm0, dword ptr [rdi+294h]
		movss xmm1, dword ptr [rsp+30h] ; armor is 0, other is 1
		comiss xmm1, xmm6 ; if = 0
		je checkObjectExist
		mulss xmm0, xmm7 ; x "friendly damage rate"
	checkObjectExist:
		mov rax, [rsi+10h]
		test rax, rax
		jz giveDamageToObject
		movss xmm2, dword ptr [rax+34h] ; now is our value
		addss xmm2, xmm0
		movss dword ptr [rax+34h], xmm2
		; check display on hit
		cmp Config_DisplayDamageType, 2
		jne giveDamageToObject
		cmp dword ptr [rsi+24h], 0 ; check team id
		jne giveDamageToObject
		mov r8, rax
		mov rdx, rsi
		; xmm0 needs to be returned
		call DigitProcessor_GetPlayerHitDamage
		movss xmm1, dword ptr [rsp+30h]

	giveDamageToObject:
		mulss xmm0, xmm1
		movss xmm1, dword ptr [rdi+1FCh]
		subss xmm1, xmm0
		minss xmm1, dword ptr [rdi+1F8h]
		maxss xmm1, dword ptr [rdi+1F4h]
		movss dword ptr [rdi+1FCh], xmm1
	ofs2DAA61:
		jmp getInflictDamageFromDamageFuncRetAddress
		int 3

ASMgetInflictDamageFromDamageFunc ENDP

END
