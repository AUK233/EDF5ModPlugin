.data

extern vftable_SolidPelletBullet01 : qword
extern rva186330 : qword

.code

ASMreadSolidPelletBullet01 proc
    ; Be aware of EDF5.exe+18579D
        mov rdi, [rbx+3A8h]
        movsxd rdx, dword ptr [rdi+8]
        mov eax, [rdx+rdi+8]
        mov [rbx+710h], eax ; can penetration time

        cmp dword ptr [rdi+4], 3 ; this is 4th node
        jle noColorChange ; so if <= 3, jump
        
        mov eax, [rdx+rdi+36+8]
        test eax, eax
        je noColorChange ; = 0 no colour change
        mov dword ptr [rbx+5D0h], 10
        ;
        lea r8, vftable_SolidPelletBullet01+8 ; load new virtual table
        mov [rbx], r8
        ;
        lea rcx, [rdx+rdi+36]
        cmp dword ptr [rcx], 1 ; check node type
        jne ptrNode
        movaps xmm0, [rbx+390h]
        shufps xmm0, xmm0, 11000110b ;3012 high to low bit
        movaps [rbx+720h], xmm0
        jmp ofs18489A
    ptrNode:
        cdqe ; eax => rax
        add rcx, rax
        mov eax, [rcx+8]
        mov [rbx+720h], eax
        mov eax, [rcx+12+8]
        mov [rbx+724h], eax
        mov eax, [rcx+24+8]
        mov [rbx+728h], eax
        mov eax, [rcx+36+8]
        mov [rbx+72Ch], eax
        jmp ofs18489A

    noColorChange:
        mov dword ptr [rbx+5D0h], 0
    ofs18489A:
        mov rbx, [rsp+40h]
        add rsp, 30h
        pop rdi
        ret  
        int 3

ASMreadSolidPelletBullet01 ENDP

align 16

ASMammoSolidPelletBullet01CheckPT proc

        push rbx
        sub rsp, 20h
        mov rbx, rcx
        call rva186330
        mov edx, [rbx+710h]
        lea eax, [rdx-1]
        mov [rbx+710h], eax
        test edx, edx ; remain time
        jg ofs185C98
        xor eax, eax
        mov byte ptr [rbx+41Ah], al ; set no penetration
        ; check color
        cmp dword ptr [rbx+5D0h], eax
        je ofs185C98
        movaps xmm0, [rbx+720h]
        movaps [rbx+390h], xmm0
        mov dword ptr [rbx+5D0h], eax
    ofs185C98:
        add rsp, 20h
        pop rbx
        ret 
        int 3

ASMammoSolidPelletBullet01CheckPT ENDP

END