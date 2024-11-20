#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum{
    CAT_NUM,
    CAT_OP,
    CAT_SPCHAR,
    CAT_ENDINPUT
};

typedef struct {
    char lexeme;
    int category;
} Token;

typedef struct {
    char lhs;
    char rhs[3];
    int rhsNum;
} ProdRule;

typedef struct {
    char termSign[5];
    char nonTermSigns[4];
    ProdRule prodRules[7];
} Language;

typedef struct {
    char c;
    int state;
} StackItem; 

typedef struct {
    StackItem* container;
    int top;
} Stack;

Token** readTokens(FILE* f);
//char* tokenToChar(Token token);

void printLanguage(const Language *lang);
void initShiftReduceMatrix(int sRMatrix[23][10]);
Language initLanguage();
int charToCol(char c);
void shiftReduceAutomata(Stack stack, FILE* f, Language l, Token** tokens, int m[23][10]);

Stack stackInit(Stack stack);
Stack push(Stack stack, StackItem item);
Stack pop(Stack stack);
StackItem top(Stack stack);
void printStack(Stack stack);
void printOutput(StackItem sItem, FILE* f, char* action, int state);
void printReduce(StackItem sItem, FILE* f, int state);
void printPop(StackItem sItem, FILE* f);
void printShift(StackItem sItem, FILE* f, int state);
void printPush(StackItem sItem, FILE* f);

int stringCompare(char str1[], char str2[]);
void strCopy(char *dest, char *src);
int strLen(char str[]);
void strCon(char *str1, char *str2);