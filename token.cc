#include <stdlib.h>
#include <ctype.h>
#include "token.hh"

// Function Definitions
Token *new_number_token(long value, char *position, unsigned int length) {
	Token *token = new Token;
	token->kind = tNumber;
	token->value = value;
	token->position = position;
	token->length = length;
	token->next = nullptr;
	return token;
}

Token *new_symbol_token(SymbolDetail detail, char *position, unsigned int length) {
	Token *token = new Token;
	token->kind = tSymbol;
	token->detail = detail;
	token->position = position;
	token->length = length;
	token->next = nullptr;
	return token;
}

Token *new_eof_token(char *position) {
	Token *token = new Token;
	token->kind = tEof;
	token->position = position;
	token->length = 1;
	token->next = nullptr;
	return token;
}

Token *tokenize(char *code) {
	char *p = code;
	Token head, *cur;
	cur = &head;

	while (*p) {
		if (isspace(*p)) {
			++p;
			continue;
		}

		if (*p == '+') {
			switch (*(p+1)) {
			case '+':
				cur->next = new_symbol_token(dDblPlus, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dPlusEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dPlus, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '-') {
			switch (*(p+1)) {
			case '-':
				cur->next = new_symbol_token(dDblMinus, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dMinusEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '>':
				cur->next = new_symbol_token(dMinusGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dMinus, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '*') {
			switch (*(p+1)) {
			case '*':
				cur->next = new_symbol_token(dDblAsterisk, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dAsteriskEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dAsterisk, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '/') {
			switch (*(p+1)) {
			case '/':
				cur->next = new_symbol_token(dDblSlash, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dSlashEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dSlash, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '%') {
			switch (*(p+1)) {
			case '%':
				cur->next = new_symbol_token(dDblPercentage, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dPercentageEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dPercentage, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '<') {
			switch (*(p+1)) {
			case '<':
				cur->next = new_symbol_token(dDblLess, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dLessEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dLess, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '>') {
			switch (*(p+1)) {
			case '>':
				cur->next = new_symbol_token(dDblGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_symbol_token(dGreaterEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dGreater, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '=') {
			switch (*(p+1)) {
			case '=':
				cur->next = new_symbol_token(dDblEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '>':
				cur->next = new_symbol_token(dEqualGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dEqual, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '!') {
			switch (*(p+1)) {
			case '=':
				cur->next = new_symbol_token(dExclamationEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_symbol_token(dExclamation, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '(') {
			cur->next = new_symbol_token(dOParenthesis, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ')') {
			cur->next = new_symbol_token(dCParenthesis, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '{') {
			cur->next = new_symbol_token(dOCuBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '}') {
			cur->next = new_symbol_token(dCCuBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '[') {
			cur->next = new_symbol_token(dOSqBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ']') {
			cur->next = new_symbol_token(dCSqBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ',') {
			cur->next = new_symbol_token(dComma, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ';') {
			cur->next = new_symbol_token(dSemiColon, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ':') {
			cur->next = new_symbol_token(dColon, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '.') {
			cur->next = new_symbol_token(dPeriod, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if ('0' <= *p && *p <= '9') {
			char *after;
			long value = strtol(p, &after, 10);
			cur->next = new_number_token(value, p, after - p);
			cur = cur->next;
			p = after;
			continue;
		}
	}

	cur->next = new_eof_token(p);
	return head.next;
}
