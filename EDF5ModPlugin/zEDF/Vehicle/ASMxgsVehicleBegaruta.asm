.data

extern edf5BDF30Address : qword
extern rva27380 : qword
extern Vehicle504CustomizeAIAimBoneRetAddr : qword

extern pVehicleBegarutaAIAimFunc : qword

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

ASMVehicleBegaruta_AIAim proc
;1MOD.dll+30110
		cmp edx, 1
		jne ofs6484BB
		mov [rsp+18h], rbx
		push rdi
		sub rsp, 20h
		mov rbx, rcx

		; start
		call pVehicleBegarutaAIAimFunc
		;mov [rsp+30h], rbp
		mov rdx, [rbx+380h]
		mov [rsp+38h], rsi
		test rdx, rdx
		je ofs64844E
		cmp dword ptr [rdx+8], 0
		je ofs64844E
		mov rsi, [rbx+388h]
		;mov rdi, [rbx+380h]
		test rsi, rsi
		je ofs64844E
		cmp byte ptr [rbx+0C48h], 0
		je ofs648450
		cmp byte ptr [rsi+300h], 0
		jne ofs648450
	ofs64844E:
		xor esi, esi
	ofs648450:
		;mov rbp, qword ptr [rsp+30]
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
		mov rdx, rdi
		mov rcx, rbx
		call ASMVehicleBegaruta_AIAimTarget
	ofs64849E:
		;inc rdi
		;cmp rdi, 2
		;jle ofs648462
		; end

	ofs6484AC:
		mov rsi, [rsp+38h]
		mov rbx, [rsp+40h]
		add rsp, 20h
		pop rdi
	ofs6484BB:
		ret 

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
		;
        mov rdx, [rcx+448h] ; get weapon pointer's pointer
        mov rax, [rdx+72+10h]
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
		; new
		mov eax, 8
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

ASMVehicleBegaruta_AIAimTargetActive ENDP

END
