global main
extern scanf
extern printf
extern exit

SECTION .data
	format db "%ld", 0
	a dq 0h
	b dq 0h
	r dq 0h


SECTION .text
main:

	push rdi
	push rsi
	push rbp
	mov rsi, a
	mov rdi, format
	call scanf
	pop rbp
	pop rsi
	pop rdi



	push rdi
	push rsi
	push rbp
	mov rsi, b
	mov rdi, format
	call scanf
	pop rbp
	pop rsi
	pop rdi


	mov rcx, [a]
	mov rax, 0
	cmp rcx, rax
	jne else_branch0

	mov rax, [b]

	push rdi
	push rsi
	push rbp
	mov rsi, rax
	mov rdi, format
	call printf
	pop rbp
	pop rsi
	pop rdi

	jmp after_else0
else_branch0:
	mov rcx, [b]
	mov rax, 0
	cmp rcx, rax
	jne else_branch1

	mov rax, [a]

	push rdi
	push rsi
	push rbp
	mov rsi, rax
	mov rdi, format
	call printf
	pop rbp
	pop rsi
	pop rdi

	jmp after_else1
else_branch1:
before_while0:
	mov rcx, [b]
	mov rax, 0
	cmp rcx, rax
	je while_end0

	mov rax, [a]
	mov rbx, [b]
	mov rdx, 0
	div rbx
	mov rax, rdx
	mov [r], rax
	mov rax, [b]
	mov [a], rax
	mov rax, [r]
	mov [b], rax
jmp before_while0
while_end0:

	mov rax, [a]

	push rdi
	push rsi
	push rbp
	mov rsi, rax
	mov rdi, format
	call printf
	pop rbp
	pop rsi
	pop rdi

after_else1:

after_else0:

push 0
call exit
