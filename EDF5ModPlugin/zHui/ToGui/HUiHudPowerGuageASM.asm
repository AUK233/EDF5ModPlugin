.data

extern rva27380 : qword
extern rva27570 : qword
extern rva4D86D0 : qword
extern readHUiHudPowerGuageRet : qword

extern module_HUiHudPowerGuageFuncP10_add : proto
extern rva26EE0 : qword

; L"TextFencerDash"
wstrTextFencerDash db 84,0,101,0,120,0,116,0,70,0,101,0,110,0,99,0,101,0,114,0,68,0,97,0,115,0,104,0,0,0
align 16
; L"TextFencerBoost"
wstrTextFencerBoost db 84,0,101,0,120,0,116,0,70,0,101,0,110,0,99,0,101,0,114,0,66,0,111,0,111,0,115,0,116,0,0,0

.code

ASMreadHUiHudPowerGuage proc
		xorps xmm0, xmm0
		movaps [rdi+0C00h], xmm0
		movaps [rdi+0C10h], xmm0
		movaps [rdi+0C20h], xmm0
		movaps [rdi+0C30h], xmm0
		movaps [rdi+0C40h], xmm0
	; TextFencerDash
		mov qword ptr [rbp+0C0h], 7
		mov [rbp+0B8h], r15
		mov [rbp+0B0h], r15
		mov [rbp+0A8h], r15
		mov r8d, 14
		lea rdx, wstrTextFencerDash
		lea rcx, [rbp+0A8h]
		call rva27380
		align 16
		lea r8, [rbp+0A8h]
		lea rdx, [rbp-28h]
		mov rcx, rdi
		call rva4D86D0
		mov rbx, [rax+8]
		mov r14, [rax]
		test rbx, rbx
		je fencerBoost
		movups xmm0, [r14+270h]
		movups [rdi+0C10h], xmm0
		mov dword ptr [rdi+0C1Ch], 3F800000h
		lock inc dword ptr [rbx+0Ch]
	ofs4CA869:
		mov [rdi+0C08h], rbx
		mov [rdi+0C00h], r14
		mov rbx, [rbp-20h]
		test rbx, rbx
		je ofs4CA8AA
		mov eax, esi
		lock xadd [rbx+8], eax
		cmp eax, 1
		jne ofs4CA8AA
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax]
		mov eax, esi
		lock xadd [rbx+0Ch], eax
		cmp eax, 1
		jne ofs4CA8AA
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+8]
		align 16
	ofs4CA8AA:
		mov rdx, [rbp+0C0h]
		cmp rdx, 8
		jb fencerBoost
		inc rdx
		mov r8d, 2
		mov rcx, [rbp+0A8h]
		call rva27570

	; TextFencerDash
	fencerBoost:
		mov qword ptr [rbp+0C0h], 7
		mov [rbp+0B8h], r15
		mov [rbp+0B0h], r15
		mov [rbp+0A8h], r15
		mov r8d, 15
		lea rdx, wstrTextFencerBoost
		lea rcx, [rbp+0A8h]
		call rva27380
		align 16
		lea r8, [rbp+0A8h]
		lea rdx, [rbp-28h]
		mov rcx, rdi
		call rva4D86D0
		mov rbx, [rax+8]
		mov r14, [rax]
		test rbx, rbx
		je ofs4CA8CC
		movups xmm0, [r14+270h]
		movups [rdi+0C30h], xmm0
		mov dword ptr [rdi+0C3Ch], 3F800000h
		lock inc dword ptr [rbx+0Ch]
	fencerBoost_1:
		mov [rdi+0C28h], rbx
		mov [rdi+0C20h], r14
		mov rbx, [rbp-20h]
		test rbx, rbx
		je fencerBoost_2
		mov eax, esi
		lock xadd [rbx+8], eax
		cmp eax, 1
		jne fencerBoost_2
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax]
		mov eax, esi
		lock xadd [rbx+0Ch], eax
		cmp eax, 1
		jne fencerBoost_2
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+8]
		align 16
	fencerBoost_2:
		mov rdx, [rbp+0C0h]
		cmp rdx, 8
		jb ofs4CA8CC
		inc rdx
		mov r8d, 2
		mov rcx, [rbp+0A8h]
		call rva27570

	ofs4CA8CC:
		mov qword ptr [rbp+0E0h], 7
		mov [rbp+0D8h], r15
		jmp readHUiHudPowerGuageRet
		int 3

ASMreadHUiHudPowerGuage ENDP

align 16

ASMhuiHudPowerGuageFuncP10 proc

	ofs4CBD14:
		lea rcx, [rbp-30h]
		call rva26EE0
		lea rcx, [rbp-50h]
		call rva26EE0
		lea rcx, [rbp-70h]
		call rva26EE0
	ofs4CBD32:
		mov rdx, r14
		mov rcx, rbx
		; call HUiHudBaseFuncP10_4B2FB0
		call module_HUiHudPowerGuageFuncP10_add
		lea r11, [rsp+120h]
		mov rbx, [r11+40h]
		movaps xmm6, [r11-10h]
		movaps xmm7, [r11-20h]
		mov rsp, r11
		pop r15
		pop r14
		pop rdi
		pop rsi
		pop rbp
		ret 
		int 3

ASMhuiHudPowerGuageFuncP10 ENDP

END
