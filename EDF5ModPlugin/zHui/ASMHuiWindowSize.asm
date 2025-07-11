.data

extern CustomAdjustWindowRect : proto


extern AdjustWindowRectRetAddr : qword

.code

ASMAdjustWindowRect proc

	lea rcx, [rsp+48h]
	call CustomAdjustWindowRect
	mov r10d, [rsp+54h]
	xor r9d, r9d
	mov r11d, [rsp+50h]
	mov r8d, r9d
	jmp AdjustWindowRectRetAddr
	int 3

ASMAdjustWindowRect ENDP

END
