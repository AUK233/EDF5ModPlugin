.data

extern edf5BDF30Address : qword
extern _str_RagdollBitmark : word
extern edf3C82F0Address : qword

.code

ASMxgsOCGeneratorPollDestroyRagdollInit proc

	; ragdoll_bitmark
		lea rdx, _str_RagdollBitmark
		mov rcx, rsi
		call edf5BDF30Address
		movsxd rcx, eax
		cmp ecx, -1
		jne setNewBitmark
		mov edx, 1Eh
		jmp ofs1FAF49
	setNewBitmark:
		mov rax, [rsi]
		movsxd rdx, dword ptr [rax+12]
		add rax, rdx
		lea rcx, [rcx+rcx*2]
		mov edx, [rax+rcx*4+8]

	ofs1FAF49:
		lea rcx, [rdi+100h]
		call edf3C82F0Address

	ofs1FAF56:
		mov rax, rdi
		mov rbx, [rsp+1A0h]
		add rsp, 150h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret 

ASMxgsOCGeneratorPollDestroyRagdollInit ENDP

END