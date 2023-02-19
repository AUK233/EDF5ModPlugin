.data

extern __RTDynamicCastAddr : qword

extern edf11B24E0Address : qword
extern edf11B1AB0Address : qword
extern edf3983B0Address : qword
extern edf3982A0Address : qword

extern ofs3073C0JmpAddr : qword
extern ofs2E4070JmpAddr : qword
extern ofs2E42C0JmpAddr : qword
extern ofs2E43E0JmpAddr : qword
extern ofs2E4500JmpAddr : qword

;fencerBoostSpeed dd 3F800000h

.code

ASMeFencerJetSetup proc

mov edi, r14d
movss dword ptr [rbx+1BB4h], xmm6 ; dash interval
; fix boost speed
mov esi, 1
cvtsi2ss xmm6, esi ; int to float
cmp qword ptr [rbx+1610h], rdi
je toValueBlockFix
mov rax, r14
cmp qword ptr [rbx+1610h], rax
checkCountBlockFix:
jbe addCountBlockFix
lea rcx, qword ptr [rax+rax*2]
mov rax, qword ptr [rbx+1600h]
movsxd rdx, dword ptr [rax+rcx*8]
cmp edx, dword ptr [rbx+15A0h]
jge addCountBlockFix
test edx, edx
js addCountBlockFix
mov rax, qword ptr [rbx+1590h]
mov rcx, qword ptr [rax+rdx*8]
test rcx, rcx
je addCountBlockFix
mov r9, edf11B24E0Address
mov dword ptr [rsp+20h], r14d
mov r8, edf11B1AB0Address
xor edx, edx
call __RTDynamicCastAddr
test rax, rax
je addCountBlockFix
mov r8d, 305 ; get value
lea rdx, qword ptr [rsp+60h]
mov rcx, rax
call edf3982A0Address
test al, al
je addCountBlockFix
cmp esi, dword ptr [rsp+60h]
cmovl esi, dword ptr [rsp+60h]
addCountBlockFix:
movss xmm0, dword ptr [rsp+60h]
maxss xmm0, xmm6
movaps xmm6, xmm0
inc edi
mov eax, edi
cmp qword ptr [rbx+1610h], rax
jne checkCountBlockFix
toValueBlockFix:
mov edi, r14d
movss dword ptr [rbx+1BACh], xmm6 ; boost speed

; dash to boost
mov esi, 0
cmp qword ptr [rbx+1610h], rdi
je toValueBlock1
mov rax, r14
cmp qword ptr [rbx+1610h], rax
checkCountBlock1:
jbe addCountBlock1
lea rcx, qword ptr [rax+rax*2]
mov rax, qword ptr [rbx+1600h]
movsxd rdx, dword ptr [rax+rcx*8]
cmp edx, dword ptr [rbx+15A0h]
jge addCountBlock1
test edx, edx
js addCountBlock1
mov rax, qword ptr [rbx+1590h]
mov rcx, qword ptr [rax+rdx*8]
test rcx, rcx
je addCountBlock1
mov r9, edf11B24E0Address
mov dword ptr [rsp+20h], r14d
mov r8, edf11B1AB0Address
xor edx, edx
call __RTDynamicCastAddr
test rax, rax
je addCountBlock1
mov r8d, 1300 ; get value
lea rdx, qword ptr [rsp+60h]
mov rcx, rax
call edf3983B0Address
test al, al
je addCountBlock1
cmp esi, dword ptr [rsp+60h]
cmovl esi, dword ptr [rsp+60h]
addCountBlock1:
inc edi
mov eax, edi
cmp qword ptr [rbx+1610h], rax
jne checkCountBlock1
toValueBlock1:
mov edi, r14d
mov dword ptr [rbx+1E00h], esi ; dash to boost

; boost to dash
mov esi, 0
cmp qword ptr [rbx+1610h], rdi
je toValueBlock2
mov rax, r14
cmp qword ptr [rbx+1610h], rax
checkCountBlock2:
jbe addCountBlock2
lea rcx, qword ptr [rax+rax*2]
mov rax, qword ptr [rbx+1600h]
movsxd rdx, dword ptr [rax+rcx*8]
cmp edx, dword ptr [rbx+15A0h]
jge addCountBlock2
test edx, edx
js addCountBlock2
mov rax, qword ptr [rbx+1590h]
mov rcx, qword ptr [rax+rdx*8]
test rcx, rcx
je addCountBlock2
mov r9, edf11B24E0Address
mov dword ptr [rsp+20h], r14d
mov r8, edf11B1AB0Address
xor edx, edx
call __RTDynamicCastAddr
test rax, rax
je addCountBlock2
mov r8d, 1301 ; get value
lea rdx, qword ptr [rsp+60h]
mov rcx, rax
call edf3983B0Address
test al, al
je addCountBlock2
cmp esi, dword ptr [rsp+60h]
cmovl esi, dword ptr [rsp+60h]
addCountBlock2:
inc edi
mov eax, edi
cmp qword ptr [rbx+1610h], rax
jne checkCountBlock2
toValueBlock2:
mov dword ptr [rbx+1E04h], esi ; boost to dash

; original
mov rbp, qword ptr [rsp+70h]
mov rbx, qword ptr [rsp+68h]
movaps xmm6, xmmword ptr [rsp+30h]
add rsp, 40h
pop r14
pop rdi
pop rsi
ret 

ASMeFencerJetSetup ENDP

; next - active

ASMeFencerBoostAndDash proc
; original
mov eax, edx
sub eax, 2003h
je checkButtonBlock
sub eax, 1
jne ofs2E3CA6
cmp byte ptr [rcx+1B80h], al
je ofs2E3D10
ofs2E3CA6:
jmp ofs3073C0JmpAddr
; required block
checkButtonBlock:
mov eax, dword ptr [r8] ; it determines trigger type
cmp eax, 1 ; 1 is dash
je toBoostBlock
cmp eax, 0 ; 0 is boost
je toDashBlock
jmp ofs2E3CAB

toBoostBlock:
cmp dword ptr [rcx+1E00h], 1 ; check dash to boost
jne ofs2E3CAB
mov dword ptr [r8], 0 ; 0 is boost
jmp ofs2E3CAB

toDashBlock:
cmp dword ptr [rcx+1E04h], 1 ; check boost to dash
jne ofs2E3CAB
mov dword ptr [r8], 1 ; 1 is dash
jmp ofs2E3CAB
; required block end

ofs2E3CAB:
xorps xmm0, xmm0
comiss xmm0, dword ptr [rcx+1FCh]
jae ofs2E3D10
test dword ptr [rcx+29Ch], -3
jne ofs2E3D10
cmp byte ptr [rcx+1B81h], 0
jne ofs2E3CE1
mov eax, dword ptr [r8]
test eax, eax
je ofs2E4070
cmp eax, 1
jne ofs2E3D10
jmp ofs2E42C0JmpAddr
ofs2E3CE1:
cmp dword ptr [rcx+1B84h], 0
jl ofs2E3CFF
mov eax, dword ptr [r8]
test eax, eax
je ofs2E43E0 ; jne dash = boost, boost invalid
cmp eax, 1
jne ofs2E3D10
jmp ofs2E4500JmpAddr
ofs2E3CFF:
mov byte ptr [rcx+1B82h], 1
mov dword ptr [rcx+1B84h], -1
ofs2E3D10:
xor al, al
ret
ofs2E4070:
jmp ofs2E4070JmpAddr
ofs2E43E0:
jmp ofs2E43E0JmpAddr

ASMeFencerBoostAndDash ENDP

END