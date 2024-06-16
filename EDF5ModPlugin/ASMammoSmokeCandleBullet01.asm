.data

extern ammoSmokeCandleBullet01justCallVeh : qword
extern ammoSmokeCandleBullet01ofs182168 : qword
extern rva181ED0 : qword

.code

ASMammoSmokeCandleBullet01ofs182060 proc

        cmp dword ptr [rdi+610h], 1 ; SummonType
        jne ofs1820BE ; check is call vehicle
        cmp byte ptr [rdi+0B15h], 0
        jne ofs1820E6 ; chech is just call
    ofs1820BE:
        cmp dword ptr [rax+850h], 0
        je ofs182168 ; check is underground
        cmp ecx, 2
        je ofs182168 ; check summon allow type
    ; do nothing in underground
    ofs182153:
        mov dl, 1
        mov rcx, rdi
        mov rbx, [rsp+50h]
        add rsp, 40h
        pop rdi
        jmp rva181ED0
    ofs1820E6:
        jmp ammoSmokeCandleBullet01justCallVeh
    ofs182168:
        jmp ammoSmokeCandleBullet01ofs182168
        int 3

ASMammoSmokeCandleBullet01ofs182060 ENDP

END