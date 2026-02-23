.data

.code

DXGI_GetUmbraSystem125B080ASM proc

		mov rdx, [rcx]
		test rdx, rdx
		jz Return0
		mov rcx, [rdx+8]
		test rcx, rcx
		jz Return0
		mov rdx, [rcx+8]
		test rdx, rdx
		jz Return0
		mov rax, [rdx+8]
		ret
	Return0:
		xor eax, eax
		ret

DXGI_GetUmbraSystem125B080ASM ENDP

END
