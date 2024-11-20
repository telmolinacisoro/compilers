#include "./functions.h"
#include <regex.h>  

int countop = 0; // Global variable to count the number of operations inside the automata

// Token categories 
enum {
    CAT_NUMBER,
    CAT_IDENTIFIER,
    CAT_KEYWORD,
    CAT_LITERAL,
    CAT_OPERAND,
    CAT_SPECIALCHAR,
    CAT_NONRECOGNIZED,

    CAT_NOTLEXEME,
    CAT_NEWLINE
};

int stringComp(char str1[], char str2[]){ // Function to compare two strings char by char
    int i = 0, flag = 0; 
    
    while(str1[i] != '\0' && str2[i] != '\0'){ 
        if(str1[i] != str2[i]){ 
            flag = 1; 
            break; 
            } 
        i++; 
    } 

    if(flag == 0 && str1[i] == '\0' && str2[i] == '\0') return 1; 
    else return 0; 
} 

//the next function does not work, we have made one that does the same goal below
/*void strcpy1(char * s, char * p)
{
    char * temp1 = s;
    char * temp2 = p;
    while(*temp1 != '\0')
    {
        temp2 =temp1;
        temp1++;
        temp2++;
    }
    *temp2 = '\0';
}*/

void strCopy(char *dest, char *src) { // Function to copy a string to another 
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}

int strLen(char str[]){ // Function to get the length of a string
    int count = 0;
    int i = 0;
    while(str[i] != '\0'){
        count++;
        i++;
    }

    return count;
}

void strCon(char *str1, char *str2){ // Function to concatenate two strings
    while (*str1 != '\0') {
        str1++;
    }

    while (*str2 != '\0') {
        *str1 = *str2;
        str1++;
        str2++;
    }

    *str1 = '\0';
}


// Structure to represent a token
typedef struct {
    char lexeme[100]; 
    int category;
    // int line;
} Token;

void tokenize(FILE* rf, FILE* wf, int states[33][74]) { // Function to tokenize a file, it uses the automata function to determine the category of the token
    //Variables for the character reading
    char currentChar;
    char* token = NULL;
    int tokenSize = 0;
    int category;
    // Initialize variables
    Token **tokens = NULL;
    int numTokens = 0;

    printf("INSIDE TOKENIZE\n");

    //Places the char reader at the beggining of the file
    fseek(rf,0,0);
    while ((currentChar = fgetc(rf)) != EOF) {   //While that reads chars from a file
        // TODO: FALTA QUE SEPARE LOS CARACTERES ESPECIALES, Y CREAR UNA CATEGORIA PARA LOS SALTOS DE LINEA Y ASI PRINTARLOS EN UN ARCHIVO
        if (currentChar != ' ' && currentChar != '\n' && currentChar != '\t'){
             // Increase token size and reallocate memory
            tokenSize++;
            token = realloc(token, tokenSize * sizeof(char));
            if (token == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            // Insert current char inside token
            token[tokenSize - 1] = currentChar;
        } else {
             if (tokenSize > 0) {
                // Null-terminate the token
                token[tokenSize] = '\0';
                // Call automata with token
                category = automata(token, states);

                
                Token *newToken = malloc(sizeof(Token));
                // Copy lexeme and category to the new token
                strCopy(newToken->lexeme, token);
                newToken->category = category;

                tokens = realloc(tokens, (numTokens + 1) * sizeof(Token*));
                if (tokens == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }

                // Add the new token to the tokens array
                tokens[numTokens] = newToken;
                numTokens++;

                // Free memory occupied by token
                free(token);
                // Reset token size and allocate memory for an empty token
                tokenSize = 0;
                token = NULL;
            }
            if(currentChar == '\n'){
                /*
                Token *newToken = malloc(sizeof(Token)));
                token = malloc(sizeof("\n"));
                token
                */
            }
        }        
    }

    if (tokenSize > 0) {
        // Null-terminate the token
        token[tokenSize] = '\0';
        // Call automata with token
        category = automata(token, states);
        // Free memory occupied by token
        free(token);
    }

    printf("FUNC OK\n");

    for (int i = 0; i < numTokens; i++) {
        fprintf(wf, "<%s, %d> ", tokens[i]->lexeme, tokens[i]->category);
        printf( "<%s, %d> ", tokens[i]->lexeme, tokens[i]->category);
        free(tokens[i]); // Free memory allocated for each token
    }

    free(tokens); // Free memory allocated for the tokens array

}

int isSpecialChar(char c){ 
    if(c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '+' || c == '*' || c == '=' || c == '>'){
        return 1;
    }
    return 0;
}

int countSpecialChar(char* token){
    int len = strLen(token);
    int count = 0;
    if (len == 1){
        return count;
    }

    for(int i = 0; i<len; i++){
        if(token[i] == ';' || token[i] == ',' || token[i] == '(' || token[i] == ')' || token[i] == '{' || token[i] == '}' || token[i] == '[' || token[i] == ']' || token[i] == '+' || token[i] == '*' || token[i] == '=' || token[i] == '>'){
            count++;
        }
    }
    return count;
}

int automata(char* token, int states[33][74]) { // Function to determine the category of a token
    char c;
    int tState = 0;

    for (int i = 0; i < strLen(token);i++){
        countop++; //Count the number of operations
        c = token[i];
        printf("Taking char %c\n",c);
        tState = advState(tState,c, states);
        printf("Advancing to state %d\n",tState);
        if(tState == 33){
            return CAT_NONRECOGNIZED;
        }
    }
    printf("Total number of operations: %d\n",countop);
    countop = 0; //Reset the number of operations
    return checkState(tState);
}

int checkState(int currState) { // Function to check the state of the automata and return the category of the token
    if (currState == 26 || currState == 27) {
        return CAT_KEYWORD;
    } else if (currState == 28) {
        return CAT_IDENTIFIER;
    } else if (currState == 29) {
        return CAT_LITERAL;
    } else if (currState == 30) {
        return CAT_OPERAND;
    } else if (currState == 31) {
        return CAT_SPECIALCHAR;
    } else if (currState == 32) {
        return CAT_NUMBER;
    } else if (currState == 33) {
        return CAT_NONRECOGNIZED;
    } else {
        return CAT_NONRECOGNIZED;
    }
}

int charCol(int cAscii){ // Function to determine the column of the matrix of states for a given char in ASCII (see developer's manual for more info)
    if(cAscii==40){
        return 0;
    }else if(cAscii==41){
        return 1;
    }else if(cAscii==42){
        return 2;
    }else if(cAscii==43){
        return 3;
    }else if(cAscii==48){
        return 4;
    }else if(cAscii==49){
        return 5;
    }else if(cAscii==50){
        return 6;
    }else if(cAscii==51){
        return 7;
    }else if(cAscii==52){
        return 8;
    }else if(cAscii==53){
        return 9;
    }else if(cAscii==54){
        return 10;
    }else if(cAscii==55){
        return 11;
    }else if(cAscii==56){
        return 12;
    }else if(cAscii==57){
        return 13;
    }else if(cAscii==59){
        return 14;
    }else if(cAscii==61){
        return 15;
    }else if(cAscii==62){
        return 16;
    }else if(cAscii==65){
        return 17;
    }else if(cAscii==66){
        return 18;
    }else if(cAscii==67){
        return 19;
    }else if(cAscii==68){
        return 20;
    }else if(cAscii==60){
        return 21;
    }else if(cAscii==70){
        return 22;
    }else if(cAscii==71){
        return 23;
    }else if(cAscii==72){
        return 24;
    }else if(cAscii==73){
        return 25;
    }else if(cAscii==74){
        return 26;
    }else if(cAscii==75){
        return 27;
    }else if(cAscii==76){
        return 28;
    }else if(cAscii==77){
        return 29;
    }else if(cAscii==78){
        return 30;
    }else if(cAscii==79){
        return 31;
    }else if(cAscii==80){
        return 32;
    }else if(cAscii==81){
        return 33;
    }else if(cAscii==82){
        return 34;
    }else if(cAscii==83){
        return 35;
    }else if(cAscii==84){
        return 36;
    }else if(cAscii==85){
        return 37;
    }else if(cAscii==86){
        return 38;
    }else if(cAscii==87){
        return 39;
    }else if(cAscii==88){
        return 40;
    }else if(cAscii==89){
        return 41;
    }else if(cAscii==90){
        return 42;
    }else if(cAscii==91){
        return 43;
    }else if(cAscii==93){
        return 44;
    }else if(cAscii==97){
        return 45;
    }else if(cAscii==98){
        return 46;
    }else if(cAscii==99){
        return 47;
    }else if(cAscii==100){
        return 48;
    }else if(cAscii==101){
        return 49;
    }else if(cAscii==102){
        return 50;
    }else if(cAscii==103){
        return 51;
    }else if(cAscii==104){
        return 52;
    }else if(cAscii==105){
        return 53;
    }else if(cAscii==106){
        return 54;
    }else if(cAscii==107){
        return 55;
    }else if(cAscii==108){
        return 56;
    }else if(cAscii==109){
        return 57;
    }else if(cAscii==110){
        return 58;
    }else if(cAscii==111){
        return 59;
    }else if(cAscii==112){
        return 60;
    }else if(cAscii==113){
        return 61;
    }else if(cAscii==114){
        return 62;
    }else if(cAscii==115){
        return 63;
    }else if(cAscii==116){
        return 64;
    }else if(cAscii==117){
        return 65;
    }else if(cAscii==118){
        return 66;
    }else if(cAscii==119){
        return 67;
    }else if(cAscii==120){
        return 68;
    }else if(cAscii==121){
        return 69;
    }else if(cAscii==122){
        return 70;
    }else if(cAscii==123){
        return 71;
    }else if(cAscii==125){
        return 72;
    }else if(cAscii==34){
        return 73;
    }         
}

int advState(int currState, char c, int states[33][74]){ // Function to advance the state of the automata
    int cAscii = c; 
    int col = charCol(cAscii); // Get the column of the matrix of states for the given char
    return states[currState][col]; // Return the new state
}

void initStates(int states[33][74]){ //Initialize function for matrix of states (see diagram in developer's manual for more info)
    for (int i = 0; i < 33; i++){
        for (int j = 0; j < 74; j++){
            if(i == 0){ //For the s0 (initial state) if it is an
                if(j == 2 || j == 3 || j == 15 || j == 16){ //Operator
                    states[i][j] = 30; //It goes to state 30
                } else if(j == 0 || j == 1 || j == 14 || j == 43 || j == 44 || j == 71 || j == 72){ //special char
                    states[i][j] = 31; //It goes to state 31
                } else if(4 <= j && j <= 13 ){ //Number
                    states[i][j] = 32;
                } else if(j == 73){ //"
                    states[i][j] = 25;
                } else if(j == 47){ //c
                    states[i][j] = 11;
                } else if(j == 49){ //e
                    states[i][j] = 17;
                } else if(j == 53){ //i
                    states[i][j] = 9;
                } else if(j == 57){ //m
                    states[i][j] = 6;
                } else if(j == 62){ //r
                    states[i][j] = 1;
                } else if(j == 66){ //v
                    states[i][j] = 14;
                } else if(j == 67){ //w
                    states[i][j] = 20;
                } else{ //Any other char
                    states[i][j] = 24;
                }
            } else if(i == 1){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 49){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 49){ //e
                    states[i][j] = 2;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 2){
                if(j == 64){ //r
                    states[i][j] = 3;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 3){
                if(j == 65){ //e
                    states[i][j] = 4;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 4){
                if(j == 62){ //r
                    states[i][j] = 5;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 5){
                if(j == 58){ //u
                    states[i][j] = 27;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 6){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 45){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 45){ //m
                    states[i][j] = 7;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 7){
                if(j == 53){ //i
                    states[i][j] = 8;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 8){
                if(j == 58){ //u
                    states[i][j] = 27;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 9){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 58 && j != 50){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 58){ //u
                    states[i][j] = 10;
                } else if(j == 50){ //f
                    states[i][j] = 27;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 10){
                if(j == 64){ //r
                    states[i][j] = 26;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 11){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 52){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 52){ //c
                    states[i][j] = 12;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 12){
                if(j == 45){ //m
                    states[i][j] = 13;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 13){
                if(j == 62){ //r
                    states[i][j] = 26;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 14){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 59){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 59){ //w
                    states[i][j] = 15;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 15){
                if(j == 53){ //i
                    states[i][j] = 16;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 16){
                if(j == 48){ //f
                    states[i][j] = 26;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 17){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 56){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 56){ //m
                    states[i][j] = 18;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 18){
                if(j == 63){ //a
                    states[i][j] = 19;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 19){
                if(j == 49){ //e
                    states[i][j] = 27;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 20){ 
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90 && j != 52){ //minus
                    states[i][j] = 28; 
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else if(j == 52){ //c
                    states[i][j] = 21;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 21){
                if(j == 53){ //i
                    states[i][j] = 22;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 22){
                if(j == 56){ //m
                    states[i][j] = 23;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 23){
                if(j == 49){ //e
                    states[i][j] = 27;
                } else{ //Any other char
                    states[i][j] = 33;
                } 
            } else if(i == 24){
                if(17 <= j && j <= 42){//Mayus
                    states[i][j] = 28;
                } else if(45<= j && j <= 90){ //minus
                    states[i][j] = 28;
                } else if(4 <= j && j <= 13){//num
                    states[i][j] = 28;
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 25){
                if(j == 73){//Mayus
                    states[i][j] = 29;
                } else{ //Any other char
                    states[i][j] = 25;
                }
            } else if(i == 26){ //If it is a keyword
                states[i][j] = 33; 
            } else if(i == 27){ //If it is a keyword
                states[i][j] = 33;
            } else if(i==28){ //If it is an identifier
                if((4 <= j && j <= 13) || (45 <= j && j <= 70) || (17 <= j && j <= 42)){
                    states[i][j] = 28; 
                } else{ //Any other char
                    states[i][j] = 33;
                }
            } else if(i == 29){ //If it is a literal
                states[i][j] = 33;
            } else if(i == 30){ //If it is an operand
                states[i][j] = 33;
            } else if(i == 31){ //If it is a special char
                states[i][j] = 33;
            } else if(i == 32){ //If it is a number
                if(4 <= j && j <= 13){ //Number
                    states[i][j] = 32; 
                } else{ //Any other char
                    states[i][j] = 33;
                }
            }
        }
    }
}