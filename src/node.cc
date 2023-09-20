#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.hh"
#include "node.hh"


Token *cur;
Node *code[128];

typedef struct LocalVariable LocalVariable;
struct LocalVariable {
	LocalVariable *next;
	char *name;
	unsigned int length;
	int offset;
};

LocalVariable *localvariables;

void error_() {
	fprintf(stderr, "Unexpected token: ");
	for (unsigned int i = 0; i < cur->length; ++i)
		fprintf(stderr, "%c", *cur->position+i);
	fprintf(stderr, ".");
}

inline bool expect_number() {
	return cur->kind == tNumber;
}

bool read_operator(OperatorDetail symbol) {
	if (cur->kind == tOperator && cur->detail == symbol) {
		cur = cur->next;
		return true;
	}
	return false;
}

LocalVariable *find_localvariable() {
	for (LocalVariable *var = localvariables; var; var = var->next) {
		if (var->length == cur->length && !memcmp(var->name, cur->position, var->length))
			return var;
	}
	return nullptr;
}

bool expect_identifier() {
	return cur->kind == tIdentifier;
}

Node *new_unary_node(NodeKind kind, Node *node) {
	Node *unary = new Node;
	if (unary == nullptr) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
	unary->kind = kind;
	unary->lhs = node;
	return unary;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = new Node;
	if (node == nullptr) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_if_node(Node *cond, Node *if_true) {
	Node *node = new Node;
	if (node == nullptr) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
	node->kind = nIf;
	node->cond = cond;
	node->lhs = if_true;
	return node;
}

Node *new_whlie_node(Node *cond, Node *stmt) {
	Node *node = new Node;
	if (node == nullptr) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
	node->kind = nWhile;
	node->cond = cond;
	node->lhs = stmt;
	return node;
}

Node *new_for_node(Node *init, Node *cond, Node *step, Node *stmt) {
	Node *node = new Node;
	if (node == nullptr) {
		fprintf(stderr, "Erro: Out of memory.\n");
		exit(1);
	}
	node->kind = nFor;
	node->init = init;
	node->cond = cond;
	node->lhs = stmt;
	node->rhs = step;
	return node;
}

Node *new_number_node(long value) {
	Node *node = new Node;
	node->kind = nNumber;
	node->value = value;
	node->lhs = node->rhs = nullptr;
	return node;
}

void program(Token *token) {
	int i = 0;
	cur = token;
	localvariables = new LocalVariable;
	while (cur->kind != tEof)
		code[i++] = statement();
	code[i] = nullptr;
}

Node *parse(Token *token) {
	cur = token;
	return statement();
}

Node *statement() {
	Node *node;
	if (read_operator(dReturn)) {
		// "return"
		node = new_unary_node(nReturn, expr());
		if (!read_operator(dSemiColon)) {
			error_();
			exit(1);
		}
	} else if (read_operator(dIf)) {
		// "if"
		if (!read_operator(dOParenthesis)) {
			error_();
			exit(1);
		}
		Node *cond = expr();
		if (!read_operator(dCParenthesis)) {
			error_();
			exit(1);
		}
		node = statement();
		node = new_if_node(cond, node);
		if (read_operator(dElse)) {
			// "else"
			node->rhs = statement();
		}
	} else if (read_operator(dWhile)) {
		// "while"
		if (!read_operator(dOParenthesis)) {
			error_();
			exit(1);
		}
		Node *cond = expr();
		if (!read_operator(dCParenthesis)) {
			error_();
			exit(1);
		}
		node = new_whlie_node(cond, statement());
	} else if (read_operator(dFor)) {
		// "for"
		Node *init, *cond, *step;
		if (!read_operator(dOParenthesis)) {
			error_();
			exit(1);
		}
		if (!read_operator(dSemiColon)) {
			init = expr();
			if (!read_operator(dSemiColon)) {
				error_();
				exit(1);
			}
		} else init = nullptr;
		if (!read_operator(dSemiColon)) {
			cond = expr();
			if (!read_operator(dSemiColon)) {
				error_();
				exit(1);
			}
		} else cond = nullptr;
		if (!read_operator(dSemiColon)) {
			step = expr();
		} else step = nullptr;
		if (!read_operator(dCParenthesis)) {
			error_();
			exit(1);
		}
		node = new_for_node(init, cond, step, statement());
	} else if (read_operator(dOCuBracket)) {
		// "{"
		// 最大128文
		Node **vector;
		node = new Node;
		vector = new Node*[128];
		int stmt_index = 0;

		while (!read_operator(dCCuBracket)) {
			vector[stmt_index] = statement();
			++stmt_index;
		}
		node->kind = nBlock;
		node->vector = vector;
	} else {
		node = expr();
		if (!read_operator(dSemiColon)) {
			error_();
			exit(1);
		}
	}
	
	
	return node;
}

Node *expr() {
	return assign();
}

Node *assign() {
	Node *node = equality();
	for (;;) {
		if (read_operator(dEqual)) {
			node = new_binary_node(nAssign, node, assign());
		}
		break;
	}
	return node;
}

Node *equality() {
	Node *lhs = relational();

	for (;;) {
		if (read_operator(dDblEqual))
			lhs = new_binary_node(nEqual, lhs, relational());
		else if (read_operator(dExclamationEqual))
			lhs = new_binary_node(nNotEqual, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *relational() {
	Node *lhs = add();

	for (;;) {
		if (read_operator(dLess))
			lhs = new_binary_node(nLess, lhs, add());
		else if (read_operator(dLessEqual))
			lhs = new_binary_node(nLessEqual, lhs, add());
		else if (read_operator(dGreaterEqual))
			lhs = new_binary_node(nLess, add(), lhs);
		else if (read_operator(dGreater))
			lhs = new_binary_node(nLessEqual, add(), lhs);
		else
			return lhs;
	}
	return lhs;
}

Node *add() {
	Node *lhs = mul();

	for (;;) {
		if (read_operator(dPlus))
			lhs = new_binary_node(nAdd, lhs, relational());
		else if (read_operator(dMinus))
			lhs = new_binary_node(nSub, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *mul() {
	Node *lhs = unary();

	for (;;) {
		if (read_operator(dAsterisk))
			lhs = new_binary_node(nMul, lhs, relational());
		else if (read_operator(dSlash))
			lhs = new_binary_node(nDiv, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *unary() {
	if (read_operator(dMinus)) {
		return new_binary_node(nSub, new_number_node(0), unary());
	}

	if (read_operator(dPlus)) {
		return unary();
	}

	return elem();
}

Node *elem() {
	if (read_operator(dOParenthesis)) {
		Node *e = expr();
		if (!read_operator(dCParenthesis)) {
			fprintf(stderr, "There must be a close parenthesis.\n");
			exit(1);
		}
		return e;
	}

	if (expect_identifier()) {
		Node *node = new Node;
		if (cur->next->detail == dOParenthesis) {
			node->kind = nFunctionCall;
			node->function = new Function;
			node->function->name = cur->position;
			node->function->length = cur->length;
			cur = cur->next;
			read_operator(dOParenthesis);
			// 引数リストの処理など...
			if (!read_operator(dCParenthesis)) {
				error_();
				exit(1);
			}
		} else {
			LocalVariable *lvar;
			// Node *node;

			lvar = find_localvariable();
			node->kind = nLocalVariable;
			if (lvar != nullptr) {
				node->offset = lvar->offset;
			} else {
				lvar = new LocalVariable;
				lvar->next = localvariables;
				lvar->name = cur->position;
				lvar->length = cur->length;
				lvar->offset = localvariables->offset + 8;
				node->offset = lvar->offset;
				localvariables = lvar;
			}
			cur = cur->next;
		}
		return node;
	}

	if (expect_number()) {
		Node *node = new_number_node(cur->value);
		cur = cur->next;
		return node;
	}

	fprintf(stderr, "Unexpected end of file.\n");
	return nullptr;
}