.data
; Use other asm functions
extern edf5BDF30 : proto

; L"BulletAlive"
m501BulletAlive db 66,0,117,0,108,0,108,0,101,0,116,0,65,0,108,0,105,0,118,0,101,0,0,0
; L"BulletColor"
m501BulletColor db 66,0,117,0,108,0,108,0,101,0,116,0,67,0,111,0,108,0,111,0,114,0,0,0
; L"BulletExSet"
m501BulletExSet db 66,0,117,0,108,0,108,0,101,0,116,0,69,0,120,0,83,0,101,0,116,0,0,0

.code

ASMxgsOCmonster501 proc
;
;mov dword ptr [rbx+1634h], 41000000h
;mov dword ptr [rbx+1654h], 40800000h
;mov dword ptr [rbx+163Ch], 42C80000h

mov dword ptr [rbx+167Ch], 3F800000h

lea rdx, m501BulletAlive
lea rcx, qword ptr [rbx+0B0h]
call edf5BDF30 ; read sgo node
movsxd rcx, eax
cmp ecx, -1
jne AmmoAliveBlock
mov rax, rbp
jmp AmmoColorBlock ; if node does not exist, jump
AmmoAliveBlock:
mov rax, qword ptr [rbx+0B0h]
movsxd rdx, dword ptr [rax+0Ch]
add rdx, rax
lea rcx, [rcx+rcx*2]
mov eax, [rdx+rcx*4+8]
mov [rbx+1638h], eax ; AmmoAlive

AmmoColorBlock:
lea rdx, m501BulletColor
lea rcx, qword ptr [rbx+0B0h]
call edf5BDF30 ; read sgo node
movsxd rcx, eax
cmp ecx, -1
jne AmmoColorBlock2
mov rax, rbp
jmp AmmoExSetBlock ; if node does not exist, jump
AmmoColorBlock2:
mov rax, qword ptr [rbx+0B0h]
movsxd rdx, dword ptr [rax+0Ch]
add rdx, rax
lea rcx, [rcx+rcx*2]
lea rcx, [rdx+rcx*4] ; store ptr address
; change ammo color
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+8]
movss dword ptr [rbx+1670h], xmm0 ;r
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+20]
movss dword ptr [rbx+1674h], xmm0 ;g
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+32]
movss dword ptr [rbx+1678h], xmm0 ;b
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+44]
movss dword ptr [rbx+167Ch], xmm0 ;a

AmmoExSetBlock:
lea rdx, m501BulletExSet
lea rcx, qword ptr [rbx+0B0h]
call edf5BDF30 ; read sgo node
movsxd rcx, eax
cmp ecx, -1
jne AmmoExSetBlock2
mov rax, rbp
jmp OriginalEndBlock ; if node does not exist, jump
AmmoExSetBlock2:
mov rax, qword ptr [rbx+0B0h]
movsxd rdx, dword ptr [rax+0Ch]
add rdx, rax
lea rcx, [rcx+rcx*2]
lea rcx, [rdx+rcx*4] ; store ptr address
; change shot count
movsxd rax, dword ptr [rcx+8]
mov eax, [rax+rcx+8]
mov [rbx+1730h], eax
; change ammo speed
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+20]
movss dword ptr [rbx+1634h], xmm0
; change ammo damage
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+32]
movss dword ptr [rbx+163Ch], xmm0
; change ammo explosion range
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+44]
movss dword ptr [rbx+1648h], xmm0
; change ammo damage
movsxd rax, dword ptr [rcx+8]
lea rax, [rax+rcx+48] ; store address
cmp dword ptr [rax+8],0
setne al
mov byte ptr [rbx+164Ch], al
; change ammo size
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+68]
movss dword ptr [rbx+1654h], xmm0
; change ammo hit size
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+80]
movss dword ptr [rbx+1658h], xmm0
; change ammo hit impulse
movsxd rax, dword ptr [rcx+8]
movss xmm0, dword ptr [rax+rcx+92]
movss dword ptr [rbx+165Ch], xmm0

OriginalEndBlock:
mov rax, rbx
mov rbx, qword ptr [rsp+78h]
add rsp, 50h
pop rdi
pop rsi
pop rbp
ret 

ASMxgsOCmonster501 endp

END