.data

extern rva27380 : qword
extern rva27570 : qword
extern rva12D890 : qword

extern displaySubtitleOn : dword
extern pSubtitleFile : qword

extern GetSubtitleTextAddress : proto
extern pSubtitleIndex : dword
extern edf5BDF30Address : qword
extern rva51960 : qword
extern vedfE918A8 : qword
extern _SubtitlePosition : dword
extern vedf125AB70 : qword
extern rva114980 : qword
extern rva51A80 : qword
extern rva61E560 : qword
extern _lyt_HuiSubtitleBox : word
;

extern script2C_FA0ret : qword
extern InitializeTalkSubtitleString : proto
extern PushTalkSubtitleString : proto
extern DisplayTalkSubtitleString : proto
extern script2C_1005ret : qword
extern script2C_1006ret : qword
extern script2C_1007ret : qword
extern script2C_1009ret : qword

.code

ASMshowSubtitleByPlayingSoundOff proc

		push rbx
		sub rsp, 20h
		; edx is input
		; rcx is input
		call rva12D890
		mov rbx, rax
		test rax, rax
		je ReturnAddress
		mov rdx, [rax]
		mov rcx, rax
		call qword ptr [rdx+38h]
		mov rdx, [rbx]
		mov rcx, rbx
		call qword ptr [rdx+40h]
	ReturnAddress:
		add rsp, 20h
		pop rbx
		ret
		int 3

ASMshowSubtitleByPlayingSoundOff ENDP

align 16

ASMshowSubtitleByPlayingSoundOn proc

		push rbp
		push rsi
		push rdi
		push r12
		push r13
		push r14
		push r15
		lea rbp, [rsp-0F0h]
		sub rsp, 1F0h
		mov qword ptr [rsp+40h], -2
		mov [rsp+240h], rbx
		; save register
		;mov [rbp+0E0h], r8
		mov r14, rdx
		mov r13, rcx
		xor r12d, r12d
		; rcx is input
	showSubtitle:
		; initialization
		lea rcx, [rbp-70h]
		call rva51960
		mov rax, vedfE918A8
		mov [rbp-70h], rax
		mov qword ptr [rbp+90h], 7
		mov [rbp+88h], r12
		mov [rbp+78h], r12w
		mov qword ptr [rbp+0B0h], 7
		mov [rbp+0A8h], r12
		mov [rbp+98h], r12w
		mov [rbp+0B8h], r12d
		mov [rbp+0C8h], r12
		mov [rbp+0D0h], r12b
		mov [rbp+0C4h], r12d
		mov [rbp-48h], r12d
		mov qword ptr [rbp-78h], 7
		mov [rbp-80h], r12
		mov [rsp+70h], r12w
		mov rsi, -1
		; end
		mov r8d, [r14+12] ; string length
		mov rdx, [r14]  ; string pointer
		lea rcx, [rbp+78h]
		call rva27380
		;
		mov dword ptr [rbp+0B8h], 18h
		mov eax, [r14+8] ; text hui pos x
		mov ecx, _SubtitlePosition+4
		mov rdx, vedf125AB70
		mov [rbp+0BCh], eax
		mov [rbp+0C0h], ecx
		mov rcx, [rdx]
		test rcx, rcx
		je ofs1268A6
		add rcx, -136
		jmp ofs1268A9
	ofs1268A6:
		mov rcx, r12
	ofs1268A9:
		mov dword ptr [rsp+20h], 0
		lea r9, [rbp-70h]
		lea r8, _lyt_HuiSubtitleBox
		lea rdx, [rsp+60h]
		call rva61E560
		;
		lea rax, [rsp+60h]
		mov rdx, [rax]
		mov [rsp+30h], rdx
		mov rcx, [rax+8]
		mov [rax], r12
		mov [rax+8], r12
		mov [rsp+48h], rcx
		mov r8, [r13+1F0h]
		mov [rsp+38h], r8
		mov [r13+1F0h], rcx
		mov [r13+1E8h], rdx
		test r8, r8
		je ofs126906
		mov eax, esi
		lock xadd [r8+12], eax
		cmp eax, 1
		jne ofs126906
		mov rcx, [rsp+38h]
		mov rax, [rcx]
		call qword ptr [rax+8]
	ofs126906:
		mov rcx, [rsp+68h]
		test rcx, rcx
		je ofs126922
		mov eax, esi
		lock xadd [rcx+12], eax
		cmp eax, 1
		jne ofs126922
		mov rax, [rcx]
		call qword ptr [rax+8]
	ofs126922:
		xorps xmm0, xmm0
		movaps [rsp+20h], xmm0
		mov rbx, [r13+1F0h]
		mov rdi, [r13+1E8h]
		test rbx, rbx
		je ofs12695C
		lock inc dword ptr [rbx+12]
		mov rcx, qword ptr [rsp+28h]
		test rcx, rcx
		je ofs12695C
		lock xadd [rcx+12], esi
		cmp esi, 1
		jne ofs12695C
		mov rax, [rcx]
		call qword ptr [rax+8]
	ofs12695C:
		mov [rsp+28h], rbx
		mov [rsp+20h], rdi
		lea rdx, [rsp+20h]
		mov rcx, r13
		call rva114980
		mov pSubtitleIndex, eax
		;
		mov rdx, [rbp+90h]
		cmp rdx, 8
		jb ofs1269FF
		inc rdx
		mov r8d, 2
		mov rcx, [rbp+78h]
		call rva27570
	ofs1269FF:
		mov qword ptr [rbp+90h], 7
		mov [rbp+88h], r12
		mov [rbp+78h], r12w
		lea rcx, [rbp-70h]
		call rva51A80
	ReturnAddress:
		;mov rax, [rbp+0E0h]
		mov rbx, [rsp+240h]
		add rsp, 1F0h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret
		int 3

ASMshowSubtitleByPlayingSoundOn ENDP

align 16

ASMshowSubtitleByPlayingSoundReady proc

		push rbx
		push rsi
		push rdi
		sub rsp, 30h
		; save register
		mov rbx, r8
		mov rsi, rcx
		mov rdi, rdx
	; check subtitles exist
		mov edx, pSubtitleIndex
		test edx, edx
		je getSubtitleText
		; rcx is input
		call ASMshowSubtitleByPlayingSoundOff
		mov pSubtitleIndex, 0
	getSubtitleText:
		; check subtitle display
		cmp displaySubtitleOn, 0
		je ReturnAddress
		cmp pSubtitleFile, 0
		je ReturnAddress
		; end
		xor r9d, r9d
		mov r8d, ebx
		mov rdx, rdi
		lea rcx, [rsp+20h]
		call GetSubtitleTextAddress
		test eax, eax
		je ReturnAddress
		;
		lea rdx, [rsp+20h]
		mov rcx, rsi
		call ASMshowSubtitleByPlayingSoundOn
	ReturnAddress:
		mov rax, rbx
		add rsp, 30h
		pop rdi
		pop rsi
		pop rbx
		ret

ASMshowSubtitleByPlayingSoundReady ENDP

align 16

ASMscript2C_FA0 proc

		mov qword ptr [rsp+40h], 7
		xor r8d,r8d
		mov [rsp+38h], r8
		mov [rsp+28], r8w
		cmp [rbx], r8w
		je ofs1270EC
		or r8, -1
		align 16
	ofs1270E1:
		inc r8
		cmp word ptr [rbx+r8*2], 0
		jne ofs1270E1
	ofs1270EC:
		mov rdx, rbx
		mov rcx, rdi
		call ASMshowSubtitleByPlayingSoundReady
	;original
		mov r8, rax
		mov rdx, rbx
		jmp script2C_FA0ret
		int 3

ASMscript2C_FA0 ENDP

align 16

ASMscript2C_FA2 proc

		test al, al
		je clearSubtitle
		mov rbx, [rsi+40h]
		mov byte ptr [rbx+68h], 1
	clearSubtitle:
		xor r12d, r12d
		cmp [rbx+68h], r12b
		jne ofs116A83
		mov edx, pSubtitleIndex
		test edx, edx
		je ofs116A83
		mov rcx, rsi
		call ASMshowSubtitleByPlayingSoundOff
		mov pSubtitleIndex, r12d
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

ASMscript2C_FA2 ENDP

align 16

ASMscript2C_1004 proc

		add rsp, 0C0h
		pop r15
		pop r14
		pop rdi
		pop rsi
		pop rbp
		jmp InitializeTalkSubtitleString
		int 3

ASMscript2C_1004 ENDP

align 16

ASMscript2C_1005 proc

		mov rcx, rdi
		call DisplayTalkSubtitleString
	ReturnAddress:
		mov [rbp+2Fh], rsi
		mov [rbp+37h], rsi
		mov [rbp+3Fh], rsi
		xor r8d, r8d
		jmp script2C_1005ret
		int 3

ASMscript2C_1005 ENDP

align 16

ASMscript2C_1006 proc

		;mov r8, rbx
		;mov rdx, rsi
		;mov rcx, rbp
		;call ASMshowSubtitleByPlayingSoundReady
		
		; check subtitle display
		cmp displaySubtitleOn, 0
		je ReturnAddress
		cmp pSubtitleFile, 0
		je ReturnAddress
		; end
		mov rdx, rbx
		mov rcx, rsi
		call PushTalkSubtitleString
	ReturnAddress:
		mov rax, script2C_1006ret
		mov r8, rbx
		mov rdx, rsi
		lea rcx, [rsp+38h]
		push rax
		jmp rva27380
		int 3

ASMscript2C_1006 ENDP

align 16

ASMscript2C_1007 proc

		;mov r8, rbx
		;mov rdx, rdi
		;mov rcx, rsi
		;call ASMshowSubtitleByPlayingSoundReady

		; check subtitle display
		cmp displaySubtitleOn, 0
		je ReturnAddress
		cmp pSubtitleFile, 0
		je ReturnAddress
		; end
		mov rdx, rbx
		mov rcx, rdi
		call PushTalkSubtitleString
	ReturnAddress:
		mov rax, script2C_1007ret
		mov r8, rbx
		mov rdx, rdi
		lea rcx, [rsp+50h]
		push rax
		jmp rva27380
		int 3

ASMscript2C_1007 ENDP

align 16

ASMscript2C_1009 proc

		;mov r8, rbx
		;mov rdx, rsi
		;mov rcx, rbp
		;call ASMshowSubtitleByPlayingSoundReady

		; check subtitle display
		cmp displaySubtitleOn, 0
		je ReturnAddress
		cmp pSubtitleFile, 0
		je ReturnAddress
		; end
		mov rdx, rbx
		mov rcx, rsi
		call PushTalkSubtitleString
	ReturnAddress:
		mov rax, script2C_1009ret
		mov r8, rbx
		mov rdx, rsi
		lea rcx, [rsp+48h]
		push rax
		jmp rva27380
		int 3

ASMscript2C_1009 ENDP

END