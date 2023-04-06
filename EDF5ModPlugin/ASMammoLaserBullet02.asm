.data

extern ammoLaserBullet02RetAddr : qword
extern ammoLaserBullet02BlastRetAddr : qword
extern edf47D950Address : qword
extern edf1AE7A0Address : qword
extern edf1AD970Address : qword
extern vedf125ABA0 : qword

extern _Common_F70P0 : dword
extern _Common_F5P0 : dword
extern _CommonRVA_EE7550 : xmmword

.code

; set delayed explosion
ASMammoLaserBullet02 proc
        mov eax, [rsi+5D0h]
    ; type = 2
        cmp eax, 2
        jne type4
        or word ptr [rsi+20Ch], 8
        mov dword ptr [rsi+404h], 3
        jmp ammoLaserBullet02RetAddr
    type4:
        cmp eax, 4
        jne ofs15669D
        or word ptr [rsi+20Ch], 8 ; no explosion destruction FX
        mov dword ptr [rsi+404h], 3 ; delayed damage
    ofs15669D:
        jmp ammoLaserBullet02RetAddr
        int 3

ASMammoLaserBullet02 ENDP

; set delayed explosion fx
ASMammoLaserBullet02Blast proc
        mov eax, [rsi+5D0h]
    ; type = 2
        cmp eax, 2
        jne type4
        cmp byte ptr [rsi+5D8h], 0
        je ofs157318

        lea rcx, qword ptr [rsi+3B0h]
        xor r8d, r8d
        lea rdx, qword ptr [rsi+670h]
        call edf47D950Address
        ; rsi+390h is color r
        movss xmm3, dword ptr [rsi+368h]
        lea r8, qword ptr [rsp+30h]
        divss xmm3, _Common_F70P0
        mov rcx, vedf125ABA0
        lea rdx, qword ptr [rsi+670h]
        mov rcx, [rcx]
        movups xmm0, _CommonRVA_EE7550
        movss dword ptr [rsp+20h], xmm9
        movups [rsp+30h], xmm0
        call edf1AE7A0Address
        jmp ammoLaserBullet02BlastRetAddr
    
    type4:
        cmp eax, 4
        jne ofs157318
        cmp byte ptr [rsi+5D8h], 0
        je ofs157318

        lea rcx, qword ptr [rsi+3B0h]
        xor r8d, r8d
        lea rdx, qword ptr [rsi+670h]
        call edf47D950Address

        movss xmm3, dword ptr [rsi+368h]
        lea r8, dword ptr [rsi+680h] ; set fx angle
        divss xmm3, _Common_F5P0
        mov rcx, vedf125ABA0
        lea rdx, qword ptr [rsi+670h]
        mov rcx, [rcx]
        movss dword ptr [rsp+20h], xmm9
        call edf1AD970Address

    ofs157318:
        jmp ammoLaserBullet02BlastRetAddr
        int 3

ASMammoLaserBullet02Blast ENDP

END