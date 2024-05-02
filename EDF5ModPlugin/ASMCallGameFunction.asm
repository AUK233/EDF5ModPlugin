.data
extern rva47C6E0 : qword

extern edf8C8C0Address : qword
extern edf3982A0Address : qword
extern edf3983B0Address : qword
extern edf3AE530Address : qword
extern edf4738B0Address : qword
extern edf4DC190Address : qword
extern edf5BDF30Address : qword
extern edf6136C0Address : qword

; Export public functions
public edf8C8C0
public edf3982A0
public edf3983B0
public edf3AE530
public edf4738B0
public edf4DC190
public edf5BDF30
public edf6136C0

.code

ASMrva47C6E0 proc
    jmp rva47C6E0
    int 3
ASMrva47C6E0 ENDP

align 16

edf8C8C0 proc
jmp edf8C8C0Address
int 3
edf8C8C0 ENDP

align 16

edf3982A0 proc
jmp edf3982A0Address
int 3
edf3982A0 ENDP

align 16

edf3983B0 proc
jmp edf3983B0Address
int 3
edf3983B0 ENDP

align 16

edf3AE530 proc
jmp edf3AE530Address
int 3
edf3AE530 ENDP

align 16

edf4738B0 proc
jmp edf4738B0Address
int 3
edf4738B0 ENDP

align 16

edf4DC190 proc
jmp edf4DC190Address
int 3
edf4DC190 ENDP

align 16

edf5BDF30 proc
jmp edf5BDF30Address
int 3
edf5BDF30 ENDP

align 16

edf6136C0 proc
jmp edf6136C0Address
int 3
edf6136C0 ENDP

END