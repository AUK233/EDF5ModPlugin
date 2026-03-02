.data

extern SubtitleRenderer_GetCurrentSubtitle : proto
extern script2C_FA0ret : qword
extern SubtitleRenderer_ClearCurrentSubtitle : proto

extern addTalkToList_ret : qword
extern setPlayTalk_ret : qword

.code

; play voice
ASMscript2C_FA0 proc

		mov qword ptr [rsp+40h], 7
		xor r8d,r8d
		mov [rsp+38h], r8
		mov [rsp+28], r8w
		cmp [rbx], r8w
		je ofs1270EC
		or rdx, -1
		align 16
	ofs1270E1:
		inc rdx
		cmp word ptr [rbx+rdx*2], 0
		jne ofs1270E1
	ofs1270EC:
		mov rcx, rbx
		call SubtitleRenderer_GetCurrentSubtitle
	;original
		mov r8, rax
		mov rdx, rbx
		jmp script2C_FA0ret
		int 3

ASMscript2C_FA0 ENDP

align 16

; wait voice
ASMscript2C_FA2 proc

		test al, al
		je clearSubtitle
		mov rbx, [rsi+40h]
		mov byte ptr [rbx+68h], 1
	clearSubtitle:
		xor r12d, r12d
		cmp [rbx+68h], r12b
		jne ofs116A83
		call SubtitleRenderer_ClearCurrentSubtitle
	ofs116A83:    
		cmp [r14+0A0h], r12b
		jne ofs116ABD
		mov [r14+98h], rdi
	ofs116ABD:
		mov [r14+0A0h], r12b
		movaps xmm6, [rsp+0F0h]
		lea r11, [rsp+100h]
		mov rbx, [r11+30h]
		mov rsi, [r11+38h]
		mov rdi, [r11+40h]
		mov rsp, r11
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret 
		int 3

ASMscript2C_FA2 ENDP

align 16

ASMaddTalkToList proc

		mov dword ptr [rbp+13h], 100h
		mov eax, [r15+20h]
		mov [rsp+40h], eax
		lea rsi, [r15+48h]
		jmp addTalkToList_ret
		int 3

ASMaddTalkToList ENDP

align 16

ASMsetPlayTalk proc
		
		mov rbx, rcx
		mov byte ptr [rcx+6Ch], 1
		; new
		cmp byte ptr [rbx+6Ch+1], 1
		jne ofs326E32
		mov byte ptr [rbx+6Ch+1], 0
		mov r8, [rbx+28h]
		mov rdx, [rbx+20h]
		lea rcx, [rbx+10h]
		cmp r8, 8
		jb callFunc
		mov rcx, [rcx]
	callFunc:
		call SubtitleRenderer_GetCurrentSubtitle

	ofs326E32:
		lea rcx, [rbx+50h]
		lea rdx, [rsp+28h]
		jmp setPlayTalk_ret
		int 3

ASMsetPlayTalk ENDP

END