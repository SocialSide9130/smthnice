#include "node.hh"

#ifndef SmthNice_codegen
#define SmthNice_codegen

void printasm(int, const char *, ...);
void prologue(      );
void  codegen(Node *);
void epilogue(      );

#endif // SmthNice_codegen