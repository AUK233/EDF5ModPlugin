.data
; Use other asm functions
extern edf5BDF30 : proto

extern playerViewIndex : dword
extern playerViewRetAddr : qword

; old data pointer offset
GOCOffset DD 0
; data pointer address
GOCofsAddr DQ 0

; L"game_object_camera_setting"
GOCsetting db 103,0,97,0,109,0,101,0,95,0,111,0,98,0,106,0,101,0,99,0,116,0,95,0,99,0
db 97,0,109,0,101,0,114,0,97,0,95,0,115,0,101,0,116,0,116,0,105,0,110,0,103,0,0,0

.code

ASMplayerViewChange proc

mov qword ptr [rsp+48h], rsi
mov qword ptr [rsp+50h], r14
add rcx, 0B0h
lea rdx, GOCsetting
call edf5BDF30
xor esi,esi
cmp eax, -1
jne OriginalBlock
mov edi,esi
jmp playerViewRetAddr

OriginalBlock:
mov r8, qword ptr [rbx+0B0h]
cdqe 
movsxd rdx, dword ptr [r8+0Ch]
lea rcx, qword ptr [rax+rax*2]
lea rax, qword ptr [r8+rdx]
lea rdi, qword ptr [rax+rcx*4] ; storage address
; check data amount
mov eax, dword ptr [rdi+4]
sub eax, 4
cmp eax, playerViewIndex
jge CheckExistBlock
; jump back to original when less than needed
jmp playerViewRetAddr

; check if pointer address has changed
CheckExistBlock:
mov rax, rdi
cmp GOCofsAddr, rax
je AddptrBlock
mov GOCofsAddr, rax
mov eax, dword ptr [rdi+8]
mov GOCOffset, eax
; increase pointer offset to what we need
AddptrBlock:
mov eax, 12
imul eax, playerViewIndex
add eax, GOCOffset
mov dword ptr [rdi+8], eax
jmp playerViewRetAddr

ASMplayerViewChange ENDP

END