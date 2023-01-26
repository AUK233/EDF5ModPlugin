.data
extern edf4738B0Address : qword
extern edf4DC190Address : qword
extern edf5BDF30Address : qword

; Export public functions
public edf4738B0
public edf4DC190
public edf5BDF30

.code

edf4738B0 proc
jmp edf4738B0Address
edf4738B0 ENDP

edf4DC190 proc
jmp edf4DC190Address
edf4DC190 ENDP

edf5BDF30 proc
jmp edf5BDF30Address
edf5BDF30 ENDP

END