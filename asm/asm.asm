.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	includelib Library.lib
	ExitProcess PROTO :DWORD

	printi PROTO: DWORD
	prints PROTO: DWORD
	len PROTO: DWORD
	pows PROTO: DWORD, :DWORD

.stack 4096
.const
	L0 DWORD 2
	L1 DWORD 59
	L2 DWORD 17
	L3 DWORD 9
	L4 BYTE "My name is Diana", 0
	L5 DWORD 6
	L6 BYTE "Hello World!!!", 0
.data
	onec DWORD ?
	twoe DWORD ?
	threez DWORD ?
	maine DWORD ?
	mainf DWORD ?
	maing DWORD ?
	mainresult DWORD ?

.code

one PROC onea : SDWORD, oneb : SDWORD
	push onea
	push oneb
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L0
	pop eax
	pop ebx
	mul ebx
	push eax
	pop onec
	push onec
		jmp local0
local0:
	pop eax
	ret
one ENDP

two PROC twod : DWORD
	push twod
	pop edx
	push twod
		call len
	push eax
	pop twoe
	push twoe
		jmp local1
local1:
	pop eax
	ret
two ENDP

three PROC threex : SDWORD, threey : SDWORD
	push threex
	push threey
	pop edx
	pop edx
	push threey
	push threex
		call pows
	push eax
	pop threez
	push threez
		jmp local2
local2:
	pop eax
	ret
three ENDP

main PROC
	push L1
	pop mainf
	push L0
	push L2
	pop eax
	pop ebx
	mul ebx
	push eax
	push L3
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push maine
	push mainf
	pop edx
	pop edx
	push mainf
	push maine
		call one
	push eax
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop maine
	push maine
	push mainf
	pop edx
	pop edx
	push mainf
	push maine
		call one
	push eax
	pop maing
	mov eax, maine
	cmp eax, maing
		jl m0
		jg m1
		je m1
m0:
	push maine
		call printi
	jmp e0
m1:
	push maing
		call printi
e0:
	push maing
		call printi
	push offset L4
	pop mainresult
	push mainresult
	pop edx
	push mainresult
		call two
	push eax
	pop maing
	push maing
		call printi
	push L3
	push L5
	pop edx
	pop edx
	push L5
	push L3
		call three
	push eax
	pop maing
	push maing
		call printi
	push offset L6
		call prints
	call ExitProcess
main ENDP
end main