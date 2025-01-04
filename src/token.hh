#ifndef SmthNice_token
#define SmthNice_token

// Utils
constexpr inline int detail1(const char c) {
	return (int)c;
}

constexpr inline int detail2(const char c1, const char c2) {
	int i1 = (int)c1, i2 = (int)c2;
	return (i1 << 8) + i2;
}

constexpr inline int detail3(const char c1, const char c2, const char c3) {
	int i1 = (int)c1, i2 = (int)c2, i3 = (int)c3;
	return (i1 << 16) + (i2 << 8) + i3;
}

// Type Definitions
typedef enum {
	tReserved,
	tNumber,
	tIdentifier,
	tOperator,
	tEof,
} TokenKind;

typedef enum {
	dPlus             =         detail1('+'), //  "+"
	dMinus            =         detail1('-'), //  "-"
	dAsterisk         =         detail1('*'), //  "*"
	dSlash            =         detail1('/'), //  "/"
	dPercentage       =         detail1('%'), //  "%"
	dEqual            =         detail1('='), //  "="
	dLess             =         detail1('<'), //  "<"
	dGreater          =         detail1('>'), //  ">"
	dOParenthesis     =         detail1('('), //  "("
	dCParenthesis     =         detail1(')'), //  ")"
	dOCuBracket       =         detail1('{'), //  "{"
	dCCuBracket       =         detail1('}'), //  "}"
	dOSqBracket       =         detail1('['), //  "["
	dCSqBracket       =         detail1(']'), //  "]"
	dExclamation      =         detail1('!'), //  "!"
	dComma            =         detail1(','), //  ","
	dSemiColon        =         detail1(';'), //  ";"
	dColon            =         detail1(':'), //  ":"
	dPeriod           =         detail1('.'), //  "."
	dAmpersand        =         detail1('&'), //  "&"

	dDblPlus          =    detail2('+', '+'), // "++"
	dDblMinus         =    detail2('-', '-'), // "--"
	dDblAsterisk      =    detail2('*', '*'), // "**"
	dDblSlash         =    detail2('/', '/'), // "//"
	dDblPercentage    =    detail2('%', '%'), // "%%"
	dDblLess          =    detail2('<', '<'), // "<<"
	dDblGreater       =    detail2('>', '>'), // ">>"
	dPlusEqual        =    detail2('+', '='), // "+="
	dMinusEqual       =    detail2('-', '='), // "-="
	dAsteriskEqual    =    detail2('*', '='), // "*="
	dSlashEqual       =    detail2('/', '='), // "/="
	dPercentageEqual  =    detail2('%', '='), // "%="
	dDblEqual         =    detail2('=', '='), // "=="
	dExclamationEqual =    detail2('!', '='), // "!="
	dLessEqual        =    detail2('<', '='), // "<="
	dGreaterEqual     =    detail2('>', '='), // ">="
	dMinusGreater     =    detail2('-', '>'), // "->"
	dEqualGreater     =    detail2('=', '>'), // "=>"
	dIf               =    detail2('i', 'f'), // "if"
	
	dElse             =    detail3('e', 'l', 's'), // "else"
	dReturn           =    detail3('r', 'e', 't'), // "return"
	dWhile            =    detail3('w', 'h', 'i'), // "while"
	dFor              =    detail3('f', 'o', 'r'), // "for"
	dInt              =    detail3('i', 'n', 't'), // "int"
} OperatorDetail;

typedef struct Token Token;
struct Token {
	TokenKind kind;
	OperatorDetail detail;
	long value;
	char *position;
	unsigned int length;
	Token *next;
};


Token *tokenize(char *);

#endif // SmthNice_token