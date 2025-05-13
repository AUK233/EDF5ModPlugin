.data


extern edf5BDF30Address : qword
extern rva27380 : qword
extern Vehicle504CustomizeAIAimBoneRetAddr : qword

extern pVehicleBegarutaAIAimInitFunc : qword

extern rvaE8E3F8 : qword
extern vedf125AB68 : qword
extern edf73F20Address : qword
extern edf6D730Address : qword
extern vedf125AB80 : qword
extern edf32C1E0Address : qword
extern edf3560D0Address : qword
extern edf6179B0Address : qword

extern pVehicleBegarutaAIAimFunc : qword

extern rva27380 : qword
extern rva751F0 : qword
extern rva37E70 : qword
extern rva27570 : qword

extern _Common_F0P001 : dword
extern _Common_F0P01 : dword
extern _Common_F1P0 : dword
extern _Common_F1P0N : dword
extern _CommonRVA_EE7460 : xmmword
extern _CommonRVA_EE8040 : dword
extern _CommonRVA_EE8070 : xmmword
extern pVehicleBegarutaAIAimActiveAddr : qword

; L"spine"
v508spine DB 73h, 0, 70h, 0, 69h, 0, 6Eh, 0, 65h, 0, 0, 0
align 8
; L"begaruta504_aiming_bone"
v508AimingBone db 98,0,101,0,103,0,97,0,114,0,117,0,116,0,97,0,53,0,48,0,52,0,95,0,97,0,105,0,109,0
db 105,0,110,0,103,0,95,0,98,0,111,0,110,0,101,0,0,0
align 8
; L"begaruta614_aiming_bone"
v614AimingBone db 98,0,101,0,103,0,97,0,114,0,117,0,116,0,97,0,54,0,49,0,52,0,95,0,97,0,105,0,109,0
db 105,0,110,0,103,0,95,0,98,0,111,0,110,0,101,0,0,0
align 8
; L"begaruta614_is_forcedfire"
v614ForceShotBone db 98,0,101,0,103,0,97,0,114,0,117,0,116,0,97,0,54,0,49,0,52,0,95,0,105,0,115,0,95,0
db 102,0,111,0,114,0,99,0,101,0,100,0,102,0,105,0,114,0,101,0,0,0

.code

ASMVehicle504CustomizeAIAimBone proc

	; customize ai aiming bone
		lea rdx, v508AimingBone
		lea rcx, [rsi+0B0h]
		call edf5BDF30Address
		cmp eax, -1
		je ofs362531
		mov r8, [rsi+0B0h]
		cdqe
		movsxd rdx, dword ptr [r8+12]
		add rdx, r8
		lea rcx, [rax+rax*2]
		lea rax, [rdx+rcx*4]
		movsxd rdx, dword ptr [rax+8]
		mov r8d, [rax+4] ; is string size
		add rdx, rax ; is data string
		jmp ofs362523

	ofs362531:
		lea r8d, [rbp+5]
		lea rdx, v508spine
	ofs362523:
		mov rax, Vehicle504CustomizeAIAimBoneRetAddr
		mov [rsp+20h], bp
		lea rcx, [rsp+20h]
		push rax
		jmp rva27380
		int 3

ASMVehicle504CustomizeAIAimBone ENDP

align 16

ASMVehicleBegaruta_AIAimInit proc

		push rsi
		push rdi
		push rbx
		sub rsp, 30h
		mov rsi, rcx
		;start
		call pVehicleBegarutaAIAimInitFunc
		; initialization
		xorps xmm0, xmm0
		movaps [rsi+1EE0h], xmm0
	;begaruta614_aiming_bone
		lea rdx, v614AimingBone
		lea rcx, [rsi+0B0h]
		call edf5BDF30Address
		cmp eax, -1
		je ofs36272C
			mov r8, [rsi+0B0h]
			cdqe
			movsxd rdx, dword ptr [r8+12]
			add rdx, r8
			lea rcx, [rax+rax*2]
			lea rax, [rdx+rcx*4]
			movsxd rbx, dword ptr [rax+8]
			add rbx, rax
			; node 0
			mov r8d, [rbx+4] ; size
			movsxd rdx, dword ptr [rbx+8]
			lea rdx, [rdx+rbx] ; ptr
			; node 1
			mov edi, [rbx+12+4] ; size
			movsxd rcx, dword ptr [rbx+12+8]
			lea rbx, [rcx+rbx+12] ; ptr
		; get bone index
			mov rcx, rsi
			call ASMVehicleBegaruta_GetBoneIndex
			mov [rsi+1EE0h], eax
			mov r8d, edi
			mov rdx, rbx
			mov rcx, rsi
			call ASMVehicleBegaruta_GetBoneIndex
			mov [rsi+1EE4h], eax
		; set seat ai
			mov edx, 1
			mov rcx, rsi
			call ASMVehicleBegaruta_SetDriverSeat
			mov edx, 2
			mov rcx, rsi
			call ASMVehicleBegaruta_SetDriverSeat
			; begaruta614_is_forcedfire
			lea rdx, v614ForceShotBone
			lea rcx, [rsi+0B0h]
			call edf5BDF30Address
			cmp eax, -1
			je ofs36272C
			mov r8, [rsi+0B0h]
			cdqe
			movsxd rdx, dword ptr [r8+12]
			add rdx, r8
			lea rcx, [rax+rax*2]
			mov eax, [rdx+rcx*4+8]
			mov [rsi+1EECh], eax
		;end
	ofs36272C:
		mov al, 1
		add rsp, 30h
		pop rbx
		pop rdi
		pop rsi
		ret 
		int 3

ASMVehicleBegaruta_AIAimInit ENDP

align 16

ASMVehicleBegaruta_GetBoneIndex proc

		mov [rsp+10h], rbx
		mov [rsp+18h], rbp
		push rsi
		push r14
		push r15
		sub rsp, 50h
		mov rsi, rcx
		; start
		xor ebp, ebp
		mov qword ptr [rsp+38h], 7
		mov [rsp+30h], rbp
		lea rcx, [rsp+20h]
		mov word ptr [rsp+20h], bp
		; r8d is input, is str size
		; rdx is input, is str pointer
		call rva27380
		mov r15, [rsi+0CF0h]
		lea rdx, [rsp+20h]
		lea rcx, [r15+0B0h]
		call rva751F0
		mov rbx, rax
		mov r14, [rsp+38h]
		cmp rax, [r15+0B0h]
		je ofs362598
		cmp qword ptr [rax+38h], 8
		lea r8, [rax+20h]
		lea r9, [r8+10h]
		jb ofs362578
		mov r8, [r8]
	ofs362578:
		mov r9, [r9]
		lea rcx, [rsp+20h]
		mov rdx, [rsp+30h]
		cmp r14, 8
		cmovae rcx, [rsp+20h]
		call rva37E70
		test eax, eax
		jns ofs36259F
	ofs362598:
		mov rbx, [r15+0B0h]
	ofs36259F:
		cmp rbx, [r15+0B0h]
		jne ofs3625AD
		or ebx, -1
		jmp ofs3625B0
	ofs3625AD:
		mov ebx, [rbx+40h]
	ofs3625B0:
		cmp r14, 8
		jb ofs36272C
		mov rcx, [rsp+20h]
		lea rdx, [r14+1]
		mov r8d, 2
		call rva27570
		;end
	ofs36272C:
		mov eax, ebx
		mov rbx, [rsp+78h]
		mov rbp, [rsp+80h]
		add rsp, 50h
		pop r15
		pop r14
		pop rsi
		ret 
		int 3

ASMVehicleBegaruta_GetBoneIndex ENDP

align 16

ASMVehicleBegaruta_SetDriverSeat proc
		; rcx is pVehicle, edx is seat
		mov rax, rsp
		push rdi
		sub rsp, 80h
		mov [rax+8], rbx
		mov [rax+10h], rsi
		mov [rsp+30h], edx
		xor ebx, ebx
		mov rsi, rcx
		; start
		mov rax, rvaE8E3F8
		mov [rsp+58h], rax
		mov [rsp+78h], rbx
		lea r9, [rsp+58h]
		mov rax, vedf125AB68
		lea rdx, [rsp+48h]
		mov rcx, [rax]
		call edf73F20Address
		lea rdx, [rsp+38h]
		mov rcx, rax
		call edf6D730Address
		align 8
		or edi, -1
			mov rcx, [rsp+50h]
			test rcx, rcx
			je ofs1207B8
			mov eax, edi
			lock xadd [rcx+12], eax
			cmp eax, 1
			jne ofs1207B8
			mov rax, [rcx]
			call qword ptr [rax+8]
		align 8
	ofs1207B8:
		mov rbx, [rsp+38h]
		cmp dword ptr [rbx+218h], 2
		jne ofs1207CF
		test byte ptr [rbx+280h], 40h
		je ofs120826
		ofs1207CF:
			mov rax, [rbx]
			xor r8d, r8d
			lea edx, [r8+2]
			mov rcx, rbx
			call qword ptr [rax+88h]
			test al, al
			je ofs120826
			cmp dword ptr [rbx+218h], -1
			je ofs12080B
			mov rax, vedf125AB80
			mov rdx, rbx
			mov rcx, [rax]
			call edf32C1E0Address
			; mov [rbx+218h], edi
			; and dword ptr [rbx+280h], -65
		ofs12080B:
			mov dword ptr [rbx+218h], 2
			mov dword ptr [rbx+21Ch], 2
			and dword ptr [rbx+280h], -65
	ofs120826:
		mov r9b, 1
		mov r8d, [rsp+30h]
		mov rdx, rbx
		mov rcx, rsi
		call edf3560D0Address
		mov rdx, rbx
		mov rcx, rsi
		call edf6179B0Address
			mov rcx, [rsp+40h]
			test rcx, rcx
			je ofs12088B
			mov eax, edi
			lock xadd [rcx+8], eax
			cmp eax, 1
			jne ofs12088B
			mov rbx, [rsp+40h]
			mov rax, [rbx]
			mov rcx, rbx
			call qword ptr [rax]
			lock xadd [rbx+12], edi
			cmp edi, 1
			jne ofs12088B
			mov rcx, [rsp+40h]
			mov rax, [rcx]
			call qword ptr [rax+8]
	ofs12088B:
		; end
		lea r11, [rsp+80h]
		mov rbx, [r11+10h]
		mov rsi, [r11+18h]
		mov rsp, r11
		pop rdi
		ret 
		int 3

ASMVehicleBegaruta_SetDriverSeat ENDP

align 16

ASMVehicleBegaruta_AIAim proc

		cmp edx, 1
		jne ofs6484BB
		mov [rsp+18h], rbx
		push rdi
		sub rsp, 20h
		mov rbx, rcx
		; start
		call pVehicleBegarutaAIAimFunc
		;
		cmp qword ptr [rbx+1EE0h], 0
		je ofs6484AD
		; if start
			mov rdx, [rbx+380h]
			mov [rsp+38h], rsi
			test rdx, rdx
			je ofs64844E
			cmp dword ptr [rdx+8], 0
			je ofs64844E
			mov rsi, [rbx+388h]
			test rsi, rsi
			je ofs64844E
			cmp byte ptr [rbx+0C48h], 0
			je ofs648450
			cmp byte ptr [rsi+300h], 0
			jne ofs648450
		ofs64844E:
			xor esi, esi
		ofs648450:
			mov edi, 1
			cmp [rbx+428h], edi ; check pilot count
			je ofs6484AC
			align 16
		ofs648462:
			test rsi, rsi
			je ofs64847E
			cmp byte ptr [rbx+300h], 0
			je ofs648487
			cmp byte ptr [rbx+0C49h], 0
			je ofs648490
			mov r8, rsi
			jmp ofs648493
		ofs64847E:
			cmp byte ptr [rbx+0C48h], 0
			jne ofs648490
		ofs648487:
			cmp byte ptr [rbx+0C49h], 0
			jne ofs64849E
		ofs648490:
			xor r8d, r8d
		ofs648493:
			; r8 is enemy pointer
			mov rdx, rdi
			mov rcx, rbx
			call ASMVehicleBegaruta_AIAimTarget
		ofs64849E:
			inc rdi
			cmp rdi, 2
			jle ofs648462
		ofs6484AC:
			mov rsi, [rsp+38h]
		; if end
	ofs6484AD:	
		mov rbx, [rsp+40h]
		add rsp, 20h
		pop rdi
	ofs6484BB:
		ret 
		int 3

ASMVehicleBegaruta_AIAim ENDP

align 16

ASMVehicleBegaruta_AIAimTarget proc

		push rbx
		sub rsp, 30h
		movaps xmm0, _CommonRVA_EE7460
		xor r9d, r9d
		imul rbx, rdx, 340h
		movaps [rsp+20h], xmm0
		add rbx, [rcx+418h]
		test r8, r8
		setne r9b
		test r9d, r9d
		je ofs63A093
		cmp r9d, 1
		jne ofs63A085
		mov r9, r8
		mov r8, rdx ; ai index
	; check forced fire
		cmp dword ptr [rcx+1EECh], 0
		je AimTarget
        lea r10, [rdx+rdx*8]
        mov rdx, [rcx+448h] ; get weapon pointer's pointer
        mov rax, [rdx+r10*8+10h]
        test rax, rax
        je AimTarget
        mov byte ptr [rax+0D9h], 1
		;
	AimTarget:
		lea rdx, [rsp+20h]
		call ASMVehicleBegaruta_AIAimTargetActive
	ofs63A085:
		movss xmm1, dword ptr [rsp+24h]
		movss xmm2, dword ptr [rsp+20h]
		jmp ofs63A0DD
	ofs63A093:
		movss xmm2, dword ptr [rbx+138h]
		xorps xmm2, _CommonRVA_EE8070
		movss xmm3, _CommonRVA_EE8040
		movaps xmm0, xmm2
		movss xmm4, _Common_F0P001
		andps xmm0, xmm3
		comiss xmm4, xmm0
		movss xmm1, dword ptr [rbx+0F8h]
		xorps xmm1, _CommonRVA_EE8070
		jb ofs63A0CF
		xorps xmm2, xmm2
	ofs63A0CF:
		movaps xmm0, xmm1
		andps xmm0, xmm3
		comiss xmm4, xmm0
		jb ofs63A0DD
		xorps xmm1, xmm1
	ofs63A0DD:
		movss xmm5, _Common_F0P01
		movss xmm3, _Common_F1P0N
		movss xmm0, _Common_F1P0
		divss xmm2, xmm5
		comiss xmm3, xmm2
		jbe ofs63A103
		movaps xmm4, xmm3
		jmp ofs63A10A
	ofs63A103:
		movaps xmm4, xmm0
		minss xmm4, xmm2
	ofs63A10A:
		xorps xmm1, _CommonRVA_EE8070
		divss xmm1, xmm5
		movss dword ptr [rbx+2D4h], xmm4
		comiss xmm3, xmm1
		jbe ofs63A130
		movss dword ptr [rbx+2D0h], xmm3
		add rsp, 30h
		pop rbx
		ret 
	ofs63A130:
		minss xmm0, xmm1
		movss dword ptr [rbx+2D0h], xmm0
		add rsp, 30h
		pop rbx
		ret 
		int 3

ASMVehicleBegaruta_AIAimTarget ENDP

align 16

ASMVehicleBegaruta_AIAimTargetActive proc

		push rbp
		push rsi
		push rdi
		push r13
		lea rbp, [rsp-0D8h]
		sub rsp, 1D8h
		;
		mov r13, rcx
		mov [rbp-70h], rdx
		; new, bone index
		mov eax, [rcx+1EE0h-4+r8*4]
		mov [rbp+10h], eax
		; ai index
		imul rsi, r8, 340h
		mov [rbp+14h], r8d
		add rsi, [rcx+418h]
		;
		xor edi, edi
		cmp [r9+278h], rdi
		jne ofs362BB4
		lea rax, [r9+0A0h]
		jmp ofs362BC3
	ofs362BB4:
		mov rax, [r9+268h]
		mov rax, [rax+8]
		add rax, 10h
	ofs362BC3:
		movaps xmm2, [rax]
		jmp pVehicleBegarutaAIAimActiveAddr
		int 3

ASMVehicleBegaruta_AIAimTargetActive ENDP

END
