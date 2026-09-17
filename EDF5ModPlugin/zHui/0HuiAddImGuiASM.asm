.data

extern module_InitializeD3D11 : proto
extern dx11CreateDeviceRetAddr : qword

extern DLSS_CreateFeature : proto
extern vedf125AB30 : qword

extern DLSS_Release : proto

extern DLSS_Draw : proto
extern RenderBufferToScreenBufferRetAddr : qword

extern streamline_SwapChainGetBuffer : proto
extern Call_IDXGISwapChain_GetBufferRetAddr : qword

extern togui_Main : proto
extern DLSS_FG_Evaluate : proto
extern streamline_SwapChainPresent : proto
extern Call_IDXGISwapChain_PresentRetAddr : qword

extern streamline_MapLoadResources : proto
extern map_async_obj_initRetAddr : qword

extern streamline_CreateSwapChain : proto
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

ASMgetPlayerCountInHQ proc

	mov rdx, [vedf125AB30]
	mov rax, [rdx]
	mov rcx, [rax+2459Ch]
	call DLSS_CreateFeature
	mov rax, rsi
	mov rbx, [rsp+2B0h]
	add rsp, 260h
	pop r15
	pop r14
	pop r13
	pop r12
	pop rdi
	pop rsi
	pop rbp
	ret
	int 3

ASMgetPlayerCountInHQ ENDP

align 16

ASMCall_IDXGISwapChain_GetBuffer proc

		lea r8, [rsp+70h]
		mov rdx, rsi
		mov rcx, rdi
		call streamline_SwapChainGetBuffer
		test eax, eax
		js ofs5C66D7
		jmp Call_IDXGISwapChain_GetBufferRetAddr
	ofs5C66D7:
		xor al, al
		add rsp, 50h
		pop rdi
		pop rsi
		pop rbx
		ret 
		int 3

ASMCall_IDXGISwapChain_GetBuffer ENDP

align 16

ASMCall_IDXGISwapChain_Present proc

	mov rdi, [rsp+20h]
	;
	call togui_Main
	;
	; xor r8d, r8d
	; mov edx, [rsp+28h]
	; mov rcx, [rdi+0C8h]
	; call DLSS_FG_Evaluate
	mov rcx, rdi
	call streamline_SwapChainPresent
	;
	xor r8d, r8d
	mov edx, [rsp+28h]
	mov rcx, [rdi+0C8h]
	mov rax, [rcx]
	call qword ptr [rax+40h]
	jmp Call_IDXGISwapChain_PresentRetAddr
	int 3

ASMCall_IDXGISwapChain_Present ENDP

align 16

ASMmap_async_obj_init proc

	; lea r9, [rbp+0B0h]
	; mov r8, [r14+40h]
	; lea rcx, [rsi+1090h]
	; call streamline_MapLoadResources
	mov rcx, rsi
	call streamline_MapLoadResources
	movzx ebx, al
	lea rdx, [rsi+1C8h]
	jmp map_async_obj_initRetAddr
	int 3

ASMmap_async_obj_init ENDP

align 16

ASMGetDXGISwapChain proc

		mov r8, [rdi+0C8h]
		lea rdx, [rsp+30h]
		lea rcx, [rsp+40h]
		call streamline_CreateSwapChain
		mov [rdi+0C8h], rbp
		test al, al
		jnz ReturnOriginal
	ofs5E0F69:
		mov rcx, [rdi+0D8h] ; IDXGIFactory*
		lea r9, [rsp+30h] ; IDXGISwapChain**
		mov rdx, [rdi+0B8h] ; ID3D11Device*
		lea r8, [rsp+40h] ; DXGI_SWAP_CHAIN_DESC*
		mov [rsp+30h], rbp
		mov rax, [rcx]
		call qword ptr [rax+50h]
		test eax, eax
		js ofs5E0FF6
	ReturnOriginal:
		mov rbx, [rsp+30h]
		;mov rcx, rbx
		;call togui_GetDXGISwapChain
		mov eax, [rsp+40h]
		jmp GetDXGISwapChainRetAddr
		int 3
	ofs5E0FF6:
		mov rbx, [rsp+0D0h]
		add rsp, 0A0h
		pop r14
		pop rdi
		pop rbp
		ret 
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

	mov rax, [rbx]
	add dword ptr [rbx+8], -8
	add dword ptr [rbx+12], 8
	movsxd rcx, dword ptr [rbx+10h]
	shl rcx, 4
	add rcx, [rax+28h]
	lea r9, [rcx+60h]
	lea rax, [rcx+70h]
	mov [rsp+28h], rax
	mov [rsp+20h], r9
	;
	mov r8, rcx
	mov rdx, [r9]
	mov rcx, rdi
	call DLSS_Draw
	;
	lea r9, [rax+50h]
	lea r8, [rax+10h]
	mov rcx, [rax]
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
