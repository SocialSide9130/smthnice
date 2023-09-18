#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.hh"

// Function Definitions
inline bool is_alpha_(int c) {
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_';
}

inline bool is_alpha_or_number(int c) {
	return is_alpha_(c) || '0' <= c && c <= '9';
}

Token *new_number_token(long value, char *position, unsigned int length) {
	Token *token = new Token;
	token->kind = tNumber;
	token->value = value;
	token->position = position;
	token->length = length;
	token->next = nullptr;
	return token;
}

Token *new_operator_token(OperatorDetail detail, char *position, unsigned int length) {
	Token *token = new Token;
	token->kind = tOperator;
	token->detail = detail;
	token->position = position;
	token->length = length;
	token->next = nullptr;
	return token;
}

Token *new_identifier_token(char *position, unsigned int length) {
	Token *token = new Token;
	token->kind = tIdentifier;
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
				cur->next = new_operator_token(dDblPlus, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dPlusEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dPlus, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '-') {
			switch (*(p+1)) {
			case '-':
				cur->next = new_operator_token(dDblMinus, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dMinusEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '>':
				cur->next = new_operator_token(dMinusGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dMinus, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '*') {
			switch (*(p+1)) {
			case '*':
				cur->next = new_operator_token(dDblAsterisk, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dAsteriskEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dAsterisk, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '/') {
			switch (*(p+1)) {
			case '/':
				cur->next = new_operator_token(dDblSlash, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dSlashEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dSlash, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '%') {
			switch (*(p+1)) {
			case '%':
				cur->next = new_operator_token(dDblPercentage, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dPercentageEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dPercentage, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '<') {
			switch (*(p+1)) {
			case '<':
				cur->next = new_operator_token(dDblLess, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dLessEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dLess, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '>') {
			switch (*(p+1)) {
			case '>':
				cur->next = new_operator_token(dDblGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '=':
				cur->next = new_operator_token(dGreaterEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dGreater, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '=') {
			switch (*(p+1)) {
			case '=':
				cur->next = new_operator_token(dDblEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			case '>':
				cur->next = new_operator_token(dEqualGreater, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dEqual, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '!') {
			switch (*(p+1)) {
			case '=':
				cur->next = new_operator_token(dExclamationEqual, p, 2);
				cur = cur->next;
				p += 2;
				break;
			default:
				cur->next = new_operator_token(dExclamation, p, 1);
				cur = cur->next;
				++p;
				break;
			}
			continue;
		}

		if (*p == '(') {
			cur->next = new_operator_token(dOParenthesis, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ')') {
			cur->next = new_operator_token(dCParenthesis, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '{') {
			cur->next = new_operator_token(dOCuBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '}') {
			cur->next = new_operator_token(dCCuBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '[') {
			cur->next = new_operator_token(dOSqBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ']') {
			cur->next = new_operator_token(dCSqBracket, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ',') {
			cur->next = new_operator_token(dComma, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ';') {
			cur->next = new_operator_token(dSemiColon, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == ':') {
			cur->next = new_operator_token(dColon, p, 1);
			cur = cur->next;
			++p;
			continue;
		}

		if (*p == '.') {
			cur->next = new_operator_token(dPeriod, p, 1);
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

		if (strncmp("return", p, 6) == 0 && !is_alpha_or_number(p[6])) {
			cur->next = new_operator_token(dReturn, p, 6);
			cur = cur->next;
			p += 6;
			continue;
		}

		if (strncmp("if", p, 2) == 0 && !is_alpha_or_number(p[2])) {
			cur->next = new_operator_token(dIf, p, 2);
			cur = cur->next;
			p += 2;
			continue;
		}

		if (strncmp("else", p, 4) == 0 && !is_alpha_or_number(p[4])) {
			cur->next = new_operator_token(dElse, p, 4);
			cur = cur->next;
			p += 4;
			continue;
		}

		if (is_alpha_(*p)) {
			char *after = p;
			while (is_alpha_or_number(*after))
				++after;
			cur->next = new_identifier_token(p, after - p);
			cur = cur->next;
			p = after;
			continue;
		}
	}

	cur->next = new_eof_token(p);
	return head.next;
}
