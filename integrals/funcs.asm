global f1
global f2
global f3
global df1
global df2
global df3

f1:
	push ebp
	mov ebp, esp
	push 30
	fld qword[ebp + 8]
	push -1
	fild dword[esp]
	add esp, 4
	faddp
	fld qword[ebp + 8]
	push 1
	fild dword[esp]
	add esp, 4
	faddp
	fdivp
	sub esp, 8
	fst qword[esp]
	sub esp, 8
	fstp qword[esp]
	push 1
	sub esp, 8
	mov dword[esp + 4], 0
	mov dword[esp], 0
	mov edx, 1
.for:
	cmp edx, [esp + 28]
	jge .break_for
	fld qword[esp + 12]
	fild dword[esp + 8]
	fdivp
	fld qword[esp]
	faddp
	fstp qword[esp]
	add dword[esp + 8], 2
	fld qword[esp + 12]
	fld qword[esp + 20]
	fmulp
	fld qword[esp + 20]
	fmulp
	fstp qword[esp + 12]
	inc edx
	jmp .for
.break_for:
	fld qword[esp]
	push 2
	fild dword[esp]
	add esp, 4
	fmulp

	leave
	ret


f2:
	push ebp
	mov ebp, esp

	push -2
	fild dword[esp]
	fld qword[ebp + 8]
	fmulp
	push 14
	fild dword[esp]
	faddp

	leave
	ret

f3:
	push ebp
	mov ebp, esp

	fld1
	fld qword[ebp + 8]
	push -1
	fild dword[esp]
	fmulp
	push 2
	fild dword[esp]
	faddp
	fdivp
	push 6
	fild dword[esp]
	faddp

	leave
	ret


df1:
	fld1
	fld qword[esp + 4]
	fdivp
	ret

df2:
	push -2
	fild dword[esp]
	add esp, 4
	ret

df3:
	push ebp
	mov ebp, esp

	push 1
	fild dword[esp]

	fld qword[ebp + 8]
	push -1
	fild dword[esp]
	fmulp
	add esp, 4
	push 2
	fild dword[esp]
	faddp
	add esp, 4
	sub esp, 8
	fst qword[esp]
	fld qword[esp]
	fmulp
	add esp, 8

	fdivp

	leave
	ret