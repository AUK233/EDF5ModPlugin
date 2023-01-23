.data
extern edf5BDF30Address : qword

colorR db 'a',0,'nt_BulletColor', 0,0,0
color db "a",0,"n",0,"t",0,"_",0,"0",0,"2",00h,00h,00h
shotnum db "a",0,"n",0,"t",0,"_",0,"0",0,"1",00h,00h,00h

.code
edf5BDF30 proc
jmp edf5BDF30Address
edf5BDF30 ENDP

ASMAntAcidColor proc

lea rdx, color
mov rcx, rsi
call edf5BDF30 ; read sgo node
cmp eax, -1
je ShotNumBlock ; if node does not exist, jump to shot count
mov r8, [rsi]
cdqe
movsxd rdx, dword ptr [r8+0Ch]
lea rcx, [rax+rax*2]
lea rax, [r8+rdx]
lea rcx, [rax+rcx*4]
; change ammo color
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+8]
movss dword ptr [rbx+1310h], xmm0
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+20]
movss dword ptr [rbx+1314h], xmm0
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+32]
movss dword ptr [rbx+1318h], xmm0
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+44]
movss dword ptr [rbx+131Ch], xmm0

ShotNumBlock:
lea rdx, shotnum
mov rcx, rsi
call edf5BDF30 ; read sgo node
cmp eax, -1
je OriginalEndBlock ; if node does not exist, jump to end
; read int in node 
mov r8, [rsi]
cdqe
movsxd rdx, dword ptr [r8+0Ch]
lea rcx, [rax+rax*2]
lea rax, [r8+rdx]
mov ecx, [rax+rcx*4+8]
imul ecx, [rbx+13B0h] ; old count * this int
mov [rbx+13B0h], ecx ; rbx+13B0h is shot count

OriginalEndBlock:
add rsp, 0A8h
pop rsi
pop rbx
ret

ASMAntAcidColor ENDP

END