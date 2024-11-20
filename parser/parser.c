#include "./parser.h"
#include "./functions.h"

// Here we initialize different variables that will be used through the code
FILE *file;
long fsize;

FILE *newFile;

ProdRule prodRules[7];
int shiftReduceMatrix[23][10];

int main(int argc, char**argv) {

    //Initialize the matrix
    initShiftReduceMatrix(shiftReduceMatrix);
    //PRINTING THE MATRIX, USEFUL FOR DEBUGGING
    /*
    for (int i = 0; i < 23; ++i) {
        for (int j = 0; j < 10; ++j) {
            printf("%d\t", shiftReduceMatrix[i][j]);
        }
        printf("\n");
    }
    */

   // Initialize language
    Language myLanguage = initLanguage();

    // printLanguage(&myLanguage);

    // We create a variable to save the filepath to read files from directory
    char filePath[100]; // Adjust the size according to your needs


    strCopy(filePath, "testing/");
    strCon(filePath, argv[argc-1]);

    // We read the file from the testing directory and open it
    file = fopen(filePath,"r");
    if(file != NULL){
        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);

    }
    else{
        printf("read file failure\n");
    }

    // We create a new filename for the output
    char *newFileName;
    newFileName = argv[argc-1];

    size_t len = strlen(newFileName);
    if (len >= 5) {
        newFileName[len - 5] = '\0';
    }
    strcat(newFileName, "_p3bdg.cscn");

    // A newfilepath is created to include this new writing file in a new directory
    char newFilePath[100] = ""; // Adjust the size according to your needs

    strcpy(newFilePath, "testing/");
    strcat(newFilePath, newFileName);

    // We open the output file inside the testing directory
    newFile = fopen(newFilePath,"w+");
    if(newFile != NULL){
        fseek(newFile,0,SEEK_END);
        fsize = ftell(newFile);
        rewind(newFile);
    }
    else{
        printf("write file failure\n");
    }

    // Creating the list of tokens
    Token** tokens = readTokens(file);
    // Initializing stack
    Stack stack;
    stack = stackInit(stack);
    // Calling the automata with all its parameters
    shiftReduceAutomata(stack , newFile, myLanguage, tokens, shiftReduceMatrix);
    printf("Automata ended succesfully\n");

    //CLOSE FILES
    fclose(file);
    fclose(newFile);
    free(tokens);
}
