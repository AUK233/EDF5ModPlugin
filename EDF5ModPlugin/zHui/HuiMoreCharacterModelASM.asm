.data

extern module_HuiPushChaModelSelButton : proto

extern vft_ModelIndexVeteran : qword
extern vft_ModelIndexElite : qword
extern vft_ModelIndexPioneer : qword

extern vedf125AB30 : qword
extern vedf125ABD0 : qword
extern edf4BE440Address : qword

extern HuiMoreChaModelSelButtonRetAddr : qword

.code

ASMHuiBoxModelIndexVeteranFuncP0 proc

		test rdx, rdx
		je ofs5309C7
		lea rax, vft_ModelIndexVeteran+8
		mov [rdx], rax
		mov rax, [rcx+8]
		mov [rdx+8], rax
	ofs5309C7:
		mov rax, rdx
		ret 
        int 3

ASMHuiBoxModelIndexVeteranFuncP0 ENDP

align 16

ASMHuiBoxModelIndexVeteranFuncP10 proc

		push rbx
		sub rsp, 20h
		mov rbx, [rdx]
		mov rdx, [rcx+8]
		mov eax, 2
		cmp [rdx+790h], eax
		je ofs530996
		mov [rdx+790h], eax
		;
		mov eax, [rdx+818h]
		mov [rdx+814h], eax
		mov byte ptr [rdx+808h], 0
		;
		movsxd rax, dword ptr [rdx+788h]
		mov rcx, vedf125AB30
		mov edx, [rdx+790h]
		imul r8, rax, 3E60h
		mov rax, [rcx]
		mov [r8+rax+6D50h], edx
	ofs530996:
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+38h]
		mov rax, [rbx]
		mov rcx, rbx
		add rsp, 20h
		pop rbx
		jmp qword ptr [rax+40h]
        int 3

ASMHuiBoxModelIndexVeteranFuncP10 ENDP

align 16

ASMHuiBoxModelIndexEliteFuncP0 proc

		test rdx, rdx
		je ofs5309C7
		lea rax, vft_ModelIndexElite+8
		mov [rdx], rax
		mov rax, [rcx+8]
		mov [rdx+8], rax
	ofs5309C7:
		mov rax, rdx
		ret 
        int 3

ASMHuiBoxModelIndexEliteFuncP0 ENDP

align 16

ASMHuiBoxModelIndexEliteFuncP10 proc

		push rbx
		sub rsp, 20h
		mov rbx, [rdx]
		mov rdx, [rcx+8]
		mov eax, 3
		cmp [rdx+790h], eax
		je ofs530996
		mov [rdx+790h], eax
		;
		mov eax, [rdx+818h]
		mov [rdx+814h], eax
		mov byte ptr [rdx+808h], 0
		;
		movsxd rax, dword ptr [rdx+788h]
		mov rcx, vedf125AB30
		mov edx, [rdx+790h]
		imul r8, rax, 3E60h
		mov rax, [rcx]
		mov [r8+rax+6D50h], edx
	ofs530996:
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+38h]
		mov rax, [rbx]
		mov rcx, rbx
		add rsp, 20h
		pop rbx
		jmp qword ptr [rax+40h]
        int 3

ASMHuiBoxModelIndexEliteFuncP10 ENDP

align 16

ASMHuiBoxModelIndexPioneerFuncP0 proc

		test rdx, rdx
		je ofs5309C7
		lea rax, vft_ModelIndexPioneer+8
		mov [rdx], rax
		mov rax, [rcx+8]
		mov [rdx+8], rax
	ofs5309C7:
		mov rax, rdx
		ret 
        int 3

ASMHuiBoxModelIndexPioneerFuncP0 ENDP

align 16

ASMHuiBoxModelIndexPioneerFuncP10 proc

		push rbx
		sub rsp, 20h
		mov rbx, [rdx]
		mov rdx, [rcx+8]
		mov eax, 4
		cmp [rdx+790h], eax
		je ofs530996
		mov [rdx+790h], eax
		;
		mov eax, [rdx+818h]
		mov [rdx+814h], eax
		mov byte ptr [rdx+808h], 0
		;
		movsxd rax, dword ptr [rdx+788h]
		mov rcx, vedf125AB30
		mov edx, [rdx+790h]
		imul r8, rax, 3E60h
		mov rax, [rcx]
		mov [r8+rax+6D50h], edx
	ofs530996:
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+38h]
		mov rax, [rbx]
		mov rcx, rbx
		add rsp, 20h
		pop rbx
		jmp qword ptr [rax+40h]
        int 3

ASMHuiBoxModelIndexPioneerFuncP10 ENDP

align 16

ASMHuiSetCharacterModelButton proc

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

ASMHuiSetCharacterModelButton ENDP

align 16

ASMHuiMoreChaModelSelButton proc

		mov r8, rsi
		mov rax, vedf125ABD0
		lea rdx, [rbp+3E0h]
		mov rcx, [rax]
		call module_HuiPushChaModelSelButton
		; original
        xor eax, eax
		mov ecx, [rsi+790h] ; model index
		mov ebx, 1
		;cmp ecx, -1
		;je ofs52D4B5
		test ecx, ecx
		jz isSoldier
		cmp ecx, ebx
		je isCivilian
		cmp ecx, 4
		ja ofs52D4B5
		mov eax, ecx
		inc eax
        jmp HuiMoreChaModelSelButtonRetAddr
	isCivilian:
		mov eax, ebx
        jmp HuiMoreChaModelSelButtonRetAddr
	isSoldier:
		mov eax, 2
	ofs52D4B5:
        jmp HuiMoreChaModelSelButtonRetAddr
        int 3

ASMHuiMoreChaModelSelButton ENDP

END