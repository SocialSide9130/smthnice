#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "token.hh"
#include "node.hh"
#include "codegen.hh"

extern Node *code;

int main(int argc, char *argv[]) {
	char *src;
#ifdef Smth_dbg
	char buffer[1024];
	fgets(buffer, 1023, stdin);
	src = buffer;
#else
	src = argv[1];
#endif
	Token *tokens = tokenize(src);
	program(tokens);
	Node *node = code;

	// prologue();
	printasm(0, ".intel_syntax noprefix");
	printasm(0, ".globl main");

	
	// printasm(1, "sub rsp, 208"); 
	while (*node) {
		codegen(*node);
		// epilogue();
		node = node->next;
	}

	return 0;
}