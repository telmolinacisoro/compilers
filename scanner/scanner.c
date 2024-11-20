#include "./scanner.h"
#include "./functions.h"

FILE *file;
FILE *newFile;
long fsize;
char buffer[1024];
size_t bytesRead;
char a;

struct arguments { // Struct to store the arguments 
    bool DEBUG;
} typedef arguments;

int states[33][74];

int main(int argc, char**argv){
    arguments args;
    args.DEBUG = false;

    initStates(states); // Initialize the automata transitions matrix

    /*for (int i = 0; i < 33; i++){      //Print to see the transition matrix
        for (int j = 0; j < 74; j++){
        //if(j == 47){
            printf("[%d]",states[i][j]);
        //}
        }
        printf("\n");
    }*/

    char* ttoken = ";"; //Little code that uses the automata to return the transition matrix
    int ttoken_cat; 
    ttoken_cat = automata(ttoken, states); 
    printf("Ttoken category %d\n",ttoken_cat);

    //Read the length of the file's name
    int len = strLen(argv[argc-1]);
    //Create a string with its corresponding memory to allocate the file's name
    char* newFileName = malloc(len);

    //Loop that reads and confirms which flags have been tagged on the command
    for(int i = 0; i<argc; i++) {
        printf("%s\n", argv[i]);
        //Flag -c reader
        if (stringComp(argv[i], "-debug") != 0){
            printf("Using flag -debug to debug\n");
            args.DEBUG = true;
        }
        if (i + 1 == argc){
            // TODO: Save the length of the filename and check the last two characters
            // If they are .c, we can use the file, otherwise exit the code.
            
            strCopy(newFileName, argv[i]);

            printf("File name: %s\n",newFileName);
            
            printf("Name lenght: %d\n", len);

            if (newFileName[len-1]=='c' && newFileName[len-2]=='.'){
                strCon(newFileName, "scn");
                printf("%s\n", newFileName);
            } else {
                printf("file parameter failure\n");
            }


        }
    }

    //File Reader
    file = fopen(argv[argc-1],"r"); 
    if(file != NULL){
        fseek(file,0,SEEK_END); // Go to the end of the file
        fsize = ftell(file); // Get the position of the file
        rewind(file); // Go back to the beginning of the file
    }
    else{
        printf("read file failure\n");
    }
    
    printf("FILE OK\n"); 
    newFile = fopen(newFileName,"w"); // Create a new file to write the tokens
    if(newFile != NULL){
        //TO DO: Write the list of tokens on the output 
    }
    else{
        printf("creation file failure\n");
    }

    tokenize(file, newFile, states); // Tokenize the file and write the tokens on the new file

    fclose(file); 
    free(newFile);


    return 0;
}
    