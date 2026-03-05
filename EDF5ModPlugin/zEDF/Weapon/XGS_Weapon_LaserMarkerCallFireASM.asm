.data

extern module_WeaponFunction_SetDelayInt32 : proto

.code

ASMxgs_Weapon_LaserMarkerCallFireInit proc

		mov ecx, [rdi+1820h]
		call module_WeaponFunction_SetDelayInt32
		mov [rdi+1820h], eax
		mov [rdi+1824h], eax
	ofs3A3D0C:
		mov rax, rdi
		mov rbx, [rsp+0E0h]
		add rsp, 90h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret 
		int 3

ASMxgs_Weapon_LaserMarkerCallFireInit ENDP

END