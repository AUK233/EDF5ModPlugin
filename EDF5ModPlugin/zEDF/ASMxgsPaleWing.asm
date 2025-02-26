.data

extern module_LoadAccessory_PaleWing : proto

extern edf5BDF30Address : qword
extern edf3AE660Address : qword
extern vft_EAF348 : qword
extern vedf125AB68 : qword
extern edf2FDB10Address : qword
extern edf6D730Address : qword

extern edf3AF840Address : qword
extern edf2FBA30Address : qword
extern edf2FD540Address : qword
extern edf6FB40Address : qword

; L"pale_wing_booster_addcore"
ePaleWingBoosterAddcore db 112,0,97,0,108,0,101,0,95,0,119,0,105,0,110,0,103,0,95,0,98,0,111,0,111,0,115,0,116,0
db 101,0,114,0,95,0,97,0,100,0,100,0,99,0,111,0,114,0,101,0,0,0
align 16
; L"pale_wing_booster_color"
ePaleWingBoosterColor db 112,0,97,0,108,0,101,0,95,0,119,0,105,0,110,0,103,0,95,0,98,0,111,0,111,0,115,0,116,0,101,0,114,0,95,0,99,0
db 111,0,108,0,111,0,114,0,0,0

.code

ASMePaleWingInitialization proc

	; initialize memory
		xorps xmm0, xmm0
		xorps xmm1, xmm1
		movaps [rsi+1E00h], xmm0
		movaps [rsi+1E10h], xmm1
	; extra emergency recharge FX
		lea r14, [rsi+0B0h]
		lea rdx, ePaleWingBoosterAddcore
		mov rcx, r14
		call edf5BDF30Address
		movsxd rcx, eax
		cmp ecx, -1
		je ChangeBoosterColor
		mov rax, [r14]
		movsxd rdx, dword ptr [rax+12]
		add rdx, rax
		lea rcx, [rcx+rcx*2]
		lea rax, [rdx+rcx*4]
		movsxd r9, dword ptr [rax+8]
		add r9, rax ; r9 is data string
		; initialize FX
		lea rdx, [rsi+1E00h]
		xor r8d, r8d
		lea rcx, [rsi+6A0h]
		call edf3AE660Address
		mov qword ptr [rbp+130h], 0
		mov rax, vft_EAF348
		mov [rbp+110h], rax
		mov [rbp+138h], rsi
		lea r9, [rbp+110h]
		mov rax, vedf125AB68
		lea rdx, [rbp-10h]
		mov rcx, [rax]
		call edf2FDB10Address
		lea rdx, [rbp+80h]
		lea rcx, [rbp-10h]
		call edf6D730Address
		mov rdi, [rax]
		mov rbx, [rbp+88h]
		test rbx, rbx
		je ofs2F6A17
		or eax, -1
		lock xadd [rbx+8], eax
		cmp eax, 1
		jne ofs2F6A17
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax]
		or eax, -1
		lock xadd [rbx+12], eax
		cmp eax, 1
		jne ofs2F6A17
		mov rax, [rbx]
		mov rcx, rbx
		call qword ptr [rax+8]
	ofs2F6A17:
		mov rcx, [rbp-8]
		test rcx, rcx
		je ofs2F6A37
		or eax, -1
		lock xadd [rcx+12], eax
		cmp eax, 1
		jne ofs2F6A37
		mov rcx, [rbp-8]
		mov rax, [rcx]
		call qword ptr [rax+8]
	ofs2F6A37:
		mov [rsi+1E10h], rdi
		;mov qword ptr [r13+1B08], rdi Original fx
	
	ChangeBoosterColor:
		lea rdx, ePaleWingBoosterColor
		lea rcx, [rsi+0B0h]
		call edf5BDF30Address
		cmp eax, -1
		je ReturnAddress
		mov r8, [rsi+0B0h]
		cdqe ; eax => rax
		movsxd rdx, dword ptr [r8+12]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		lea r12, [rax+rcx*4]
		movsxd rax, dword ptr [r12+8]
		add r12, rax
		mov r13d, [r12+8]
		mov r14d, [r12+12+8]
		mov r15d, [r12+24+8]
		; get booster pointer
		mov rdx, [rsi+1AA0h]
		mov rax, [rsi+1AB0h]
		lea rbx, [rax+rax*2]
		shl rbx, 5
		add rbx, rdx
		cmp rdx, rbx
		je ReturnAddress
		align 16
	nextBooster:
		mov rax, [rdx+10h]
		add rdx, 60h
		mov [rax+1E0h], r13d
		mov [rax+1E0h+4], r14d
		mov [rax+1E0h+8], r15d
		cmp rdx, rbx
		jne nextBooster

	ReturnAddress:
		mov rax, rsi
		mov rbx, [rsp+4A0h]
		movaps xmm6, [rsp+440h]
		add rsp, 450h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret 
		int 3

ASMePaleWingInitialization ENDP

align 16

ASMePaleWingLoadAccessory proc
		mov rcx, rbx
		add rsp, 70h
		pop rbx
		jmp module_LoadAccessory_PaleWing
		int 3
ASMePaleWingLoadAccessory ENDP

align 16

ASMePaleWingActivateRechargeFX proc

		cmp edx, 1
		jne ofs2FADD6
		; fx
		add rcx, 1AF8h
		lea rdx, [rsp+30h]
		call edf3AF840Address
		mov rcx, [rbx+1B08h]
		mov r8, rax
		mov edx, 1
		call ASMePaleWingRechargeFX1C4520
		; next fx
		cmp dword ptr [rbx+1E10h], 0
		je ofs2FAD8E
		lea rcx, [rbx+1E00h]
		lea rdx, [rsp+30h]
		call edf3AF840Address
		mov rcx, [rbx+1E10h]
		mov r8, rax
		mov edx, 1
		call ASMePaleWingRechargeFX1C4520

	ofs2FAD8E:
		cmp [rbx+200h], edx
		je ofs2FAE22
		xor r8d, r8d
		mov rax, edf2FBA30Address
		mov [rsp+20h], rax
		lea rcx, [rbx+1A68h]
		mov [rsp+28h], r8d
		;movaps xmm0, [rsp+20h]
		;movdqa [rsp+20h], xmm0
		cmp [rcx+28h], r8
		je ofs2FAE22
		lea rdx, [rsp+20h]
		call edf2FD540Address
		add rsp, 70h
		pop rbx
		ret 
	ofs2FADD6:
		cmp edx, 2
		jne ofs2FAE22
		add rcx, 1C10h
		mov rax, qword ptr [rcx+8]
		test rax, rax
		je ofs2FAE00
		cmp dword ptr [rax+10h], -1
		je ofs2FADFB
		mov [rax+30h], edx
		mov dword ptr [rax+34h], 3D888889h
	ofs2FADFB:
		call edf6FB40Address
	ofs2FAE00:
		lea rcx, [rbx+1AF8h]
		lea rdx, [rsp+30h]
		call edf3AF840Address
		mov rcx, [rbx+1B08h]
		mov r8, rax
		xor edx, edx
		call ASMePaleWingRechargeFX1C4520
		; next fx
		cmp dword ptr [rbx+1E10h], 0
		je ofs2FAE22
		lea rcx, [rbx+1E00h]
		lea rdx, [rsp+30h]
		call edf3AF840Address
		mov rcx, [rbx+1E10h]
		mov r8, rax
		xor edx, edx
		call ASMePaleWingRechargeFX1C4520
	ofs2FAE22:
		add rsp, 70h
		pop rbx
		ret 
    	int 3

ASMePaleWingActivateRechargeFX ENDP

align 16

ASMePaleWingRechargeFX1C4520 proc
		mov dword ptr [rcx+0C8h], edx
		movaps xmm0, [r8+30h]
		movaps [rcx+190h], xmm0
		movaps xmm1, [r8+20h]
		movaps [rcx+1A0h], xmm1
		movaps xmm0, [r8]
		movaps [rcx+70h], xmm0
		movaps xmm1, [r8+10h]
		movaps [rcx+80h], xmm1
		movaps xmm0, [r8+20h]
		movaps [rcx+90h], xmm0
		movaps xmm1, [r8+30h]
		movaps [rcx+0A0h], xmm1
		;cmp dword ptr [rcx+0C8h], 1
		cmp edx, 1
		jne ofs1C3982
		mov byte ptr [rcx+180h], 1
		mov byte ptr [rcx+490h], 1
		ret 
	ofs1C3982:
		mov byte ptr [rcx+180h], 0
		mov byte ptr [rcx+490h], 0
		ret 
		int 3
ASMePaleWingRechargeFX1C4520 ENDP

END
