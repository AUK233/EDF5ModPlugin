.data

extern pOutRenderColor1259640 : qword
extern pGameRenderer1259680 : qword

.code

Get_g_xgl_draw_utility_color_textrure proc

		mov rdx, pOutRenderColor1259640
		mov rcx, [rdx]
		mov rax, [rcx+10h] ; maybe is pGameRenderer1259680
		mov rdx, [rax+560h]
		mov rax, [rcx+0F98h]
		mov rcx, [rdx+0C8h]
		lea r9, [rax+rax*2]
		mov rdx, [rcx+r9*8+10h]
		test rdx, rdx
		jz ReturnZero
		mov rcx, [rdx+10h]
		test rcx, rcx
		jz ReturnZero
		mov rax, [rcx+18h]
		ret
	ReturnZero:
		xor eax, eax
		ret
		int 3

Get_g_xgl_draw_utility_color_textrure ENDP

align 16

Get_g_xgl_viewXYZ_id_texture proc

		mov rdx, pGameRenderer1259680
		mov rcx, [rdx]
		mov rdx, [rcx+560h]
		mov rax, [rcx+5E8h]
		mov rcx, [rdx+0C8h]
		lea r9, [rax+rax*2]
		mov rdx, [rcx+r9*8+10h]
		test rdx, rdx
		jz ReturnZero
		mov rcx, [rdx+10h]
		test rcx, rcx
		jz ReturnZero
		mov rax, [rcx+18h]
		ret
	ReturnZero:
		xor eax, eax
		ret
		int 3

Get_g_xgl_viewXYZ_id_texture ENDP

END