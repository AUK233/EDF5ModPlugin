.data

extern setDamageString : proto

extern hookTextDisplayRetAddr : qword

.code

; [rbx+60h] pstr
ASMhookTextDisplay proc
; check size is required value
mov rcx, qword ptr [rbx+80h]
cmp ecx, 95
jl ofs4B09B4 ; if false, return to original
mov rdx, r14
mov rcx, rbx
call setDamageString 

ofs4B09B4:
lea rax, qword ptr [rbx+60h]
cmp qword ptr [rax+18h], 8
jb ofs4B09C2
mov rax, qword ptr [rax]
ofs4B09C2:
mov rcx, qword ptr [rbx+80h]
jmp hookTextDisplayRetAddr

ASMhookTextDisplay ENDP

END