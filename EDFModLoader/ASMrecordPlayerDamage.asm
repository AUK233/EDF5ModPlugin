.data
extern ASMwww1 : proto
extern playerDmgRetAddress : qword
extern damage_tmp : dword
extern playerAddress : qword

.code
ASMrecordPlayerDamage proc

;push rbx
;call ASMwww1
mov rax,[rsi+10h]
cmp rax, playerAddress
je addDmg
cmp dword ptr [rsi+24h],00
jne fend
mov rcx, playerAddress
mov rcx, [rcx+1168h]
cmp rax, rcx
jne fend
addDmg:
movd ecx,xmm0
addss xmm0,damage_tmp
movss damage_tmp,xmm0
movd xmm0,ecx
fend:
addss xmm0,dword ptr [rdi+000001FCh]
;pop rbx
jmp playerDmgRetAddress

ASMrecordPlayerDamage ENDP
END