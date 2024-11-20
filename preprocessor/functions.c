#include "./functions.h"

void eliminateSingleLineComments(FILE * f) {
    //Variables for the character reading
    char currentChar;
    char previousChar;
    //Places the char reader at the beggining of the file
    fseek(f,0,0);
    while ((currentChar = fgetc(f)) != EOF) {   //While that reads chars from a file
        // Check for the beginning of a comment
        if (previousChar == '/' && currentChar == '/') {
            //If found, retreat 2 positions
            fseek(f,-2, SEEK_CUR);
            //Put a space
            fputc(' ',f);
            while(currentChar != EOF && currentChar != '\n'){
                //Retreat to the previous char
                fseek(f,-1,SEEK_CUR);
                //Substitutes the previous char, advancing to the current char
                fputc(' ', f);
                //Updates the current char with the next char
                currentChar = fgetc(f);
            }
        }
        //Update the previous char to search for '//' on the next chars
        previousChar = currentChar;
    }       
}

void eliminateMultipleLineComments(FILE* f){
    char currentChar;
    char previousChar;
    //Places the char reader at the beggining of the file
    fseek(f,0,0);
    while ((currentChar = fgetc(f)) != EOF) {   //While that reads chars from a file
        // Check for the beginning of a comment
        if (previousChar == '/' && currentChar == '*') {
            //If found, retreat 2 positions
            fseek(f,-2, SEEK_CUR);
            //Put a space
            fputc(' ',f);
            while(currentChar != EOF){
                //Retreat to the previous char
                fseek(f,-1,SEEK_CUR);
                //Substitutes the previous char, advancing to the current char
                fputc(' ', f);
                //Updates the current char with the next char
                previousChar = currentChar;
                //And advances a char
                currentChar = fgetc(f);
                //If it finds the end of the comment
                if (previousChar == '*' && currentChar == '/'){
                    //It retraets a possition 
                    fseek(f,-1,SEEK_CUR);
                    //Deletes the '/'
                    fputc(' ',f);
                    //And stops the while
                    break;
                }
            }
        }
        //Update the previous char to search for '/*' on the next chars
        previousChar = currentChar;
    }

}

void backlashIdentifier(FILE* f) {

    //We define some char variables to work on this function and a temporal file
    char currentChar;
    char nextChar;
    FILE* temp;

    //Search the start of the preprocessed file and creates a temporal file
    fseek(f,0,0);
    temp = fopen("tempF.c","w+");
    
    //Reads all the preprocessed file and writes it on the temporal file ignoring '\' and their end of lines
    while((currentChar = fgetc(f)) != EOF && (nextChar != EOF)){
        nextChar = fgetc(f);
        fseek(f,-1,SEEK_CUR);
        if (currentChar == 92 && nextChar != 110){
            fseek(f,2,SEEK_CUR);
        } else {
            fputc(currentChar, temp);
        }
    }

    //Truncates the preprocessor file, and if it fails it prints an error
    if (ftruncate(fileno(f), 0) != 0) {
        printf("Error deleting destination file\n");
        fclose(temp);
    }

    //Seeks the starting positions on the preprocessed ant temporal files
    fseek(f,0,0);
    fseek(temp,0,0);

    //Writes all the code from temp file to the preprocessed file    
    while ((currentChar = fgetc(temp)) != EOF) {
        fputc(currentChar, f);
    }

    //Closes the temporal file and removes it
    fclose(temp);
    remove("tempF.c");
}

/*void directiveIfDef(FILE* f, const char* macro) {
    char line[256];

    while (fgets(line, sizeof(line), f) != NULL) {

        // Check for the #ifdef directive
        if (strncmp(line, "#ifdef", 6) == 0) {
            char ifdefMacro[256];
            sscanf(line + 6, "%s", ifdefMacro);

            // If the specified macro is defined, copy the following line to the specified file
            if (strcmp(ifdefMacro, macro) == 0) {
                char nextLine[256];

                // Read the next line from the file
                if (fgets(nextLine, sizeof(nextLine), f) != NULL) {
                    // Write the line to the file
                    fputs(nextLine, f);
                }
            }
        }
    }
}*/

typedef struct {

   char *name;
   char *value;

} Define;

void defineConstants(FILE* f) {
   /*Define defines[100];
   int numDefines = 0;


   fseek(of,0,0);
    // Read lines from the file
    char line[256];
    while (fgets(line, sizeof(line), of) != NULL) {
        // Check if the line starts with "#define"
        if (strstr(line, "#define") != NULL) {
            // BORRAR EL DEFINE
            // Parse the line and store the define
            if (sscanf(line, "#define %s %d", defines[numDefines].name, &defines[numDefines].value) == 2) {
               
               numDefines++;
            }
        }
    }

    for (int i=0; i<numDefines; i++){
        defines[i].nameLength = strlen(defines[i].name);
        snprintf (defines[i].valueToStr, sizeof(defines[i].valueToStr), "%d",defines[i].value);
        defines[i].valueLength = strlen(defines[i].valueToStr);

    }
   

    fseek(of,0,0);
    fseek(f, 0, 0);
    char currentChar;
    char previousChar;
    int chartoascii;
    int k = 0;
    int m = 0;
    FILE* tempFile = fopen("tempFile.c", "w+");
    while (fgets(line, sizeof(line), f) != NULL) {
        
        for (int i=0; i<numDefines; i++){
            if (strstr(line, defines[i].name) != NULL){
                k = 0;
                int lenLine = strlen(line);
                currentChar = defines[i].name[k];

                for(int j=0; j<lenLine; j++){
                    if(line[j] == currentChar){
                        k++;
                        previousChar = currentChar;
                        currentChar = defines[i].name[k];
                        if(k == defines[i].nameLength){
                            
                            for(int n = 0; n<defines[i].nameLength;n++){
                                if(defines[i].valueLength > n){
                                    line[j - defines[i].nameLength + n + 1] = defines[i].valueToStr[n];
                                } else {
                                    line[j - defines[i].nameLength + n + 1] = ' ';
                                }
                                
                            }
                            k = 0;
                            
                        }
                    }
                    
                }
            }
        }
        if (sscanf(line, "#define %s %d", defines[numDefines].name, &defines[numDefines].value) != 2) {
            fputs(line, tempFile);
        }
        
    }

    if(ftruncate(fileno(f), 0) != 0) {
        printf("Error deleting destination file\n");
        fclose(tempFile);
    }

    fseek(f,0,0);
    fseek(tempFile,0,0);
    char c;
    c = fgetc(tempFile); 
    while (c != EOF) 
    { 
        fputc(c, f); 
        c = fgetc(tempFile); 
    } 
    fclose(tempFile);
    remove("tempFile.c");*/

    Define defines[100];
    int numDefines = 0;
    FILE *temp;
    char currentChar;
    char line[1000]; // Adjust the size as per your requirement
    char delimiters[] = " \t\n"; // Delimiter characters

    fseek(f,0,0);

    while (fgets(line, sizeof(line), f) != NULL) {
        // Tokenize the line
        char *token = strtok(line, delimiters);

        // Iterate through the tokens
        while (token != NULL) {
            if (strcmp(token,"#define") == 0){
                // Advance a token
                token = strtok(NULL, delimiters);
                if (token != NULL) {
                    // Allocate memory for the name
                    defines[numDefines].name = malloc(strlen(token) + 1);
                    if (defines[numDefines].name == NULL) {
                        printf("Memory allocation failed\n");
                        exit(1);
                    }
                    // Copy the variable name
                    strcpy(defines[numDefines].name, token);
                    printf("Name %s\n", defines[numDefines].name);

                    // Advance a token and do the same for the value
                    token = strtok(NULL, delimiters);
                    if (token != NULL) {
                        // Allocate memory for the value
                        defines[numDefines].value = malloc(strlen(token) + 1);
                        if (defines[numDefines].value == NULL) {
                            printf("Memory allocation failed\n");
                            exit(1);
                        }
                        // Copy the value
                        strcpy(defines[numDefines].value, token);
                        printf("Value %s\n", defines[numDefines].value);
                        numDefines++;
                    }
                }
            } else {
                // Get the next token
                token = strtok(NULL, delimiters);
            }
        }
    }

    fseek(f,0,0);
    //TODO:

    //Open a temporary file and copy all the code from the file to process inside
    //changing the constants by its values

    //Truncate the origianl file (you can use the if on the function of Backslash include)

    //Copy from the temporary file all the code to the file to process

    //Celebrate

    //START HERE
    //temp = fopen("tempF.c","w+");
    
   
}



void defineMacros(FILE* f){

}


#define MAX_INCLUDES 100
#define MAX_FILENAME_LENGTH 256

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
} Include;

void defineIncludes(FILE* f) {
    Include includes[MAX_INCLUDES];
    int numIncludes = 0;

    fseek(f, 0, SEEK_SET);

    char line[1000]; // Ajusta el tamaño según tus necesidades
    char delimiters[] = " \t\n\"<>"; // Caracteres delimitadores

    // Leer los #include y copiarlos a includes
    while (fgets(line, sizeof(line), f) != NULL && numIncludes < MAX_INCLUDES) {
        // Tokenizar la línea
        char *token = strtok(line, delimiters);

        // Iterar a través de los tokens
        while (token != NULL) {
            if (strcmp(token, "#include") == 0) {
                // Si encuentra #include, intenta obtener el nombre del archivo
                token = strtok(NULL, delimiters); // El siguiente token debe ser el nombre del archivo
                if (token != NULL && numIncludes < MAX_INCLUDES) {
                    char* filename_start = strrchr(token, '/') + 1; // Encuentra la última barra (ruta relativa)
                    if (filename_start == NULL) {
                        filename_start = token; // Si no hay barra, usa todo el token
                    }
                    strcpy(includes[numIncludes].filename, filename_start);
                    numIncludes++;
                }
            } else {
                // Obtener el siguiente token
                token = strtok(NULL, delimiters);
            }
        }
    }

    // Rebobinar el archivo para poder sobreescribirlo
    fseek(f, 0, SEEK_SET);

    // Crear un archivo temporal para almacenar el contenido modificado
    FILE* temp = tmpfile();
    if (temp == NULL) {
        perror("Error al crear archivo temporal");
        return;
    }

    // Variable de control para evitar la duplicación de contenido
    int include_copied = 0;

    // Leer el archivo original línea por línea y realizar el reemplazo de #include
    while (fgets(line, sizeof(line), f) != NULL) {
        if (!include_copied) {
            int found_include = 0;
            for (int i = 0; i < numIncludes; i++) {
                if (strstr(line, includes[i].filename) != NULL) {
                    found_include = 1;

                    // Abrir el archivo incluido
                    FILE* incluido = fopen(includes[i].filename, "r");
                    if (incluido == NULL) {
                        perror("Error al abrir el archivo incluido");
                        break; // Salir del bucle for si no se puede abrir el archivo incluido
                    }

                    // Copiar el contenido del archivo incluido al archivo temporal
                    char c;
                    while ((c = fgetc(incluido)) != EOF) {
                        fputc(c, temp);
                    }

                    fclose(incluido); // Cerrar el archivo incluido
                    include_copied = 1; // Marcar que ya se ha copiado un include
                    break; // Salir del bucle for después de encontrar el archivo incluido
                }
            }
        }

        // Si no se encuentra una directiva #include o ya se ha copiado un include, simplemente copia la línea al archivo temporal
        if (!strstr(line, "#include")) {
            fputs(line, temp);
        }
    }

    // Vaciar el contenido del archivo principal
    freopen(NULL, "w", f);

    // Copiar el contenido desde el archivo temporal al archivo principal
    rewind(temp);
    while (fgets(line, sizeof(line), temp) != NULL) {
        fputs(line, f);
    }

    fclose(temp); // Cerrar el archivo temporal
}


void printManPage(){
    FILE *file;
    long fsize;
    file = fopen("man.txt","r");

    if(file != NULL){

        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);

        char* file_content = (char*)malloc(fsize);

        while (fgets(file_content, fsize, file) != NULL) {
            puts(file_content);
        }
        free(file_content);
    }
    else{
        printf("open failure\n");
    }
    fclose(file);
}
