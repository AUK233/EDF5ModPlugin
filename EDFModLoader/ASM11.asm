.data
; Use other asm functions
extern edf4738B0 : proto
extern edf4DC190 : proto

extern wwwRetAddr1 : qword
extern wwwRetAddr2 : qword
extern wwwwww : qword

; L"text"
whdiahdi db 116,0,101,0,120,0,116,0,0,0
; L"text_dmg"
dwadds db 116,0,101,0,120,0,116,0,95,0,100,0,109,0,103,0,0,0

.code

ASMwww proc

lea r8, whdiahdi
mov rdx, rdi
call edf4DC190
test rax, rax
je testBlock
cmp dword ptr [rax], 3
jne testBlock
movsxd rdx, dword ptr [rax+8]
mov rcx, wwwwww
mov rcx, qword ptr [rcx]
add rdx, rax
call edf4738B0
jmp wwwRetAddr2

testBlock:
lea r8, dwadds
mov rdx, rdi
call edf4DC190
test rax, rax
je shidw
cmp dword ptr [rax], 3
jne shidw
lea rdx, dwadds
jmp wwwRetAddr2

shidw:
jmp wwwRetAddr1

ASMwww ENDP

END