.data

extern module_WeaponFunction_SetDelayInt32 : proto

.code

ASMxgs_Weapon_RadioContactInit proc

		mov ecx, [r12+1204h]
		call module_WeaponFunction_SetDelayInt32
		mov [r12+1204h], eax
	ofs3A3D0C:
		mov rax, r12
		lea r11, [rsp+0F0h]
		mov rbx, [r11+38h]
		mov rsi, [r11+40h]
		mov rdi, [r11+48h]
		mov rsp, r11
		pop r15
		pop r14
		pop r13
		pop r12
		pop rbp
		ret 
		int 3

ASMxgs_Weapon_RadioContactInit ENDP

END