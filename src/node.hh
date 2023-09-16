#include "token.hh"

#ifndef SmthNice_node
#define SmthNice_node

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
} NodeKind;

typedef struct Node Node;
struct Node {
	NodeKind kind;
	long value;
	Node *lhs, *rhs;
};

Node      *parse(Token *token);
Node  *statement(            );
Node       *expr(            );
Node     *assign(            );
Node   *equality(            );
Node *relational(            );
Node        *add(            );
Node        *mul(            );
Node      *unary(            );
Node       *elem(            );

#endif