.data

extern edf5BDF30Address : qword
extern _str_GameObjectDestroyFadeOutTime : word
extern _str_GameObjectDestroySpawnObject : word

extern BigNestDeathAnimationPlayAddr : qword

extern edf617B30Address : qword

extern rvaE8E3F8 : qword
extern vedf125AB68 : qword
extern edf12D130Address : qword

.code

ASMxgsOCBigNestInitialization proc

	; initialize
		xor r15d, r15d
		mov dword ptr [rbx+0CE0h], 42B40000h ; death fade time, 90.0f
		mov dword ptr [rbx+0CE4h], r15d
		mov qword ptr [rbx+0CE8h], r15 ; string pointer

	; game_object_destroy_fadeout_time
		lea rdx, _str_GameObjectDestroyFadeOutTime
		mov rcx, r14
		call edf5BDF30Address
		movsxd rcx, eax
		cmp ecx, -1
		je DestroySpawnObject
		mov rax, [r14]
		movsxd rdx, dword ptr [rax+12]
		add rdx, rax
		lea rcx, [rcx+rcx*2]
		mov eax, [rdx+rcx*4+8]
		mov [rbx+0CE0h], eax

	DestroySpawnObject:
		; game_object_destroy_spawn_object
		lea rdx, _str_GameObjectDestroySpawnObject
		mov rcx, r14
		call edf5BDF30Address
		movsxd rcx, eax
		cmp ecx, -1
		je ofs29981A
		mov rax, [r14]
		movsxd rdx, dword ptr [rax+12]
		add rdx, rax
		lea rcx, [rcx+rcx*2]
		lea rsi, [rdx+rcx*4]
		movsxd rax, dword ptr [rsi+8]
		add rsi, rax
		mov [rbx+0CE8h], rsi

	ofs29981A:
		mov rax, rbx
		mov rbx, [rsp+290h]
		add rsp, 240h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret 

ASMxgsOCBigNestInitialization ENDP

align 16

ASMBigNestDamageAction proc

		cmp [rbx+0CE8h], rcx
		je Original
		lea rax, ASMBigNestDestroySpawnObjectAction
	Original:
		mov [rsp+38h], ecx
		lea rcx, [rbx+0A00h]
		cmp qword ptr [rcx+28h], 0
		;movaps xmm0, [rsp+30h]
		;movdqa [rsp+30h], xmm0
		je ofs29AE04
		mov [rsp+30h], rax
		lea rdx, [rsp+30h]
		call BigNestDeathAnimationPlayAddr
	ofs29AE04:
		mov rbx, [rsp+50h]
		add rsp, 40h
		pop rdi
		ret 

ASMBigNestDamageAction ENDP

align 16

ASMBigNestDestroySpawnObjectAction proc

		push rdi
		sub rsp, 90h
		mov rdi, rcx
		; start
		mov rax, rvaE8E3F8
		mov [rsp+68h], rax
		xor edx, edx
		mov [rsp+88h], rdx
		lea rax, [rsp+68h]
		mov [rsp+20h], rax
		mov r9, [rcx+0CE8h]
		lea r8, [rcx+70h]
		mov rax, vedf125AB68
		lea rdx, [rsp+48h]
		mov rcx, [rax]
		call edf12D130Address
		;
		mov rcx, [rsp+50h]
		test rcx, rcx
		je ofs1F956C
		or eax, -1
		lock xadd [rcx+12], eax
		cmp eax, 1
		jne ofs1F956C
		mov rax, [rcx]
		call qword ptr [rax+8]
	ofs1F956C:
		; end
		mov rcx, rdi
		call edf617B30Address
		add rsp, 90h
		pop rdi
		ret 

ASMBigNestDestroySpawnObjectAction ENDP

END