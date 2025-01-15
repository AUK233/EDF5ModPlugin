.data

extern testFadadad : proto

extern vft_ModelIndexCivilian : qword
extern vft_ModelIndexSoldier : qword
extern vedf125ABD0 : qword
extern edf5BDF30Address : qword
extern rva27380 : qword
extern edf73240Address : qword
extern edf4738B0Address : qword
extern edf4BE440Address : qword
extern moreClassModelSelectRetAddr : qword

extern moreClassModelSelect1RetAddr : qword

; L"ModelIndexCivilian"
wstrModelIndexCivilian db 4Dh, 00h, 6Fh, 00h, 64h, 00h, 65h, 00h, 6Ch, 00h, 49h, 00h, 6Eh, 00h, 64h, 00h, 65h, 00h, 78h, 00h, 43h, 00h, 69h, 00h, 76h, 00h, 69h, 00h, 6Ch, 00h, 69h, 00h, 61h, 00h, 6Eh, 00h, 0, 0
align 16
; L"ModelIndexSoldier"
wstrModelIndexSoldier db 4Dh, 00h, 6Fh, 00h, 64h, 00h, 65h, 00h, 6Ch, 00h, 49h, 00h, 6Eh, 00h, 64h, 00h, 65h, 00h, 78h, 00h, 53h, 00h, 6Fh, 00h, 6Ch, 00h, 64h, 00h, 69h, 00h, 65h, 00h, 72h, 00h, 0, 0
align 16
; L"ModelIndexChgDlgMsg"
wstrModelIndexChgDlgMsg db 77,0,111,0,100,0,101,0,108,0,73,0,110,0,100,0,101,0,120,0,67,0,104,0,103,0,68,0,108,0,103,0,77,0,115,0,103,0,0,0

.code

ASMmoreClassModelSelect proc

		; r13 is 0
		; r14 is -1
		; func maybe is 0x40
	ModelIndexCivilian:
		mov rax, vft_ModelIndexSoldier
		;mov rax, vft_ModelIndexCivilian
		mov [rbp+60h], rax
		mov [rbp+68h], rsi
		lea rax, [rbp+60h]
		mov [rbp+98h], rax
		;
		mov rdi, vedf125ABD0
		mov rdi, [rdi]
		mov rbx, [rdi+130h]
		mov rax, rbx
		mov rbx, [rbx]
		mov [rsp+30h], rax
		cmp rbx, rax
		je ofs52D31C
		mov rdi, rax
		mov [rsp+30h], rax
		align 16
	ofs52D2E0:
		lea rcx, [rbx+38h]
		lea rdx, wstrModelIndexChgDlgMsg
		call edf5BDF30Address
		movsxd rcx, eax
		cmp ecx, -1
		jne ofs52D302
		mov rbx, [rbx]
		cmp rbx, rdi
		jne ofs52D2E0
		jmp ofs52D31C
	ofs52D302:
		mov rax, [rbx+38h]
		movsxd rdx, dword ptr [rax+0Ch]
		add rdx, rax
		lea rcx, [rcx+rcx*2]
		lea rax, [rdx+rcx*4]
		movsxd r15, dword ptr [rax+8]
		add r15, rax
	ofs52D31C:
		mov qword ptr [rbp+188h], 7
		mov [rbp+180h], r13
		mov [rbp+170h], r13w
		cmp word ptr [r15], 0
		jne ofs52D342
		mov r8, r13
		jmp ofs52D350
	ofs52D342:
		mov r8, r14
		align 16
	ofs52D345:
		inc r8
		cmp word ptr [r15+r8*2], 0
		jne ofs52D345
	ofs52D350:
		mov rdx, r15
		lea rcx, [rbp+170h]
		call rva27380
		lea rax, [rbp+190h]
		mov [rsp+38h], rax
		mov [rbp+1C8h], r13
		;
		mov rcx, [rbp+98h]
		test rcx, rcx
		je ofs52D392
		mov rax, [rcx]
		lea rdx, [rbp+190h]
		call qword ptr [rax]
		mov [rbp+1C8h], rax
	ofs52D392:
		mov word ptr [rbp+1D0h], 1
		;mov [rbp+1F0h], r13
		mov [rbp+1F8h], r13
		call edf73240Address
		mov [rbp+1F0h], rax
		;
		mov rcx, [rbp+98h]
		test rcx, rcx
		je ModelIndexSoldier
		mov rax, [rcx]
		lea rdx, [rbp+60h]
		cmp rcx, rdx
		setne dl
		call qword ptr [rax+20h]
		mov [rbp+98h], r13

	ModelIndexSoldier:
		mov rax, vft_ModelIndexSoldier
		mov [rbp+0A0h], rax
		mov [rbp+0A8h], rsi
		lea rax, [rbp+0A0h]
		mov [rbp+0D8h], rax
		lea rdx, wstrModelIndexSoldier
		mov rax, vedf125ABD0
		mov rcx, [rax]
		call edf4738B0Address
		mov rdx, rax
		lea r8, [rbp+0A0h]
		lea rcx, [rbp+200h]
		call edf4BE440Address
		;
		mov rcx, [rbp+0D8h]
		test rcx, rcx
		je ofs52D44B
		mov rax, [rcx]
		lea rdx, [rbp+0A0h]
		cmp rcx, rdx
		setne dl
		call qword ptr [rax+20h]
		mov [rbp+0D8h], r13

	ofs52D44B:
		lea rbx, [rbp+0E0h]
		jmp moreClassModelSelectRetAddr
		int 3

ASMmoreClassModelSelect ENDP

align 16

ASMdwwfafaffa proc

		push rsi
		sub rsp, 20h
		mov rsi, r8
		; rcx, rdx, r8 is input
		call edf4BE440Address
		;
		mov rcx, [rsi+38h]
		test rcx, rcx
		je retFunc
		mov rax, [rcx]
		cmp rcx, rsi
		setne dl
		call qword ptr [rax+20h]
	retFunc:
		xor eax, eax
		mov [rsi+38h], rax
		add rsp, 20h
		pop rsi
		ret
        int 3

ASMdwwfafaffa ENDP

align 16

ASMmoreClassModelSelect1 proc

		mov r8, rsi
		mov rax, vedf125ABD0
		lea rdx, [rbp+3E0h]
		mov rcx, [rax]
		call testFadadad
		; original
        xor eax, eax
		mov ecx, [rsi+790h] ; model index
		mov ebx, 1
		cmp ecx, -1
        jmp moreClassModelSelect1RetAddr
        int 3

ASMmoreClassModelSelect1 ENDP

END