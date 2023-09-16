#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "token.hh"
#include "node.hh"
#include "codegen.hh"

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
	Node *node = parse(tokens);

	prologue();
	codegen(node);
	epilogue();

	return 0;
}