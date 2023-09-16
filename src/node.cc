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
	return expr();
}

Node *expr() {
	return add();
}

Node *add() {
	Node *lhs = mul();

	while (cur->kind != tEof) {
		if (cur->kind == tSymbol) {
			switch (cur->detail) {
			case dPlus:
				cur = cur->next;
				lhs = new_binary_node(nAdd, lhs, mul());
				break;
			case dMinus:
				cur = cur->next;
				lhs = new_binary_node(nSub, lhs, mul());
				break;
			default:
				return lhs;
			}
		}
	}
	return lhs;
}

Node *mul() {
	Node *lhs = unary();

	while (cur->kind != tEof) {
		if (cur->kind == tSymbol) {
			switch (cur->detail) {
			case dAsterisk:
				cur = cur->next;
				lhs = new_binary_node(nMul, lhs, unary());
				break;
			case dSlash:
				cur = cur->next;
				lhs = new_binary_node(nDiv, lhs, unary());
				break;
			default:
				return lhs;
			}
		}
	}
	return lhs;
}

Node *unary() {
	if (read_symbol(dMinus)) {
		return new_binary_node(nSub, new_number_node(0), unary());
	}

	if (read_symbol(dPlus)) {
		return elem();
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