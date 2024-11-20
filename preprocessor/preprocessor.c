#include "./preprocessor.h"
#include "./functions.h"

FILE *file;
long fsize;
char buffer[1024];
size_t bytesRead;
char a;

struct arguments {
    bool COMMENTS;
    bool DIRECTIVES;
    bool HELP;
} typedef arguments;

int main(int argc, char**argv) {
    arguments args = { false };

    //Loop that reads and confirms which flags have been tagged on the command
    for(int i = 0; i<argc; i++) {
        printf("%s\n", argv[i]);
        //Flag -c reader
        if (strcmp(argv[i], "-c") == 0){
            printf("Using flag -c to delete comments\n");
            args.COMMENTS = true;
        //Flag -d reader
        } else if (strcmp(argv[i], "-d") == 0) {
            printf("Using flag -d to include directives\n");
            args.DIRECTIVES = true;
        //Flag -help reader
        } else if (strcmp(argv[i], "-help") == 0) {
            printf("Using flag -help to print manual page\n");
            args.HELP = true;
            break;
        //Flag -all reader
        } else  if (strcmp(argv[i], "-all") == 0) {
            printf("Using flag -all to use all flags\n");
            args.COMMENTS = true;
            args.DIRECTIVES = true;
        }
      
    }

    if (args.HELP == true) {
        printManPage();
        return 0;
    }
    
    //File Reader
    file = fopen(argv[argc-1],"r");
    if(file != NULL){

        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);

        char* file_content = (char*)malloc(fsize);

        while (fgets(file_content, fsize, file) != NULL) {
            //puts(file_content);
        }
        free(file_content);
    }
    else{
        printf("read file failure\n");
    }

    FILE* newFile;
    char *newFileName;
    newFileName = argv[argc-1];
    
    //Remove the .c from the file name
    size_t len = strlen(newFileName);
    if (len >= 2 && strcmp(newFileName + len - 2, ".c") == 0) {
        newFileName[len - 2] = '\0';
    }
    printf("%s\n", newFileName);

    //Add _pp.c to the file name
    strcat(newFileName, "_pp.c");
    printf("%s\n", newFileName);
    
    //Create a file with the modified file name
    newFile = fopen(newFileName,"w+");
    if(newFile != NULL){

        fseek(newFile,0,SEEK_END);
        fsize = ftell(newFile);
        rewind(newFile);

        //Find start of the file
        fseek(file,0,0);

        //Copy the first char of the file
        a = fgetc(file);

        //Make a loop to copy all the chars of the file
        while(a != EOF){
        fputc(a, newFile);
        a = fgetc(file);
        }
    }
    else{
        printf("create file failure\n");
    }

    // Test the functions here, with the file to read as file and the file to write as newFile
    
    //


    
    if (args.COMMENTS == true) {
        eliminateSingleLineComments(newFile);
        eliminateMultipleLineComments(newFile);
    }

    if (args.DIRECTIVES == true) {
        backlashIdentifier(newFile);
        defineConstants(newFile);
    } 

    fclose(file);
    fclose(newFile);




    return 0;
}
    