.data

extern vedf125AB90 : qword
extern gameStartupReadVoiceFileRetAddr : qword
extern LoadNewVoiceFilesCPP : proto

extern ASMrva47C6E0modRetAddr : qword
extern rva27380 : qword

extern gameStartupReadMusicFileRetAddr : qword
extern rva478020 : qword
extern rva27570 : qword

extern _Common_F1P0 : dword
extern rva4781C0 : qword
extern rva47C9D0 : qword
extern SoundControllerPlayBgmRetAddr : qword

extern ThrowInvalidSGOFilenameCPP : proto

; L"app:/sound/adx/test_music.acb"
align 16
wstrAddMusic1 db 97,0,112,0,112,0,58,0,47,0,115,0,111,0,117,0,110,0,100,0,47,0,97,0,100,0,120,0,47,0,116,0,101,0
db 115,0,116,0,95,0,109,0,117,0,115,0,105,0,99,0,46,0,97,0,99,0,98,0,0,0

.code

ASMgameStartupReadVoiceFile proc

		mov rcx, vedf125AB90
		mov rcx, [rcx]
		call LoadNewVoiceFilesCPP
		mov qword ptr [rbp+98h], 7
		mov [rbp+90h], rdi
		mov [rbp+80h], di
		jmp gameStartupReadVoiceFileRetAddr
		int 3

ASMgameStartupReadVoiceFile ENDP

align 16

ASMrva47C6E0mod proc

		mov r11, rsp
		push rdi
		sub rsp, 60h
		mov qword ptr [r11-38h], -2
		mov qword ptr [r11+18h], rbx
		mov [rsp+5Ch], r9d
		mov [rsp+58h], r8d
		mov rbx, rcx
		xorps xmm0, xmm0
		movaps [rsp+20h], xmm0
		mov qword ptr [r11-18h], 7
		xor r8d, r8d
		mov [r11-20h], r8
		mov [r11-30h], r8w
		cmp [rdx], r8w
		je ofs47BB3B
		or r8, -1
		align 16
	ofs47BB30:
		inc r8
		cmp word ptr [rdx+r8*2], 0
		jne ofs47BB30
	ofs47BB3B:
		lea rcx, [rsp+38h]
		call rva27380
		mov r9d, [rsp+5Ch]
		mov r8d, [rsp+58h]
		jmp ASMrva47C6E0modRetAddr
		int 3

ASMrva47C6E0mod ENDP

align 16

ASMgameStartupReadMusicFile proc

		xorps xmm0, xmm0
		movaps [rsi+0D0h], xmm0
		movaps [rsi+0D0h+10h], xmm0
		movaps [rsi+0D0h+20h], xmm0
		movaps [rsi+0D0h+30h], xmm0
		movaps [rsi+0D0h+40h], xmm0
		;
		mov qword ptr [rbp-29h], 7
		mov [rbp-31h], r15
		mov [rbp-41h], r15w
		mov r8d, 29
		lea rdx, wstrAddMusic1
		lea rcx, [rbp-41h]
		call rva27380
		mov r9d, 2
		lea r8d, [r9-1]
		lea rdx, [rbp-41h]
		lea rcx, [rsi+0D0h]
		call rva478020
		mov rdx, [rbp-29h]
		cmp rdx, 8
		jb ofs47A9E5
		inc rdx
		mov r8d, 2
		mov rcx, [rbp-41h]
		call rva27570
	ofs47A9E5:
		mov qword ptr [rbp-29h], 7
		mov [rbp-31h], r15
		mov [rbp-41h], r15w
		jmp gameStartupReadMusicFileRetAddr
		int 3

ASMgameStartupReadMusicFile ENDP

align 16

ASMSoundControllerPlayBgm proc

		mov [rsp+30h], rdx
		movss xmm2, _Common_F1P0
		movaps xmm3, xmm2
		call rva4781C0
		test al, al
		jnz ofs11ADF3
		mov r9, vedf125AB90
		mov rdx, [rsp+30h]
		mov rcx, [r9]
		lea r8, [rcx+20h]
		call rva47C9D0
	ofs11ADF3:
		jmp SoundControllerPlayBgmRetAddr
		int 3

ASMSoundControllerPlayBgm ENDP

align 16

ASMgameReadInvalidSGO proc

		lea rcx, qword ptr [rbp+7]
		call ThrowInvalidSGOFilenameCPP
		int 3

ASMgameReadInvalidSGO ENDP

END
