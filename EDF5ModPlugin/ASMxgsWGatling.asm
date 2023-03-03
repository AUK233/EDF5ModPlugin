.data

extern wGatlingSetupRetAddr : qword
; this address allows weapon fire
extern wGatlingShotRetAddr : qword

.code

ASMweaponGatlingSetup proc
        ; original
        mov dword ptr [rbx+11D4h], 3E800000h
        mov dword ptr [rbx+11D8h], 3D800000h
        mov byte ptr [rbx+11F0h], 0

        ; new, set pre-heat type
        ; initialize memory
        mov dword ptr [rbx+1400h], 0
        mov ecx, dword ptr [rbx+2FCh] ; store original FireInterval
        mov dword ptr [rbx+1404h], ecx
        mov dword ptr [rbx+1410h], 1
        movss xmm0, dword ptr [rbx+308h] ; store original FireAccuracy
        movss dword ptr [rbx+1414h], xmm0
        ; store default rate
        mov dword ptr [rbx+1408h], 3F800000h ; max FireInterval
        mov dword ptr [rbx+140Ch], 3F800000h ; min FireInterval
        mov dword ptr [rbx+1418h], 3F800000h ; max FireAccuracy
        mov dword ptr [rbx+141Ch], 3F800000h ; min FireAccuracy
        ; check node
        mov eax, dword ptr [rsi+4]
        cmp eax, 11
        jl ofs39A0E0 ; If it exists, read node9 and node10

        ; node9, pre-heat rate
        movsxd rdx, dword ptr [rsi+8]
        add rdx, rsi
        add rdx, 108 ; 12 bytes per node
        movsxd rcx, dword ptr [rdx+8] ; get ptr offset
        ; get int
        mov eax, dword ptr [rdx+rcx+8]
        mov dword ptr [rbx+1410h], eax ; get startup delay
        ; get float
        movss xmm0, dword ptr [rdx+rcx+20]
        movss dword ptr [rbx+1408h], xmm0 ; max FireInterval
        movss xmm0, dword ptr [rdx+rcx+32]
        movss dword ptr [rbx+140Ch], xmm0 ; min FireInterval
        movss xmm0, dword ptr [rdx+rcx+44]
        movss dword ptr [rbx+1418h], xmm0 ; max FireAccuracy
        movss xmm0, dword ptr [rdx+rcx+56]
        movss dword ptr [rbx+141Ch], xmm0 ; min FireAccuracy

        ; node10, pre-heat type
        movsxd rax, dword ptr [rsi+8]
        mov ecx, dword ptr [rax+rsi+128] ; get int
        mov dword ptr [rbx+1400h], ecx ; set pre-heat type
    ; return original
    ofs39A0E0:
        jmp wGatlingSetupRetAddr
        int 3

ASMweaponGatlingSetup ENDP

; next

ASMweaponGatlingShot proc
        ; check preheat type
        cmp dword ptr [rcx+1400h], 0
        je ofs39A7AA
        ; check other
        cmp dword ptr [rcx+8E8h], 0 ; check ammo
        je returnBlock
        mov eax, dword ptr [rbx+1410h]
        cmp dword ptr [rcx+11A4h], eax ; check delay count
        jle returnBlock
    ; decision to modify ROF
    checkROFBlock:
        mov eax, dword ptr [rcx+11A8h] ; total delay
        cmp dword ptr [rcx+11A4h], eax ; delay count
        jl setROFBlock ; delay count < total delay
        mov eax, dword ptr [rcx+1404h] ; get FireInterval
        mov dword ptr [rcx+2FCh], eax ; set ROF is FireInterval
        movss xmm0, dword ptr [rbx+1414h] ; get FireAccuracy
        movss dword ptr [rbx+308h], xmm0 ; set FireAccuracy
        jmp wGatlingShotRetAddr
    setROFBlock:
        cvtsi2ss xmm0, dword ptr [rcx+11A4h] ; delay count
        cvtsi2ss xmm1, eax ; total delay
        movss xmm2, xmm1 ; backup total delay
        subss xmm1, xmm0 ; total delay - delay count
        divss xmm1, xmm2 ; / total delay
        movss xmm2, xmm1 ; backup result
        mulss xmm1, dword ptr [rcx+1408h] ; * max rate
        maxss xmm1, dword ptr [rcx+140Ch] ; fmaxf(min, current)
        mov eax, dword ptr [rcx+1404h] ; get FireInterval
        cvtsi2ss xmm0, eax ; ROF to float
        mulss xmm0, xmm1 ; * ROF
        cvttss2si eax, xmm0 ; float to int
        ;shr eax, 1 ; / 2
        mov dword ptr [rcx+2FCh], eax ; to ROF
        ; check set accuracy
        cmp dword ptr [rcx+1400h], 2
        je setAccuracyBlock
        jmp wGatlingShotRetAddr
    setAccuracyBlock:
        movss xmm0, xmm2 ; get result
        mulss xmm0, dword ptr [rcx+1418h] ; * max rate
        maxss xmm0, dword ptr [rcx+141Ch] ; fmaxf(min, current)
        mulss xmm0, dword ptr [rcx+1414h] ; * accuracy
        movss dword ptr [rbx+308h], xmm0 ; set FireAccuracy
        jmp wGatlingShotRetAddr
    ; original
    ofs39A7AA:
        mov eax, dword ptr [rcx+11A8h] ; total delay
        cmp dword ptr [rcx+11A4h], eax ; delay count
        jl returnBlock
        jmp wGatlingShotRetAddr
    ; return
    returnBlock:
        add rsp, 38h
        ret 

ASMweaponGatlingShot ENDP

END