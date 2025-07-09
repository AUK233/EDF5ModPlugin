.data

extern ASMgetCurrentMissionClassModelType : proto


extern huiObjectClassSGOName : qword
extern edf5BDF30Address : qword

extern huiCommonUIClassFuncP8RetAddr : qword

.code

ASMhuiCommonUIClassFuncP8 proc

		call ASMgetCurrentMissionClassModelType
		cmp eax, 3
		ja ofs4AD617
		lea rcx, huiObjectClassSGOName
		mov rdx, [rcx+rax*8]
		lea rcx, [rdi+28h]
		call edf5BDF30Address
		cmp eax, -1 ; check return value exist
		jne ofs4AD62A
	ofs4AD617:
		mov rdx, huiObjectClassSGOName
		lea rcx, [rdi+28h]
		call edf5BDF30Address
	ofs4AD62A:
		movsxd rcx, eax
		jmp huiCommonUIClassFuncP8RetAddr
        int 3

ASMhuiCommonUIClassFuncP8 ENDP

END
