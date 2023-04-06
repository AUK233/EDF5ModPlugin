.data

extern ammoMissileBullet01BlastRetAddr : qword
extern edf1AE7A0Address : qword
extern edf1AD970Address : qword
extern vedf125ABA0 : qword

extern _Common_F70P0 : dword
extern _Common_F5P0 : dword
extern _CommonRVA_EE7550 : xmmword

.code

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