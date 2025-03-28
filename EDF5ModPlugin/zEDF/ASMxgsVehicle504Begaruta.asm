.data

extern edf5BDF30Address : qword
extern rva27380 : qword
extern Vehicle504CustomizeAIAimBoneRetAddr : qword

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

END
