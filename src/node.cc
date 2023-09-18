#include <stdio.h>
#include <stdlib.h>

#include "token.hh"
#include "node.hh"


Token *cur;
Node *code[128];
Token *localVariables[128];

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

int get_localvariable_offset() {
	
}

bool expect_identifier() {
	return cur->kind == tIdentifier;
}

Node *new_identifier_node(char *name) {
	Node *identifier = new Node;
	if (identifier == nullptr) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
	identifier->kind = nLocalVariable;
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
		node = new_unary_node(nReturn, expr());
	} else {
		node = expr();
	}
	
	if (!read_operator(dSemiColon)) {
		fprintf(stderr, "Unexpected token: ");
		for (unsigned int i = 0; i < cur->length; ++i)
			fprintf(stderr, "%c", *cur->position+i);
		fprintf(stderr, ".");
		exit(1);
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
		node->kind = nLocalVariable;
		node->offset = (cur->position[0] - 'a' + 1) * 8;
		cur = cur->next;
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