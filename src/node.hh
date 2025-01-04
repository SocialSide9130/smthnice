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
	nRef,
	nDeref,
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

typedef struct LocalVariable LocalVariable;
struct LocalVariable {
	LocalVariable *next;
	char *name;
	unsigned int length;
	int offset;
};


struct Function {
	char *name;
	unsigned int length, argument_number;
	Node *body, **arguments;
	LocalVariable *locals, *arguments_var;
};

struct Node {
	NodeKind kind;
	long value;
	Node *init, *cond, *lhs, *rhs, *next, *body;
	Function *function;
	LocalVariable *locals;
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