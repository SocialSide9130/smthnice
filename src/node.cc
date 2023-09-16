#include <stdio.h>
#include <stdlib.h>

#include "token.hh"
#include "node.hh"


Token *cur;

inline bool expect_number() {
	return cur->kind == tNumber;
}

bool read_symbol(SymbolDetail symbol) {
	if (cur->kind == tSymbol && cur->detail == symbol) {
		cur = cur->next;
		return true;
	}
	return false;
}

Node *new_binary_node(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = new Node;
	if (node == NULL) {
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

Node *parse(Token *token) {
	cur = token;
	return statement();
}

Node *statement() {
	Node *node;
	for (;;) {
		node = expr();
		if (!read_symbol(dSemiColon)) {
			fprintf(stderr, "Unexpected token: ");
			for (unsigned int i = 0; i < cur->length; ++i)
				fprintf(stderr, "%c", *cur->position+i);
			fprintf(stderr, ".");
			exit(1);
		}
		break;
	}
	return node;
}

Node *expr() {
	return assign();
}

Node *assign() {
	Node *node = equality();
	for (;;) {
		if (read_symbol(dEqual)) {
			cur = cur->next;
			node = new_binary_node(nAssign, node, assign());
		}
		break;
	}
	return node;
}

Node *equality() {
	Node *lhs = relational();

	for (;;) {
		if (read_symbol(dDblEqual))
			lhs = new_binary_node(nEqual, lhs, relational());
		else if (read_symbol(dExclamationEqual))
			lhs = new_binary_node(nNotEqual, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *relational() {
	Node *lhs = add();

	for (;;) {
		if (read_symbol(dLess))
			lhs = new_binary_node(nLess, lhs, add());
		else if (read_symbol(dLessEqual))
			lhs = new_binary_node(nLessEqual, lhs, add());
		else if (read_symbol(dGreaterEqual))
			lhs = new_binary_node(nLess, add(), lhs);
		else if (read_symbol(dGreater))
			lhs = new_binary_node(nLessEqual, add(), lhs);
		else
			return lhs;
	}
	return lhs;
}

Node *add() {
	Node *lhs = mul();

	for (;;) {
		if (read_symbol(dPlus))
			lhs = new_binary_node(nAdd, lhs, relational());
		else if (read_symbol(dMinus))
			lhs = new_binary_node(nSub, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *mul() {
	Node *lhs = unary();

	for (;;) {
		if (read_symbol(dAsterisk))
			lhs = new_binary_node(nMul, lhs, relational());
		else if (read_symbol(dSlash))
			lhs = new_binary_node(nDiv, lhs, relational());
		else
			return lhs;
	}
	return lhs;
}

Node *unary() {
	if (read_symbol(dMinus)) {
		return new_binary_node(nSub, new_number_node(0), unary());
	}

	if (read_symbol(dPlus)) {
		return unary();
	}

	return elem();
}

Node *elem() {
	if (read_symbol(dOParenthesis)) {
		Node *e = expr();
		if (!read_symbol(dCParenthesis)) {
			fprintf(stderr, "There must be a close parenthesis.\n");
			exit(1);
		}
		return e;
	}

	if (expect_number()) {
		Node *node = new_number_node(cur->value);
		cur = cur->next;
		return node;
	}

	fprintf(stderr, "Unexpected end of file.\n");
	return nullptr;
}