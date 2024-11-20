#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


//void eliminateSingleLineComments(FILE* f);
//void eliminateMultipleLineComments(FILE* f);
//Example on how to include functions

int stringComp(char str1[], char str2[]);

//void strcpy1(char * s, char * p);

void strCopy(char* dest, char* src);

int strLen(char str[]);

void strCon(char* str1, char* str2);

void tokenize(FILE* rf, FILE* wf,int states[33][74]);

int automata(char* token, int states[33][74]);

int countSpecialChar(char* token);

int isSpecialChar(char c);

int countSpecialChar(char* token);

void initStates(int states[33][74]);
int checkState(int currState);
int charCol(int cAscii);
int advState(int currState, char c, int states[33][74]);
