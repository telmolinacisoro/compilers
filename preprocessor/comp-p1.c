/*
 * Compilers
 * Practice 1: c pre-processor
 * File name: comp-p1.c
 * Main program file
 * Additional files: comp-p1.h, comp-p1.c, lib1.h, lib1.c
 * 
 * Responsible: put the names of people responsible of the modifications of this file
 */
#include "./comp-p1.h"
#define MAX 67
#define Min 23
#define answerToEverything 42
#define long 999999999

int main(int argc, char**argv) {
    
    int i;
    printf ("%d\n", MAX);

    printf("hola que tal com vas?\n");
    ofile = stdout;             // default is stdout
    ofile = fopen( PRINTDESTFILE, "w");
    if(ofile == NULL)                 
        WARNING(0, (ofile, "Problem creating %s", PRINTDESTFILE));

    errfile = stdout;           // default is stdout
    errfile = fopen( PRINTERRORFILE, "w");
    if(errfile != NULL)                
        WARNING(0, (ofile, "Problem creating %s", PRINTERRORFILE));
    

    // Prints arguments
    fprintf(ofile, "Arguments:\n");
    for (i = 0; i < argc; i++) {
        fprintf(ofile, "%i: %s\n", i, argv[i]);
#if (PRINTWARNING == ON)
        WARNING(i, (errfile, "warning print example %s \n", argv[i]));
#endif
    }
    
#if (PRINTERROR == ON)
    ERROR(i, (errfile, "This is an error, and it stops"));
#endif
    
    fclose(ofile);
    fclose(errfile);
    
    return 0;
}
