.data

; Use other asm functions
extern edf5BDF30Address : qword
;extern edf6136C0 : proto
extern vedf125AB68 : qword
extern GiantAntNormalShotRetAddr : qword
extern InsectAmmoType : qword

extern GiantAntDeathReactionRetAddr : qword

extern _AnimationEvent_BurstFire : word
extern _AnimationEvent_BurstFireStop : word
extern rva27380 : qword
extern rva204EA0 : qword
extern rva27570 : qword
extern GiantAntAnimationEventRetAddr : qword

extern aligned_mallocAddr : qword
extern GiantAntNormalShotFireRetAddr : qword

extern GiantAntNormalShotAddr : qword


extern GiantAntDeathBoneList : xmmword
; L"ant_BulletClass"
antBulletClass db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,108,0,97,0,115,0,115,0,0,0
align 8
; L"ant_BulletParam"
antBulletParam db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,80,0,97,0,114,0,97,0,109,0,0,0
align 8
; L"ant_BulletAlive"
antBulletAlive db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
align 8
; L"ant_BulletColor"
antBulletColor db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
align 8
; L"ant_BulletExSet"
antBulletExSet db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0
align 8
; L"ant_BulletFireHeight"
antBulletHeight db 97,0,110,0,116,0,95,0,66,0,117,0,108,0,108,0,101,0,116,0,70,0,105,0,114,0,101,0,72,0,101,0,105,0,103,0,104,0,116,0,0,0

.code

ASMxgsOCgiantAnt proc

		test rax, rax
		je ofs4694E8
		mov rdx, rbx
		mov rcx, rax
		call ASMGiantAntInitEX
	ofs4694E8:
		add rsp, 30h
		pop rbx
		ret 
		int 3

ASMxgsOCgiantAnt ENDP

align 16

ASMGiantAntInitEX proc

		mov [rsp+8], rbx
		push rsi
		sub rsp, 50h
		mov rbx, rcx
		lea rsi, [rcx+0B0h]
		; initialization
		xor rax, rax
		mov [rcx+1930h], rax ; ammo call address
		mov dword ptr [rcx+1938h], 90 ; burst count
		mov dword ptr [rcx+193Ch], 3DCCCCCDh ; accuracy, 0.1f
		mov [rcx+1940h], rax ; burst state, current burst count
		mov [rcx+1948h], rax ; shot count in new burst, burst interval
		mov [rcx+1950h], rax ; burst interval count,
		mov [rcx+1958h], rax
		; set default death bone
		movaps xmm0, GiantAntDeathBoneList
		movaps [rcx+1960h], xmm0
		movaps xmm1, GiantAntDeathBoneList+10h
		movaps [rcx+1970h], xmm1
		; end
	AmmoTypeBlock:
		lea rdx, antBulletClass
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je ReturnAddress
		mov r8, [rsi]
		cdqe ; eax => rax
		movsxd rdx, dword ptr [r8+12]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		lea rdx, [rax+rcx*4]
		movsxd rax, dword ptr [rdx+8]
		add rdx, rax
		; node 0 is ammo type
		movsxd rcx, dword ptr [rdx+8]
		lea rax, InsectAmmoType
		mov rax, [rax+rcx*8]
		mov [rbx+1930h], rax ; address
		; node 1 is accuracy
		mov eax, dword ptr [rdx+12+8]
		mov [rbx+193Ch], eax
		; node 2 is burst count
		mov eax, dword ptr [rdx+24+8]
		mov [rbx+1938h], eax
		; node 3 is new shot count
		mov eax, dword ptr [rdx+36+8]
		mov [rbx+1948h], eax
		; node 4 is burst interval
		mov eax, dword ptr [rdx+48+8]
		mov [rbx+194Ch], eax
	; It is only activated when +12A0==1, so we have to reactivate it
		lea rdx, antBulletParam
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je ReturnAddress
		mov r8, [rsi]
		cdqe ; eax => rax
		movsxd rdx, dword ptr [r8+12]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		lea rax, [rax+rcx*4]
		mov [rbx+1328h], rax
		
	ReturnAddress:
		add rsp, 50h
		pop rsi
		mov rbx, [rsp+8]
		ret
		int 3

ASMGiantAntInitEX ENDP

align 16

ASMGiantAntUpdateAttack proc

	AmmoAliveBlock:
		lea rdx, antBulletAlive
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je AmmoColorBlock ; if node does not exist, jump
		; read int in node 
		mov r8, [rsi]
		cdqe
		movsxd rdx, dword ptr [r8+0Ch]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		mov ecx, [rax+rcx*4+8]
		mov [rbx+12D8h], ecx ; AmmoAlive

	AmmoColorBlock:
		lea rdx, antBulletColor
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je AmmoExSetBlock ; if node does not exist, jump
		; read ptr in node
		mov r8, [rsi]
		cdqe
		movsxd rdx, dword ptr [r8+0Ch]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		lea rcx, [rax+rcx*4] ; store ptr address
		; change ammo color
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+8]
		movss dword ptr [rbx+1310h], xmm0 ;r
		movss xmm1, dword ptr [rax+rcx+20]
		movss dword ptr [rbx+1314h], xmm1 ;g
		movss xmm0, dword ptr [rax+rcx+32]
		movss dword ptr [rbx+1318h], xmm0 ;b
		movss xmm1, dword ptr [rax+rcx+44]
		movss dword ptr [rbx+131Ch], xmm1 ;a

	AmmoExSetBlock:
		lea rdx, antBulletExSet
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je ShotHeightBlock ; if node does not exist, jump
		; read ptr in node
		mov r8, [rsi]
		cdqe
		movsxd rdx, dword ptr [r8+0Ch]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		lea rcx, [rax+rcx*4] ; store ptr address

		; change shot count
		movsxd rax, dword ptr [rcx+8]
		mov eax, [rax+rcx+8]
		imul eax, [rbx+13B0h] ; old * this
		; check 0
		mov edx, 1
		cmp eax, edx
		cmovl eax, edx ; if < 1
		; write value
		mov [rbx+13B0h], eax

		; change ammo speed
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+20]
		mulss xmm0, dword ptr [rbx+12D4h] ; old * this
		movss dword ptr [rbx+12D4h], xmm0
		; change ammo damage
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+32]
		mulss xmm0, dword ptr [rbx+12DCh] ; old * this
		movss dword ptr [rbx+12DCh], xmm0
		; change ammo explosion range
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+44]
		movss dword ptr [rbx+12E8h], xmm0
		; change ammo penetration
		movsxd rax, dword ptr [rcx+8]
		lea rax, [rax+rcx+48] ; store address
		cmp dword ptr [rax+8],0
		setne al
		mov byte ptr [rbx+12ECh], al
		; change ammo size
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+68]
		movss dword ptr [rbx+12F4h], xmm0
		; change ammo hit size
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+80]
		movss dword ptr [rbx+12F8h], xmm0
		; change ammo hit impulse
		movsxd rax, dword ptr [rcx+8]
		movss xmm0, dword ptr [rax+rcx+92]
		movss dword ptr [rbx+12FCh], xmm0

	ShotHeightBlock:
		lea rdx, antBulletHeight
		mov rcx, rsi
		call edf5BDF30Address ; read sgo node
		cmp eax, -1
		je ofs1FF113 ; if node does not exist, jump
		; read float in node 
		mov r8, [rsi]
		cdqe
		movsxd rdx, dword ptr [r8+0Ch]
		lea rcx, [rax+rax*2]
		lea rax, [r8+rdx]
		movss xmm0, dword ptr [rax+rcx*4+8]
		movss dword ptr [rbx+1320h], xmm0 ; shot height

	ofs1FF113:
		movaps xmm6, [rsp+90h]
		add rsp, 0A8h
		pop rsi
		pop rbx
		ret 
		int 3

ASMGiantAntUpdateAttack ENDP

align 16

ASMGiantAntDeathReaction proc

		cmp dword ptr [rcx+4], 4
		jl ofs200789
		; new block, get death bone
		movsxd rax, dword ptr [rcx+8]
		lea rdx, [rax+rcx+36]
		movsxd rax, dword ptr [rdx+8]
		add rdx, rax
		; node 0
		movsxd rax, dword ptr [rdx+8]
		lea rax, [rax+rdx]
		mov [rdi+1960h], rax
		; node 1
		movsxd rax, dword ptr [rdx+12+8]
		lea rax, [rax+rdx+12]
		mov [rdi+1968h], rax
		; node 2
		movsxd rax, dword ptr [rdx+24+8]
		lea rax, [rax+rdx+24]
		mov [rdi+1970h], rax
		; node 3
		movsxd rax, dword ptr [rdx+36+8]
		lea rax, [rax+rdx+36]
		mov [rdi+1978h], rax
		; =================== end ===================
	ofs200789:
        movsxd rax, dword ptr [rcx+8]
		movss xmm6, dword ptr [rax+rcx+8]
		movss xmm12, dword ptr [rax+rcx+14h]
		jmp GiantAntDeathReactionRetAddr
        int 3

ASMGiantAntDeathReaction ENDP

align 16

ASMGiantAntBurstFire proc

		xor edx, edx
		mov [rcx+1950h], edx ; set burst interval count to 0
		mov dword ptr [rcx+1940h], 1 ; set burst
		mov eax, [rcx+1938h]
		mov [rcx+1944h], eax ; set max count
		mov eax, [rcx+1948h]
		cmp eax, edx
		jle ReturnAddress ; if <= 0
		mov [rcx+13B0h], eax ; set new shot count
	ReturnAddress:
		ret 
		int 3

ASMGiantAntBurstFire ENDP

align 16

ASMGiantAntBurstStop proc

		xor rax, rax
		mov [rcx+1940h], rax
		mov [rcx+1950h], eax
		ret 
		int 3

ASMGiantAntBurstStop ENDP

align 16

ASMGiantAntAnimationEvent proc

	BurstFireBlock:
		lea rax, ASMGiantAntBurstFire
		mov [rbp-50h], rax
		mov [rbp-48h], edi
		mov [rbp-40h], di
		mov [rbp-30h], rdi
		mov qword ptr [rbp-28h], 7
		mov r8d, 9 ; string length
		lea rdx, _AnimationEvent_BurstFire ; BurstFire
		lea rcx, [rbp-40h]
		call rva27380
		movaps xmm0, [rbp-50h]
		movdqa [rbp-50h], xmm0
		lea r9, [rbp-50h]
		mov r8, rsi
		lea rdx, [rbp-40h]
		mov rcx, r15
		call rva204EA0
		mov rdx, [rbp-28h]
		cmp rdx, 8
		jb BurstStopBlock
		mov rcx, [rbp-40h]
		inc rdx
		mov r8d, 2
		call rva27570

	BurstStopBlock:
		lea rax, ASMGiantAntBurstStop
		mov [rbp-50h], rax
		mov [rbp-48h], edi
		mov [rbp-40h], di
		mov [rbp-30h], rdi
		mov qword ptr [rbp-28h], 7
		mov r8d, 14 ; string length
		lea rdx, _AnimationEvent_BurstFireStop ; BurstFire
		lea rcx, [rbp-40h]
		call rva27380
		movdqa xmm0, [rbp-50h]
		movdqa [rbp-50h], xmm0
		lea r9, [rbp-50h]
		mov r8, rsi
		lea rdx, [rbp-40h]
		mov rcx, r15
		call rva204EA0
		mov rdx, [rbp-28h]
		cmp rdx, 8
		jb ofs1FD6BB
		mov rcx, [rbp-40h]
		inc rdx
		mov r8d, 2
		call rva27570

	ofs1FD6BB:
		mov r12d, [rsi+430h]
		mov r8d, edi
		mov r14, [rsi+7B0h]
		jmp GiantAntAnimationEventRetAddr
		int 3

ASMGiantAntAnimationEvent ENDP

align 16

ASMGiantAntNormalShot proc

		mov rax, vedf125AB68
		lea r9, [rbp+50h]
		lea r8, [rbp-80h]
		lea rdx, [rsp+40h]
		mov rcx, [rax]
		mov rax, [rsi+1930h]
		test rax, rax
		je ofs1FF6FF
		;mov [rsp+20h], rsi
		call rax
		jmp GiantAntNormalShotRetAddr
	ofs1FF6FF:
		call InsectAmmoType+8;AcidBullet01
		jmp GiantAntNormalShotRetAddr
		int 3

ASMGiantAntNormalShot ENDP

align 16

ASMGiantAntFuncP10 proc

		cmp [rbx+1940h], r14d ; if == 0
		je ofs1FDEEA ; if == 0
		; check current burst count
		mov eax, [rbx+1944h]
		cmp eax, r14d
		jle ofs1FDEEA ; if <= 0
		; check burst interval count
		mov ecx, [rbx+1950h]
		cmp ecx, r14d
		jle FireBlock ; if <= 0
		dec ecx
		mov [rbx+1950h], ecx
		jmp ofs1FDEEA
	FireBlock:
		dec eax
		mov [rbx+1944h], eax
		mov eax, [rbx+194Ch]
		mov [rbx+1950h], eax
		mov rcx, rbx
		call GiantAntNormalShotAddr
	ofs1FDEEA:
		lea r11, [rsp+90h]
		mov rbx, [r11+10h]
		mov rsi, [r11+18h]
		mov rdi, [r11+20h]
		mov r14, [r11+28h]
		mov rsp, r11
		pop rbp
		ret 
		int 3

ASMGiantAntFuncP10 ENDP

align 16

ASMGiantAntNormalShotFire proc

		mov rax, rsp
		push rsi
		push rdi
		push r12
		push r14
		push r15
		sub rsp, 60h
		mov qword ptr [rax-60h], -2
		mov [rax+8h], rbx
		mov [rax+10h], rbp
		mov r15, [rax+28h]
		mov rdi, r9
		mov rsi, rdx
		mov r14, rcx
		xor r12d, r12d
		mov dword ptr [rsp+20h], r12d
		xorps xmm0, xmm0
		movdqu [rax-58h], xmm0
		mov [r9+8h], r8
		mov [r9+10h], r12
		mov [r9+18h], r12
		lea rax, [rax-58h]
		mov [r9+20h], rax
		mov edx, 10h
		mov ecx, 1000h
		call aligned_mallocAddr
		mov [rsp+0A0h], rax
		test rax, rax
		je ofs204439
		mov rdx, rdi
		mov rcx, rax
		call qword ptr [r15+1930h]
		mov rbp, rax
		jmp ofs20443C
	ofs204439:
		mov rbp, r12
	ofs20443C:
		mov rcx, [rsp+30h]
		jmp GiantAntNormalShotFireRetAddr
		int 3

ASMGiantAntNormalShotFire ENDP

END