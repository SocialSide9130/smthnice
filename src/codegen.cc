#include <stdio.h>
#include <stdarg.h>

#include "codegen.hh"

int label_number;

Function *functions_list;

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
	printasm(1, "push rbp");
	printasm(1, "mov rbp, rsp");	
}

void epilogue() {
	printasm(1, "mov rsp, rbp");
	printasm(1, "pop rbp");
	printasm(1, "pop rax");
	printasm(1, "ret");
}

void gen_leftvalue(Node *node) {
	printasm(1, "mov rax, rbp");
	printasm(1, "sub rax, %d", node->offset);
	printasm(1, "push rax");
}

void codegen(Node *node) {
	char function_name[128];
	const char *register_name[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
	int label;
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
	case nRef:
		gen_leftvalue(node->lhs);
		return;
	case nDeref:
		codegen(node->lhs);
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
	case nReturn:
		codegen(node->lhs);
		printasm(1, "pop rax");
		printasm(1, "mov rsp, rbp");
		printasm(1, "pop rbp");
		printasm(1, "ret");
		return;
	case nIf:
		label = ++label_number;
		codegen(node->cond);
		printasm(1, "pop rax");
		printasm(1, "cmp rax, 0");
		if (node->rhs != nullptr) {
			printasm(1, "jne .Lelse%d", label);
			codegen(node->rhs);
			printasm(1, "jmp .Lend%d", label);
			printasm(0, ".Lelse%d:", label);
		} else {
			printasm(1, "je .Lend%d", label);
		}
		codegen(node->lhs);
		printasm(0, ".Lend%d:", label);
		return;
	case nWhile:
		label = ++label_number;
		printasm(0, ".Lwhile%d:", label);
		codegen(node->cond);
		printasm(1, "pop rax");
		printasm(1, "cmp rax, 0");
		printasm(1, "je .Lend%d", label);
		codegen(node->lhs);
		printasm(1, "jmp .Lwhile%d", label);
		printasm(0, ".Lend%d:", label);
		return;
	case nFor:
		label = ++label_number;
		codegen(node->init);
		printasm(0, ".Lfor%d:", label);
		codegen(node->cond);
		printasm(1, "pop rax");
		printasm(1, "cmp rax, 0");
		printasm(1, "je .Lend%d", label);
		codegen(node->lhs);
		codegen(node->rhs);
		printasm(1, "jmp .Lfor%d", label);
		printasm(0, ".Lend%d:", label);
		return;
	case nBlock:
		Node *block;
		block = node->body;
		while (block) {
			codegen(block);
			block = block->next;
		}
		// printasm(1, "pop rax");
		return;
	case nFunctionCall:
		for (unsigned int i = 0; i < node->function->argument_number; ++i) {
			codegen(node->function->arguments[i]);
		}
		for (int i = node->function->argument_number-1; i >= 0; --i)
			printasm(1, "pop %s", register_name[i]);
		snprintf(function_name, node->function->length+1, "%s", node->function->name);
		printasm(1, "call %s", function_name);
		printasm(1, "push rax");
		return;
	case nFunctionDefinition:
		snprintf(function_name, node->function->length+1, "%s", node->function->name);
		printasm(0, "%s:", function_name);
		prologue();
		LocalVariable *arg = node->function->arguments_var;
		if (node->function->locals->offset > 0) {
			printasm(1, "mov rax, rbp");
			printasm(1, "sub rax, %d", node->function->locals->offset);
			printasm(1, "mov rsp, rax");
		}
		for (int u = node->function->argument_number-1; u >= 0 && arg->next; --u) {
			printasm(1, "mov -%d[rbp], %s", arg->offset, register_name[u]);
			arg = arg->next;
		}
		codegen(node->function->body);
		epilogue();
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