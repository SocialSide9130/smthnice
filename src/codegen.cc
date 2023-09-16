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
	printasm(0, ".intel_syntax noprefix");
	printasm(0, ".globl main");
	printasm(0, "main:");
}

void epilogue() {
	printasm(1, "pop rax");
	printasm(1, "ret");
}

void codegen(Node *node) {
	if (node->kind == nNumber) {
		printasm(1, "push %d", node->value);
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