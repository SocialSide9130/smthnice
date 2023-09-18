#include <stdio.h>
#include <stdarg.h>

#include "codegen.hh"

void printasm(int tabs, const char *format, ...) {
	va_list args;
	
	for (int i = 0; i < tabs; ++i)
		printf("\t");
	
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

void prologue() {
	
}

void epilogue() {
	printasm(1, "pop rax");
	printasm(1, "ret");
}

void gen_leftvalue(Node *node) {
	printasm(1, "mov rax, rbp");
	printasm(1, "sub rax, %d", node->offset);
	printasm(1, "push rax");
}

void codegen(Node *node) {
	switch (node->kind) {
	case nNumber:
		printasm(1, "push %d", node->value);
		return;
	case nLocalVariable:
		gen_leftvalue(node);
		printasm(1, "pop rax");
		printasm(1, "mov rax, [rax]");
		printasm(1, "push rax");
		return;
	case nAssign:
		gen_leftvalue(node->lhs);
		codegen(node->rhs);

		printasm(1, "pop rdi");
		printasm(1, "pop rax");
		printasm(1, "mov [rax], rdi");
		printasm(1, "push rdi");
		return;
	}

	if (node->kind == nReturn) {
		codegen(node->lhs);
		printasm(1, "pop rax");
		printasm(1, "mov rsp, rbp");
		printasm(1, "pop rbp");
		printasm(1, "ret");
		return;
	}

	codegen(node->lhs);
	codegen(node->rhs);

	printasm(1, "pop rdi");
	printasm(1, "pop rax");

	switch (node->kind) {
	case nAdd:
		printasm(1, "add rax, rdi");
		break;
	case nSub:
		printasm(1, "sub rax, rdi");
		break;
	case nMul:
		printasm(1, "imul rax, rdi");
		break;
	case nDiv:
		printasm(1, "cqo");
		printasm(1, "idiv rdi");
		break;
	case nEqual:
		printasm(1, "cmp rax, rdi");
		printasm(1, "sete al");
		printasm(1, "movzb rax, al");
		break;
	case nNotEqual:
		printasm(1, "cmp rax, rdi");
		printasm(1, "setne al");
		printasm(1, "movzb rax, al");
		break;
	case nLess:
		printasm(1, "cmp rax, rdi");
		printasm(1, "setl al");
		printasm(1, "movzb rax, al");
		break;
	case nLessEqual:
		printasm(1, "cmp rax, rdi");
		printasm(1, "setle al");
		printasm(1, "movzb rax, al");
		break;
	default:
		break;
	}
	printasm(1, "push rax");
}