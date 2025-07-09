.data

extern Initialize_NGX_dlss : proto
extern InitializeDLSS : proto
extern Release_NGX_dlss : proto
extern Evaluate_NGX_dlss : proto

extern dx11CreateDeviceRetAddr : qword
extern dx11EvaluateInMissionRetAddr : qword

.code

ASMdx11CreateDevice proc

    ; mov rcx, [rbp-69h]
    ; mov rax, [rcx]
    ; lea rdx, [rbp-61h]
    ; call qword ptr [rax+38h]
    ; mov rcx, [rbp-61h] ; ID3D11Device*
    ; call InitializeDLSS
    ; mov rcx, [rbp-61h]
    ; jmp dx11CreateDeviceRetAddr
    xor r9d, r9d
    xor r8d, r8d
    xor ecx, ecx
    call Initialize_NGX_dlss
    mov esi, eax
    jmp dx11CreateDeviceRetAddr
    int 3

ASMdx11CreateDevice ENDP

align 16

ASMsysExitGame proc

    add rsp, 0B0h
    pop rdi
    pop rsi
    pop rbp
    jmp Release_NGX_dlss
    int 3

ASMsysExitGame ENDP

align 16

ASMdx11EvaluateInMission proc

    mov rdx, [r13+0B8h] ; ID3D11Device*
    mov rcx, [r13+0B8h+8] ; ID3D11DeviceContext*
    call Evaluate_NGX_dlss
    mov rcx, [r13+0B8h] ; ID3D11Device*
    mov rax, dx11EvaluateInMissionRetAddr
    lea r9, [rbp-20h]
    push rax
    mov rdx, [rbp+38h]
    lea r8, [rbp-18h]
    mov rax, [rcx]
    jmp qword ptr [rax+38h]
    int 3

ASMdx11EvaluateInMission ENDP

END