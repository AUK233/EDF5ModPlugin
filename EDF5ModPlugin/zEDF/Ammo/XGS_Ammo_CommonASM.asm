include AmmoStructure.inc

.data

extern _call_CheckCollideABB20 : qword
extern _Common_F1P0 : dword
extern _CommonRVA_EE7790 : xmmword

.code

ASMcheckAmmoCollide1439E0 proc

		test r9b, r9b
		je ofs142EB2
		movaps xmm0, _CommonRVA_EE7790
		lea rax, [rsp+30h]
		lea rcx, [rbx+1D8h]
		cmp byte ptr [rbx+230h+ofs_AmmoBase_NoFriendCollision], -1
		je noUnitCollision
		; now it's not used.
		; cmp byte ptr [rbx+230h+ofs_AmmoBase_NoUnitCollision], 0
		; jne noUnitCollision
		mov dword ptr [rsp+28h], 0Ch
		mov dword ptr [rsp+28h+4], 0Bh
		jmp ofs142E96
	noUnitCollision:
		mov dword ptr [rsp+28h], 17h
		mov dword ptr [rsp+28h+4], 1Dh
	ofs142E96:
		mov r9, rdi
		movaps xmmword ptr [rsp+30h], xmm0
		mov r8, rsi
		mov [rsp+20h], rax
		lea rdx, [rbx+188h]
		call _call_CheckCollideABB20
	ofs142EB2:
		movss xmm0, dword ptr [rbx+3C8h]
		lea rax, [rsp+30h]
		movss dword ptr [rsp+30h], xmm0
		mov [rsp+20h], rax
		lea rcx, [rbx+1E8h]
		movss dword ptr [rsp+34h], xmm0
		lea rdx, [rbx+188h]
		movss dword ptr [rsp+38h], xmm0
		mov r9, rdi
		movss xmm0, _Common_F1P0
		mov eax, [rsp+28h+4]
		mov r8, rsi
		mov [rsp+28h], eax
		movss dword ptr [rsp+3Ch], xmm0
		call _call_CheckCollideABB20
		;
		mov rbx, [rsp+50h]
		mov rbp, [rsp+58h]
		mov rsi, [rsp+60h]
		add rsp, 40h
		pop rdi
		ret 

ASMcheckAmmoCollide1439E0 ENDP

END
