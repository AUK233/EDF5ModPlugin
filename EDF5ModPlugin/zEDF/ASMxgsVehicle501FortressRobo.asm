.data

extern rva27380 : qword
extern rva344F30 : qword
extern rva27570 : qword
extern rva9C6E40 : qword

; L"balam_cannon"
v501MABbalamCannon db 98,0,97,0,108,0,97,0,109,0,95,0,99,0,97,0,110,0,110,0,111,0,110,0,0,0

.code

ASMVehicle501AEWeaponFire proc

        push rsi
        mov r9, [rcx+450h] ; get weapon num
        mov rsi, [rcx+448h] ; get weapon pointer's pointer
        xor rdx, rdx ; set weapon index
    getWeapon:
        lea r8, [rdx+rdx*8]
        mov rax, [rsi+r8*8+10h]
        test rax, rax
        je returnAddress
        mov byte ptr [rax+0D9h], 1
    nextWeapon:
        inc rdx
        cmp rdx, r9
        jl getWeapon
        ; Index must be less than total
    returnAddress:
        pop rsi
        ret 
        int 3

ASMVehicle501AEWeaponFire ENDP

align 16

ASMVehicle501AnimationEvent proc
        lea rax, ASMVehicle501AEWeaponFire
        mov [rbp-30h], rax
        mov [rbp-28h], r15d
        mov qword ptr [rbp-40h], 7
        mov [rbp-48h], r15
        mov [rbp-58h], r15w
        mov r8d, 12 ; string length
        lea rdx, v501MABbalamCannon
        lea rcx, [rbp-58h]
        call rva27380
        movaps xmm0, [rbp-30h]
        movaps [rbp-30h], xmm0
        lea r9, [rbp-30h]
        mov r8, rdi
        lea rdx, [rbp-58h]
        mov rcx, rsi
        call rva344F30
        mov rdx, [rbp-40h]
        cmp rdx, 8
        jb ofs33C400
        inc rdx
        mov r8d, 2
        mov rcx, [rbp-58h]
        call rva27570

    ofs33C400:
        mov rcx, [rbp-10h]
        xor rcx, rsp
        call rva9C6E40
        lea r11, [rsp+80h]
        mov rbx, [r11+28h]
        mov rsi, [r11+30h]
        mov rdi, [r11+38h]
        mov rsp, r11
        pop r15
        pop r14
        pop rbp
        ret 
        int 3

ASMVehicle501AnimationEvent ENDP

END
