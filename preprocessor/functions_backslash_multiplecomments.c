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
    char currentChar;
    char previousChar; 
    fseek(f, 0, SEEK_SET); 
    while ((currentChar = fgetc(f)) != EOF) {
        if (previousChar == 92 && currentChar != 110) { // ASCII code for backslash '\' and 'n'
            fseek(f, -2, SEEK_CUR); 
            fputc(' ', f); // We delete the backslash
            fputc(' ', f); // We delete the end of line which consists of two chars '\n'
            fputc(' ', f);
            fseek(f, 0, SEEK_CUR);
            
        }
        previousChar = currentChar;
    }
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

void defineConstants(FILE* f) {
    char line[256];

    while (fgets(line, sizeof(line), f) != NULL) {

        // strncmp compares the 7 first characters with the string #define, if the 7 characters 
        // are the same returns 0. Then, we check if that is happening: 
        if (strncmp(line, "#define", 7) == 0) {

	        // two arrays are declared to store in str1 the identifier of the define and in 
            // str2 the replacement text
            char str1[256], str2[256];

            // After the #define, extract the str1 and str2, where str1 finishes after a 
            // space and str2 at jump of line.
            sscanf(line + 7, "%s %[^\n]", str1, str2);

            // Here we start to replace occurrences of str1 with str2 in the file f
            fseek(f, 0, SEEK_SET); // First we move the file pointer to the beginning of the file

            FILE* tempFile = tmpfile(); // Create a temporary file
            char tempLine[256];

            while (fgets(tempLine, sizeof(tempLine), f) != NULL) {

                // Pointer position at the start of the line
                char* pos = tempLine;

	            // Search a match of the first occurrence of the #define (str1)
                while ((pos = strstr(pos, str1)) != NULL) {

                   // tempLine: source buffer or array from which the data is to be written
                   // 1: size of each element to be written
                   //  pos-tempLine: nº of elements to be written (length of the substring before str1)
                   // tempFile: pointer to the destination file
                   fwrite(tempLine, 1, pos - tempLine, tempFile);

                    // writes the str1 by the value of the constant (str2) in the temporary file
                    fputs(str2, tempFile);

	                // Position of the pointer is updated after the replaced str1 to continue searching
                    pos += strlen(str1);
                }

                // Write the remaining part of the line (strlen(pos)) byte by byte (1) to the temporary file
                fwrite(pos, 1, strlen(pos), tempFile);
            }

            // Pointer position to the beginning of the temporary file and the original file
            fseek(tempFile, 0, SEEK_SET);
            fseek(f, 0, SEEK_SET);
	
            // Reads each line from the temporary file
            while (fgets(tempLine, sizeof(tempLine), tempFile) != NULL) {

	            // Writes it back to the original file
                fputs(tempLine, f);
            }
            fclose(tempFile);
        }
    }
}


void defineMacros(FILE* f){

}

void defineIncludes(FILE* f){

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
