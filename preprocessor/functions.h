#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void eliminateSingleLineComments(FILE* f);
void eliminateMultipleLineComments(FILE* f);
void backlashIdentifier(FILE* f);
void directiveIfDef(FILE* f);
void defineConstants(FILE* f);
void defineMacros(FILE* f);
void defineIncludes(FILE* f);
void printManPage();

