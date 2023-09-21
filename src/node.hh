#include "token.hh"

#ifndef SmthNice_node
#define SmthNice_node

typedef struct Function Function;
typedef struct Node Node;

typedef enum {
	nNumber,
	nAdd,
	nSub,
	nMul,
	nDiv,
	nEqual,
	nNotEqual,
	nLess,
	nLessEqual,
	nAssign,
	nReturn,
	nIf,
	nElse,
	nWhile,
	nFor,
	nLocalVariable,
	nBlock,
	nFunctionCall,
	nFunctionDefinition,
} NodeKind;

struct Function {
	Function *next;
	char *name;
	unsigned int length, arguments_number;
	Node **arguments, **body;
};

struct Node {
	NodeKind kind;
	long value;
	Node *init, *cond, *lhs, *rhs, **vector;
	Function *function;
	int offset;
};

void       program(Token *token);
Node        *parse(Token *token);
Node *function_def(            );
Node    *statement(            );
Node         *expr(            );
Node       *assign(            );
Node     *equality(            );
Node   *relational(            );
Node          *add(            );
Node          *mul(            );
Node        *unary(            );
Node         *elem(            );

#endif