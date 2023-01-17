.data
extern WeaponAddress : qword
extern WeaponRetAddress : qword

.code
; It does not work properly
ASMgetWeaponAddress proc

mov byte ptr [rcx+140h],0
mov byte ptr [rcx+0D0h],0
; get weapon address
mov WeaponAddress, rdx
; jump to old
jmp WeaponRetAddress

ASMgetWeaponAddress ENDP
END