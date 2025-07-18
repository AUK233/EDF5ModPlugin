.data

extern rva9C6E40 : qword

extern aligned_mallocAddr : qword
extern rvaF3F10 : qword
extern rva1CD6C0 : qword
extern rvaBDAD0 : qword

extern noThrowAnime : dword
extern HUDEnhanceStatus : dword

extern edf2E0270Address : qword
extern edf2E07C0Address : qword
extern edf2E18A0Address : qword
extern eArmySoldierUseAuxiliaryRetAddr : qword
extern edf5F8C40Address : qword


extern _Common_PI : dword
extern _Common_F0P5 : dword
extern _Common_F0P025 : dword
extern _Common_F0P025N : dword
extern _Common_F1P58 : dword
extern _Common_F1P58N : dword
extern _Common_F0P75N : dword
extern __sqrtfAddr : qword
extern eArmySoldierDashTurningRetAddr : qword

extern eSoldierCallSupportRetAddr : qword
extern hudShowSupportSlot2RetAddr : qword

extern eEngineerUseAuxiliaryRetAddr : qword
extern edf2E2E30Address : qword

; L"support_call"
eSoldierCallSupport db 115,0,117,0,112,0,112,0,111,0,114,0,116,0,95,0,99,0,97,0,108,0,108,0,0,0
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR.sgo"
lyt_HudSubWeaponGuageR1 db 61h, 00h, 70h, 00h, 70h, 00h, 3Ah, 00h, 2Fh, 00h, 75h, 00h, 69h, 00h, 2Fh, 00h, 6Ch, 00h, 79h, 00h, 74h, 00h, 5Fh, 00h, 48h, 00h, 75h, 00h, 64h, 00h, 53h, 00h
db 75h, 00h, 62h, 00h, 57h, 00h, 65h, 00h, 61h, 00h, 70h, 00h, 6Fh, 00h, 6Eh, 00h, 47h, 00h, 75h, 00h, 61h, 00h, 67h, 00h, 65h, 00h, 52h, 00h, 2Eh, 00h, 73h, 00h, 67h, 00h, 6Fh, 00h, 00h, 00h
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR_E.sgo"
lyt_HudSubWeaponGuageR1E db 97,0,112,0,112,0,58,0,47,0,117,0,105,0,47,0,108,0,121,0,116,0,95,0,72,0,117,0,100,0,83,0,117,0,98,0,87,0,101,0,97,0,112,0
db 111,0,110,0,71,0,117,0,97,0,103,0,101,0,82,0,95,0,69,0,46,0,115,0,103,0,111,0,0,0
align 16
; L"app:/ui/lyt_HudSubWeaponGuageR2.sgo"
lyt_HudSubWeaponGuageR2 db 97,0,112,0,112,0,58,0,47,0,117,0,105,0,47,0,108,0,121,0,116,0,95,0,72,0,117,0,100,0,83,0,117,0,98,0,87,0,101,0,97,0
db 112,0,111,0,110,0,71,0,117,0,97,0,103,0,101,0,82,0,50,0,46,0,115,0,103,0,111,0,0,0

.code

ASMeAssultSoldierInitialization proc

	; initialize memory
		mov dword ptr [rbx+1BD0h], 41280000h ; 10.5f
		xorps xmm0, xmm0
		movaps [rbx+1BE0h], xmm0
	ofs2DF10D:
		;mov rcx, qword ptr [rsp+68h]
		;xor rcx, rsp
		;call rva9C6E40
		mov rax, rbx
		mov rbx, qword ptr [rsp+0A0h]
		add rsp, 70h
		pop rdi
		pop rsi
		pop rbp
		ret 
		int 3

ASMeAssultSoldierInitialization ENDP

align 16

ASMeAssultSoldierActionInit proc

		mov edx, 10h
		mov [rbp-18h], ebx
		lea rax, ASMeSoldierCallSupport
		mov [rbp-20h], rax
		lea ecx, [rdx+10h]
		call aligned_mallocAddr
		mov rcx, rax
		test rax, rax
		je is0jump_1
		mov rax, [rdi+1A20h]
		mov rbx, rcx
		movups xmm0, [rbp-20h]
		mov [rcx], rsi
		mov [rcx+18h], rax
		movups [rcx+8], xmm0
	is0jump_1:
		mov rdx, rbx
		lea rcx, [rbp-10h]
		call rvaF3F10
		mov rcx, [rdi+1A28h]
		mov rdx, rax
		add rcx, 10h
		call rva1CD6C0
	; original
	ofs2E0613:
		lea rcx, [rdi+0D70h]
		mov rdx, [rcx+18h]
		cmp [rcx+10h], rdx
		jae ofs2E0629
		call rvaBDAD0
	ofs2E0629:
		mov rbx, [rsp+58h]
		mov rsi, [rsp+60h]
		mov rdi, [rsp+68h]
		add rsp, 40h
		pop rbp
		ret 
		int 3

ASMeAssultSoldierActionInit ENDP

align 16

ASMeArmySoldierUseAuxiliary proc

		cmp byte ptr [rbx+0B5Eh], 0 ; old is +B5D, but we now need the hold-on version
		mov eax, [rbx+1BE0h] ; throw button timer
		je throwTimer
		inc eax
		mov [rbx+1BE0h], eax
		cmp eax, 20
		jbe ofs2DF473
		mov dword ptr [rbx+1BE0h], 0
		;Get a weapon with support slot 1
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		cmp ecx, dword ptr [rbx+15A0h]
		jge ofs2DF473
		test ecx, ecx
		js ofs2DF473
		mov rax, qword ptr [rbx+1590h]
		mov rcx, qword ptr [rax+rcx*8]
		test rcx, rcx
		je ofs2DF473
		cmp dword ptr [rcx+2500h], -1
		jne ofs2DF473
		mov dword ptr [rbx+1BE4h], 10 ; throw button cd
		mov rax, [rcx]
		call qword ptr [rax+80h] ; Request to reload
		jmp ofs2DF473

	throwCD:
		dec dword ptr [rbx+1BE4h]
		jmp ofs2DF473

	throwTimer:
		cmp dword ptr [rbx+1BE4h], 0
		jg throwCD
		test eax, eax
		je ofs2DF473
		mov dword ptr [rbx+1BE0h], 0 ; clear timer
		cmp eax, 10
		ja ofs2DF473 ; if it <= 10
		cmp qword ptr [rbx+1610h], 0
		jbe ofs2DF4C1
		;Get a weapon with support slot 1
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		;inc rcx ; support slot 2
		cmp ecx, dword ptr [rbx+15A0h]
		jge ofs2DF4C1
		test ecx, ecx
		js ofs2DF4C1
		mov rax, qword ptr [rbx+1590h]
		mov rdx, qword ptr [rax+rcx*8]
		test rdx, rdx
		je ofs2DF4C1
		; check ROF allowed to fire
		cmp dword ptr [rdx+0B40h], 0
		jne ofs2DF4C1
		cmp dword ptr [rdx+8E8h], 0
		jbe ofs2DF4C1
		; use support slot 1
		cmp noThrowAnime, 0
		jne noThrowAnimeBlock
		lea rax, ASMeSoldierCallSupport
		jmp ofs2DF496
	noThrowAnimeBlock:
		mov byte ptr [rdx+0D9h], 1
		jmp ofs2DF4C1

	ofs2DF473:
		cmp byte ptr [rbx+0B5Bh+1], 0 ; old is +B5B, but we now need the hold-on version
		mov r11d, [rbx+1BE8h] ; dash button timer
		je checkDashTimer
		inc r11d
		mov [rbx+1BE8h], r11d
		; if hold-on for more than 1s, it also go to dash
		cmp r11d, 60
		ja clearDashTimer
		jmp ofs2DF4C1

	checkDashTimer:
		test r11d, r11d
		jz ofs2DF4C1
	clearDashTimer:
		mov dword ptr [rbx+1BE8h], 0
	; check in air
		cmp dword ptr [rbx+560h], 2
		jne ofs2DF4C1
		;
		cmp qword ptr [rbx+1618h], 0
		je checkHasSlot2
		cmp dword ptr [rbx+1620h], 1
		jge ofs2DF4C1

	checkHasSlot2:
		;Get a weapon with support slot 2
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		inc rcx ; support slot 2
		cmp ecx, dword ptr [rbx+15A0h]
		jge useDash
		test ecx, ecx
		js useDash
		mov rax, qword ptr [rbx+1590h]
		mov rdx, qword ptr [rax+rcx*8]
		test rdx, rdx
		je useDash
		; check is it a weapon
		cmp dword ptr [rdx+698h], 0 ;AmmoAlive
		je useDash
		; if hold the button then dash
		cmp r11d, 15
		ja useDash
		; check current ammo count
		cmp dword ptr [rdx+8E8h], 0
		jle ofs2DF4C1
		; use support slot 2
		mov rax, edf2E0270Address
		jmp ofs2DF496
	useDash:
		; use dash
		mov rax, edf2E07C0Address
	ofs2DF496:
		mov [rbp-69h], rax
		lea rcx, [rbx+1A00h]
		cmp qword ptr [rcx+28h], 0
		mov dword ptr [rbp-61h], 0
		movaps xmm0, [rbp-69h]
		movdqa [rbp-69h], xmm0
		je ofs2DF4C1
		lea rdx, qword ptr [rbp-69h]
		call edf2E18A0Address
	ofs2DF4C1:
		jmp eArmySoldierUseAuxiliaryRetAddr
		int 3

ASMeArmySoldierUseAuxiliary ENDP

align 16

ASMeArmySoldierDashTurning proc

		movss xmm0, dword ptr [rcx+0B30h] ; get lateral velocity
		mulss xmm0, _Common_PI
		movss xmm1, dword ptr [rcx+1A78h]
		movss xmm2, dword ptr [rcx+0F18h]
		movaps xmm3, xmm1
		mulss xmm0, _Common_F0P5
		mulss xmm3, _Common_F0P025N
		movaps [rsp+0A0h], xmm7
		movaps [rsp+90h], xmm8
		subss xmm0, xmm2
		movaps [rsp+80h], xmm9
		movaps [rsp+70h], xmm10
		movss xmm8, _Common_F0P025
		comiss xmm3, xmm0
		ja negativePI
		mulss xmm1, xmm8
		minss xmm1, xmm0
		movaps xmm3, xmm1
	negativePI:
		movss xmm0, _Common_F1P58N
		addss xmm2, xmm3
		comiss xmm0, xmm2
		ja write2direction
		movss xmm0, _Common_F1P58
		minss xmm0, xmm2
	write2direction:
		movss dword ptr [rcx+0F18h], xmm0
		; check forward velocity
		movss xmm0, dword ptr [rcx+0B38h]
		comiss xmm0, _Common_F0P75N ; forward velocity > -0.75
		ja original
		mov byte ptr [rcx+0B5Bh], 2
	original:
		lea rsi, [rcx+0E50h]
		movaps xmm0, [rsi]
		mulps xmm0, xmm0
		xorps xmm8, xmm8
		movaps xmm1, xmm0
		shufps xmm1, xmm0, 55h
		addss xmm1, xmm0
		shufps xmm0, xmm0, 0AAh
		addss xmm1, xmm0
		ucomiss xmm1, xmm8
		jp ofs2DFDA1
		jne ofs2DFDA1
		movaps xmm10, xmm8
		jmp ofs2DFDAD
	ofs2DFDA1:
		movaps xmm0, xmm1
		call __sqrtfAddr
		movaps xmm10, xmm0
	ofs2DFDAD:
		movss xmm6, dword ptr [rdi+0E94h]
		addss xmm6, dword ptr [rdi+0F18h]
		movaps xmm0, xmm6
		jmp eArmySoldierDashTurningRetAddr
		int 3

ASMeArmySoldierDashTurning ENDP

align 16

ASMeSoldierCallSupport proc

		push rbx
		sub rsp, 30h
		mov rbx, rcx
		test edx, edx
		jne ofs2DF768
		mov dword ptr [rcx+420h], 81h
		xor r8d, r8d
		mov eax, [rcx+1050h]
		imul rdx, rax, 1A0h
		mov rax, [rcx+9D0h]
		mov [rsp+48h], rsi
		mov r10, [rdx+rax+118h]
		mov r9d, [r10+4]
		test r9d, r9d
		je ofs2DF73E
		movsxd r11, dword ptr [r10+8]
		mov [rsp+40h], rbp
		lea rbp, eSoldierCallSupport
		
		align 16
	ofs2DF6D0:
		lea rcx, qword ptr [r8+r8*8]
		lea rax, qword ptr [r11+rcx*4]
		add rax, r10
		movsxd rdx, dword ptr [rax]
		add rdx, rax
		;or rcx, -1
		mov rcx, -4

		align 16
	ofs2DF6F0:
		;movzx eax, word ptr [rdx+rcx*2+2]
		;cmp ax, word ptr [rbp+rcx*2+2]
		;jne ofs2DF711
		;add rcx, 2
		;cmp rcx, 13
		;je ofs2DF71B
		;movzx eax, word ptr [rdx+rcx*2]
		;cmp ax, word ptr [rbp+rcx*2]
		;je ofs2DF6F0

		add rcx, 4
		cmp rcx, 12
		jge checkNull
		mov eax, [rdx+rcx*2]
		cmp eax, [rbp+rcx*2]
		je ofs2DF6F0
	checkNull:
		movzx eax, word ptr [rdx+rcx*2]
		test eax, eax
		je ofs2DF71B

	ofs2DF711:
		inc r8d
		cmp r8d, r9d
		jne ofs2DF6D0
		jmp ofs2DF739
	ofs2DF71B:
		cmp r8d, -1
		je ofs2DF739
		mov edx, [rbx+1050h]
		lea rcx, [rbx+970h]
		mov r9d, 1
		call edf5F8C40Address
	ofs2DF739:
		mov rbp, [rsp+40h]
	ofs2DF73E:
		mov eax, [rbx+1050h]
		mov rsi, [rsp+48h]
		imul rcx, rax, 1A0h
		mov rax, [rbx+9D0h]
		mov dword ptr [rcx+rax+138h], 3F800000h
		add rsp, 30h
		pop rbx
		ret
	ofs2DF768:
		jmp eSoldierCallSupportRetAddr
		int 3

ASMeSoldierCallSupport ENDP

align 16

ASMhudShowSupportSlot2 proc

		lea r9, qword ptr [rbp-60h]
		lea rdx, qword ptr [rbp+48h]
		cmp ebx, 1
		je ShowSupportSlot2
		cmp HUDEnhanceStatus, 0
		jne SupportSlot1Enhance
		lea r8, lyt_HudSubWeaponGuageR1
		jmp hudShowSupportSlot2RetAddr
	SupportSlot1Enhance:
		;test rbx, rbx
		;jne SupportSlot1Enhance2
		;mov rax, [r15+1590h]
		;mov rax, [rax+r10*8]
		;mov dword ptr [rax+2510h], 16 ; use_extraShotType
	SupportSlot1Enhance2:
		lea r8, lyt_HudSubWeaponGuageR1E
		jmp hudShowSupportSlot2RetAddr
	ShowSupportSlot2:
		lea r8, lyt_HudSubWeaponGuageR2
		jmp hudShowSupportSlot2RetAddr
		int 3

ASMhudShowSupportSlot2 ENDP

align 16

ASMeEngineerInitialization proc

	; initialize memory
		xorps xmm0, xmm0
		movaps [rbx+1AE0h], xmm0
	ofs2E1747:
		mov rcx, [rsp+50h]
		xor rcx, rsp
		call rva9C6E40
		mov rax, rbx
		lea r11, [rsp+60h]
		mov rbx, [r11+20h]
		mov rsi, [r11+28h]
		mov rsp, r11
		pop rdi
		ret 
		int 3

ASMeEngineerInitialization ENDP

align 16

ASMeEngineerUseAuxiliary proc
	;Get a weapon with support slot 1
		cmp byte ptr [rbx+0B5Eh], 0 ; old is +B5D, but we now need the hold-on version
		mov eax, [rbx+1AE0h] ; throw button timer
		je throwTimer
		inc eax
		mov [rbx+1AE0h], eax
		cmp eax, 20
		jbe use2Slot
		mov dword ptr [rbx+1AE0h], 0
		;Get a weapon with support slot 1
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		cmp ecx, dword ptr [rbx+15A0h]
		jge use2Slot
		test ecx, ecx
		js use2Slot
		mov rax, qword ptr [rbx+1590h]
		mov rcx, qword ptr [rax+rcx*8]
		test rcx, rcx
		je use2Slot
		cmp dword ptr [rcx+2500h], -1
		jne use2Slot
		mov dword ptr [rbx+1AE4h], 10 ; throw button cd
		mov rax, [rcx]
		call qword ptr [rax+80h] ; Request to reload
		jmp use2Slot
	throwCD:
		dec dword ptr [rbx+1AE4h]
		jmp use2Slot
	throwTimer:
		cmp dword ptr [rbx+1AE4h], 0
		jg throwCD
		test eax, eax
		je use2Slot
		mov dword ptr [rbx+1AE0h], 0 ; clear timer
		cmp eax, 10
		ja use2Slot ; if it <= 10
		test byte ptr [rbx+420h], 4
		jne ofs2E19FC
		cmp qword ptr [rbx+1610h], 0
		jbe ofs2E19FC
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		cmp ecx, dword ptr [rbx+15A0h]
		jge ofs2E19FC
		test ecx, ecx
		js ofs2E19FC
		mov rax, qword ptr [rbx+1590h]
		mov rdx, qword ptr [rax+rcx*8]
		test rdx, rdx
		je ofs2E19FC
		cmp dword ptr [rdx+8E8h], 0
		jbe ofs2E19FC
		; use support slot 1
		cmp noThrowAnime, 0
		jne noThrowAnimeBlock
		lea rax, ASMeSoldierCallSupport
		jmp common
	noThrowAnimeBlock:
		mov byte ptr [rdx+0D9h], 1
		jmp ofs2E19FC

	use2Slot:
		;Get a weapon with support slot 2
		cmp byte ptr [rbx+0B5Bh], 0
		je ofs2E19FC
		test byte ptr [rbx+420h], 4
		jne ofs2E19FC
		cmp qword ptr [rbx+1610h], 0
		jbe ofs2E19FC
		mov rax, qword ptr [rbx+1600h]
		movsxd rcx, dword ptr [rax]
		inc rcx ; support slot 2
		cmp ecx, dword ptr [rbx+15A0h]
		jge ofs2E19FC
		test ecx, ecx
		js ofs2E19FC
		mov rax, qword ptr [rbx+1590h]
		mov rdx, qword ptr [rax+rcx*8]
		test rdx, rdx
		je ofs2E19FC
		cmp dword ptr [rdx+8E8h], 0
		jbe ofs2E19FC
		; use support slot 2
		mov rax, edf2E0270Address
	common:
		mov qword ptr [rsp+30h], rax
		lea rcx, qword ptr [rbx+1A00h]
		cmp qword ptr [rcx+28h], 0
		mov dword ptr [rsp+38h], 0
		movaps xmm0, xmmword ptr [rsp+30h]
		movdqa xmmword ptr [rsp+30h], xmm0
		je ofs2E19FC
		lea rdx, qword ptr [rsp+30h]
		call edf2E2E30Address
	ofs2E19FC:
		jmp eEngineerUseAuxiliaryRetAddr
		int 3

ASMeEngineerUseAuxiliary ENDP

END
