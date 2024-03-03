.data

extern readMissileBullet01RetAddr : qword
extern ammoMissileBullet01BlastRetAddr : qword
extern edf1AE7A0Address : qword
extern edf1AD970Address : qword
extern vedf125ABA0 : qword

extern _Common_F70P0 : dword
extern _Common_F5P0 : dword
extern _CommonRVA_EE7550 : xmmword

.code

ASMreadMissileBullet01 proc

        movsxd rax, dword ptr [rbx+8]
        mov ecx, [rax+rbx+8]
        mov [rdi+0AA0h], ecx ; missile type
        cmp ecx, 2
        jle newBlock
        cmp ecx, 4
        jg newBlock
        or word ptr [rdi+20Ch], 8 ; no explosion destruction FX

    newBlock:
        mov ecx, [rax+rbx+12+8]
        cmp ecx, 10
        jne ofs160306
        mov dword ptr [rdi+400h], 1 ; no explosive knockout

    ofs160306:
        movsxd rax, dword ptr [rbx+8]
        mov r13d, [rax+rbx+24+8]

        lea rdx, [rbx+24h]
        add rdx, rax
        jmp readMissileBullet01RetAddr
        int 3

ASMreadMissileBullet01 ENDP

align 16

; set explosion fx
ASMammoMissileBullet01Blast proc
        mov rcx, vedf125ABA0
        mov dword ptr [rsp+20h], 3F800000h
        mov eax, [rbx+0AA0h] ; check type
        cmp eax, 3
        je type3
        cmp eax, 4
        je type4
        
        mov rdx, qword ptr [rbx+148h]
        movss xmm3, dword ptr [rbx+368h]
        divss xmm3, _Common_F5P0
        mov rcx, [rcx]
        lea r8, qword ptr [rdx+20h]
        add rdx, 10h
        call edf1AD970Address
        jmp ammoMissileBullet01BlastRetAddr

    type3:
        mov rdx, qword ptr [rbx+148h]
        movss xmm3, dword ptr [rbx+368h]
        divss xmm3, _Common_F70P0
        mov rcx, [rcx]
        lea r8, qword ptr [rdx+20h]
        add rdx, 10h
        call edf1AE7A0Address
        jmp ammoMissileBullet01BlastRetAddr

    type4:
        mov rdx, qword ptr [rbx+148h]
        movss xmm3, dword ptr [rbx+368h]
        divss xmm3, _Common_F70P0
        mov rcx, [rcx]
        lea r8, qword ptr [rdx+20h]
        add rdx, 10h
        movups xmm0, _CommonRVA_EE7550
        movups [r8], xmm0
        call edf1AE7A0Address
        jmp ammoMissileBullet01BlastRetAddr
        int 3

ASMammoMissileBullet01Blast ENDP

END