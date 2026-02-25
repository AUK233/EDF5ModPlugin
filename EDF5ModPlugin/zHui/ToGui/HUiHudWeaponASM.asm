.data


extern _str_UIWeaponType : word
extern _SGO_FindNodeFromName : qword
extern _TextWingEnergy6Position : qword
extern ASMgetCurrentMissionClassModelType : proto

extern DigitProcessor_GetPlayerWeaponStatus : proto

.code

ASMhuiHudWeaponInitialization proc

		xorps xmm6, xmm6
		movaps [rdi+0CC0h], xmm6

		; L"ui_weapon_type"
	UIWeaponType:
		lea rdx, _str_UIWeaponType
		lea rcx, [rdi+70h]
		call _SGO_FindNodeFromName
		cmp eax, -1
		je updateWingEnergy
		mov rcx, [rdi+70h]
		movsxd rdx, dword ptr [rcx+12]
		add rdx, rcx
		cdqe
		lea rcx, [rax+rax*2]
		mov eax, [rdx+rcx*4+8]
		mov [rdi+0CC0h], eax

	updateWingEnergy:
		mov r14, [rdi+0A28h] ; get text pointer
		test r14, r14
		je original
		call ASMgetCurrentMissionClassModelType
		cmp eax, 2
		jb original
		mov rdx, _TextWingEnergy6Position
		mov [r14+1A0h], rdx ; write new pos

	original:
		mov rax, rdi
		mov rbx, [rsp+290h]
		movaps xmm6, [rsp+230h]
		add rsp, 240h
		pop r15
		pop r14
		pop r13
		pop r12
		pop rdi
		pop rsi
		pop rbp
		ret 
		int 3

ASMhuiHudWeaponInitialization ENDP

align 16

ASMHUiHudWeaponUpdateAmmoText proc

		mov edx, [rsi+0CC0h]; get Hud_WeaponAlignType
		test edx, edx
		jz original ; If it is 0, ignore it.
		mov rax, [rsi+7E8h] ; get weapon pointer
		cmp dword ptr [rax+2510h], 16 ; check use_extraShotType
		je showWeaponStatus
        cmp byte ptr [rax+5ACh], 0 ; check current
		je original

	showWeaponStatus:
		mov r8, rax
		mov rcx, [rsi+768h]
		call DigitProcessor_GetPlayerWeaponStatus

    original:
        lea r11, qword ptr [rsp+0B0h]
        mov rbx, qword ptr [r11+28h]
        mov rsi, qword ptr [r11+30h]
        mov rsp, r11
        pop r14
        pop rdi
        pop rbp
        ret 
        int 3

ASMHUiHudWeaponUpdateAmmoText ENDP

END
