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
} NodeKind;

typedef struct Node Node;
struct Node {
	NodeKind kind;
	long value;
	Node *lhs, *rhs;
};

Node      *parse(Token *token);
Node   *equality(            );
Node *relational(            );
Node       *expr(            );
Node        *add(            );
Node        *mul(            );
Node      *unary(            );
Node       *elem(            );

#endif