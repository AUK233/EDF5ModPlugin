.data
; Use other asm functions
extern edf5BDF30 : proto

extern edf5BDF30Address : qword
extern _CommonRVA_EE7460 : xmmword

extern playerViewIndex : dword
extern playerViewRetAddr : qword

; old data pointer offset
GOCOffset dd 0
; data pointer address
GOCofsAddr dq 0
align 16
; L"game_object_camera_setting"
GOCsetting db 103,0,97,0,109,0,101,0,95,0,111,0,98,0,106,0,101,0,99,0,116,0,95,0,99,0
db 97,0,109,0,101,0,114,0,97,0,95,0,115,0,101,0,116,0,116,0,105,0,110,0,103,0,0,0

.code

ASMplayerViewChange proc

        cmp qword ptr [rcx+0B0h], 0
        je ofs2DA5FE
        push rbx
        push rsi
        push rdi
        sub rsp, 30h
        mov rbx, rcx
        xor esi,esi
        ; initialize viewpoint
        movaps xmm0, _CommonRVA_EE7460
        movaps xmm1, xmm0
        movups [rbx+100h], xmm0
        movups [rbx+110h], xmm1
        mov dword ptr [rbx+120h], 3F800000h
        ; read sgo
        add rcx, 0B0h
        lea rdx, GOCsetting
        call edf5BDF30Address
        cmp eax, -1
        je ofs2DA5DF ; if it doesn't exist
        ; read sgo node
        mov r8, [rbx+0B0h]
        cdqe 
        movsxd rdx, dword ptr [r8+12]
        lea rcx, [rax+rax*2]
        lea rax, [r8+rdx]
        lea rdi, [rax+rcx*4]
        movsxd rsi, dword ptr [rdi+8]
        add rsi, rdi
        mov edx, [rdi+4]
        ; check node3
        cmp edx, 3
        jle readViewNode
        mov ecx, [rsi+2Ch]
        mov [rbx+120h], ecx
    readViewNode:
        ; check data amount
        mov eax, playerViewIndex
        test eax, eax
        je ofs2DA4D5
        cmp eax, edx
        jge ofs2DA4D5 ; jump back to original when less than needed
        cdqe ; eax=>rax
        imul rax, 12
        add rsi, rax

    ofs2DA4D5:
        mov ecx, [rsi+4]
        test ecx, ecx
        je ofs2DA549
        movsxd rax, dword ptr [rsi+8]
        lea rdx, [rax+rsi+8]
        mov eax, [rdx]
        mov [rbx+100h], eax
        ; copy more
        cmp ecx, 1
        jle ofs2DA549
        mov eax, [rdx+12]
        mov [rbx+100h+4], eax
        cmp ecx, 2
        jle ofs2DA549
        mov eax, [rdx+24]
        mov [rbx+100h+8], eax
        cmp ecx, 3
        jle ofs2DA549
        mov eax, [rdx+36]
        mov [rbx+100h+12], eax

    ofs2DA549:
        mov ecx, [rsi+12+4]
        test ecx, ecx
        je ofs2DA5DF
        movsxd rax, dword ptr [rsi+12+8]
        lea rdx, [rax+rsi+12+8]
        mov eax, [rdx]
        mov [rbx+110h], eax
        ; copy more
        cmp ecx, 1
        jle ofs2DA5DF
        mov eax, [rdx+12]
        mov [rbx+110h+4], eax
        cmp ecx, 2
        jle ofs2DA5DF
        mov eax, [rdx+24]
        mov [rbx+110h+8], eax
        cmp ecx, 3
        jle ofs2DA5DF
        mov eax, [rdx+36]
        mov [rbx+110h+12], eax

    ofs2DA5DF:
        mov byte ptr [rbx+140h], 0
        mov byte ptr [rbx+124h], 0
        mov dword ptr [rbx+160h], 3F800000h
        mov byte ptr [rbx+170h], 0
        add rsp, 30h
        pop rdi
        pop rsi
        pop rbx
    ofs2DA5FE:
        ret 
        int 3

ASMplayerViewChange ENDP

ASMplayerViewChange1 proc

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
        add eax, -3
        cmp eax, playerViewIndex
        jg CheckExistBlock
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
        int 3

ASMplayerViewChange1 ENDP

END