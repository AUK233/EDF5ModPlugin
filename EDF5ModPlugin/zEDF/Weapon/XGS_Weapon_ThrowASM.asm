.data


extern ASMrva3B4F40mod: proto
extern _Common_F5P0 : dword
extern _CommonRVA_EE7460 : xmmword
extern rva3B5460 : qword

extern vedf125AB68 : qword
extern xgs_Weapon_Throw_SetIndicatorPos: proto
extern rva3B67C0 : qword
extern xgs_Weapon_ThrowFuncP0Ret : qword

extern rva181DA0 : qword
extern xgs_Weapon_ThrowFuncFreeRet : qword

extern jmp_Umbra_Object_set : qword

extern Weapon_Throw_VFT : qword
extern characterGhostCameraSetIndicatorLineRetAddr : qword

.code

ASMxgs_Weapon_ThrowInit proc

		sub rsp, 20h
	; indicator circle init
		lea rcx, [rbx+1200h]
		call ASMrva3B4F40mod

		cmp dword ptr [rbx+6A8h], 0
		jne GetExplosionRadius
		movss xmm3, _Common_F5P0
		jmp SetCircleRadius
	GetExplosionRadius:
		movss xmm3, dword ptr [rbx+6A8h]
		movaps xmm0, xmm3
		mulss xmm0, dword ptr [rbx+308h] ; FireAccuracy
		addss xmm3, xmm0
		
	SetCircleRadius:
		movaps xmm0, _CommonRVA_EE7460
		movaps [rsp+30h], xmm0
		movaps [rsp+40h], xmm0
		mov byte ptr [rsp+20h], 1 ; no horizontal
		lea r8, [rsp+30h]
		lea rdx, [rsp+40h]
		lea rcx, [rbx+1200h]
		call rva3B5460

	ofs3A810F:
		mov rax, rbx
		mov rbx, [rsp+68h+20h]
		mov rbp, [rsp+70h+20h]
		mov rsi, [rsp+78h+20h]
		add rsp, 40h+20h
		pop r15
		pop r14
		pop rdi
		ret 
		int 3

ASMxgs_Weapon_ThrowInit ENDP

align 16

ASMxgs_Weapon_ThrowFuncP0 proc

		push rbx
		sub rsp, 20h
		mov rbx, rcx

	indicatorDraw:
		xor r8d, r8d
		cmp dword ptr [rcx+1B4h], 0
		je indicatorDrawCall
		mov rdx, vedf125AB68
		mov rax, [rdx]
		mov rdx, [rax+18h]
		call xgs_Weapon_Throw_SetIndicatorPos
		mov r8d, eax
	indicatorDrawCall:
		xor edx, edx
		mov rcx, [rbx+1200h+10h]
		call jmp_Umbra_Object_set
		;
		lea rcx, [rbx+1200h]
		call rva3B67C0

	ofs398875:
		cmp qword ptr [rbx+0C60h], 0
		je ofs38F3A6
		lea rcx, [rbx+0C50h]
		add rsp, 20h
		pop rbx
		jmp xgs_Weapon_ThrowFuncP0Ret
	ofs38F3A6:
		add rsp, 20h
		pop rbx
		ret 
		int 3

ASMxgs_Weapon_ThrowFuncP0 ENDP

align 16

ASMxgs_Weapon_ThrowFuncFree proc

		mov [rsp+8], rbx
		push rdi
		sub rsp, 30h
		mov [rsp+28h], rdx
		mov [rsp+20h], rcx

		lea rcx, [rcx+1200h]
		call rva181DA0

	ofs3A8167:
		mov rdx, [rsp+28h]
		mov rcx, [rsp+20h]
		add rsp, 10h
		jmp xgs_Weapon_ThrowFuncFreeRet
		int 3

ASMxgs_Weapon_ThrowFuncFree ENDP

align 16

ASMcharacterGhostCameraSetIndicatorLine proc

		mov rax, [rbx]
		cmp rax, Weapon_Throw_VFT
		jne ofs9E345
		xor r8d, r8d
		cmp byte ptr [rdi+4A4h], 0
		je indicatorDrawCall
		movaps xmm0, [rdi+4B0h]
		movaps [rbx+1200h+0B0h], xmm0
		movzx r8d, byte ptr [rdi+4A4h]
	indicatorDrawCall:
		xor edx, edx
		mov rcx, [rbx+1200h+10h]
		call jmp_Umbra_Object_set

	ofs9E345:
		lea r8, [rdi+5E8h]
		mov [r8+20h], r15
		mov rax, [rbp-70h]
		jmp characterGhostCameraSetIndicatorLineRetAddr
		int 3

ASMcharacterGhostCameraSetIndicatorLine ENDP

END