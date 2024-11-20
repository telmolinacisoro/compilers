#include "./functions.h"

//Initialize Stack
Stack stackInit(Stack stack){
    /* Function stackInit(stack)
        - Input: Recieves the stack to initialize.
        - Returns: The stack initialized.

        Inside this function we allocate memory for the stack container and assign the top to 0, as there are no stack items yet.
    */
    stack.container = malloc(sizeof(StackItem));
    stack.top = 0;
    return stack;
}

//Push in stack
Stack push(Stack stack, StackItem item){
    /* Function push(stack, sItem)
        - Input: Recieves the stack, and the item that will be pushed in the stack.
        - Returns: The stack with the item pushed.

        We realloc the container, to allow a new item to be pushed inside. We then assign the stack item to the top and assign the number
        of items in the container to the top, which will be top = top + 1.
    */
    stack.container = realloc(stack.container, (stack.top + 1) * sizeof(StackItem));
    stack.container[stack.top] = item;
    stack.top++;
    return stack;
}

//Pop in stack
Stack pop(Stack stack){
    /* Function pop(stack)
        - Input: Recieves the stack where we want to pop an item
        - Returns: The stack with the item popped.

        If the top is 0, means there is nothing to pop, so we return the stack. Otherwise
        we remove one from the top, and we reallocate memory from the container, to remove one item from it. 
    */
    if(stack.top == 0){
        return stack;
    }
    stack.top--;
    stack.container = realloc(stack.container, stack.top * sizeof(StackItem));
    return stack;
}

//Return top of stack
StackItem top(Stack stack){
    /* Function top(stack)
        - Input: Recieves the stack from which we want to know the top item
        - Returns: The stack item in the top.

        If the top is 0, means there is no item, so we return a placeholder item with a character that's not inside the language, to
        show that the stack is empty. In any other case, we return the top, which will be the item in the stack container in top-1, as
        the top counter contains the number of items in the container.
    */
    StackItem sItem;
    if(stack.top == 0){
        sItem.c = 'c';
        sItem.state = 0;
        return sItem;
    } else {
        return stack.container[stack.top - 1];
    }   
}

void printStack(Stack stack){
    /* Function printStack(stack)
        - Input: Recieves the stack we want to print

        If top is bigger than 0, enters a reverse for loop that prints the whole stack. This function has been used for debugging.
    */
    if(stack.top > 0){
        for(int i = stack.top - 1; i>-1; i--){
            printf("Stack current point: %d with item <%c,%d>\n", i, stack.container[i].c, stack.container[i].state);
        }
    }    
}

//Auxiliar function used in readTokens
int checkChar(char currentChar){
    /* Function checkChar(currentChar)
        - Input: Recieves the char to check where we are inside the token
        - Returns: A int representing where we are in the token

        We divide the token with three dividers, which are the opening of the token <, the divider is the comma and > is the closing char.
        We use this idea to return if we are in the first divider, the second or the third, in any other case we return 0. This function
        is used inside the readTokens to store correctly the tokens inside the tokens list.
    */
    if(currentChar == '<'){
        return 1;
    } else if (currentChar == ','){
        return 2;
    } else if (currentChar == '>'){
        return 3;
    } else {
        return 0;
    }
}

Language initLanguage(){
    /* Function initLanguage()
        - Returns: The language initialized.

        We initialize inside this function a language, storing the terminal and non-terminals signs as well as the production rules.
        This language is what we return.
    */

    Language myLanguage;

    char nonTerminalSigns[4] = {'s', 'e', 't', 'f'};
    char terminalSigns[5] = {'+', '*', '(', ')', 'N'};

    strcpy(myLanguage.termSign, terminalSigns);
    strcpy(myLanguage.nonTermSigns, nonTerminalSigns);

    ProdRule productionRules[7] = {
        {'s', {'e', '\0', '\0'}, 1}, // ProdRule 1
        {'e', {'e', '+', 't'}, 3}, // ProdRule 2
        {'e', {'t', '\0', '\0'}, 1}, // ProdRule 3
        {'t', {'t', '*', 'f'}, 3}, // ProdRule 4
        {'t', {'f', '\0', '\0'}, 1}, // ProdRule 5
        {'f', {'(', 'e', ')'}, 3}, // ProdRule 6 
        {'f', {'N', '\0', '\0'}, 1} // ProdRule 7
    };

    // Copy production rules into myLanguage
    for (int i = 0; i < 7; ++i) {
        myLanguage.prodRules[i] = productionRules[i];
    }

    return myLanguage;
}

//Read tokens from a file
Token** readTokens(FILE* f){
    /* Function readTokens(file)
        - Input: Recieves the file from which we read the tokens.
        - Returns: A list of the tokens that were inside the file.

        In this function we read char by char from the input file, and use checkChar to know what exactly are we reading. We allocate memory
        for every token read and reallocate memory in the list. Then store the token inside the list. When all tokens are read, we return
        the list of tokens.
    */
    // Save all variables
    char currentChar = 'c';
    int state;
    int laststate;

    Token** tokens = NULL;
    int numTokens = 0;


    char lexeme;
    int category;
    int tokenSize;
    // Check file char by char
    fseek(f,0,0);
    while (1) {
        currentChar = fgetc(f);
        if(currentChar == EOF){
            break;
        }
        // Check if the first char is '<' '>' or ','
        state = checkChar(currentChar);
        if(state == 1){
            laststate = 1;            
        } else if (state == 2){
            laststate = 2;
        } else if(state == 3){
            laststate = 3;
        }
        else {
            // If the last char was '<', save the lexeme in the token
            if(laststate == 1){
                lexeme = currentChar;
            } else if (laststate == 2) {
                // If the last char was ',' save the current category and the lexeme in the token
                Token* newToken = malloc(sizeof(Token));

                newToken->lexeme = lexeme;

                newToken->category = currentChar - '0';

                // Save the token in a list
                tokens = realloc(tokens, (numTokens + 1) * sizeof(Token*));
                
                tokens[numTokens] = newToken;
                numTokens++;
            }
        }
    }
    return tokens;
}

/*Print language*/
void printLanguage(const Language *lang) {
    /*
    Function printLanguage(*lang)
    - Parameters:
        - lang: A pointer to the Language structure containing the language to be printed.

    Inside this function we print the language, that includes the terminal signs, non-terminal signs, and production rules.
*/
    printf("Terminal Signs: ");
    for (int i = 0; i < 5; ++i) {
        printf("%c ", lang->termSign[i]);
    }
    printf("\nNon-Terminal Signs: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c ", lang->nonTermSigns[i]);
    }
    printf("\nProduction Rules:\n");
    for (int i = 0; i < 7; ++i) {
        printf("ProdRule %d: lhs=%c, rhs=%c%c%c\n", i+1, lang->prodRules[i].lhs,
               lang->prodRules[i].rhs[0], lang->prodRules[i].rhs[1], lang->prodRules[i].rhs[2]);
    }
}

void shiftReduceAutomata(Stack stack, FILE* f, Language l, Token** tokens, int m[23][10]){

    /*
    Function shiftReduceAutomata(Stack stack, FILE* f, Language l, Token** tokens, int m[23][10])
    - Parameters:
        - stack: Stack used in the shift-reduce automaton.
        - f: File pointer for output.
        - l: Language structure containing production rules.
        - tokens: Pointer to an array of pointers to tokens.
        - m: 2D array representing the transition table for the automaton.

    With this function we implement a shift-reduce automaton to parse tokens according to a given language's production rules.
*/
    int state = 0;
    int lastState = 0;
    int i = 0;

    char lhs;
    char rhs[3];
    int rhsNum;

    int go;

    Token token;
    StackItem sItem;
    token = *tokens[i];
    
    sItem.c = 's';
    sItem.state = 0;
    stack = push(stack, sItem);
    
    fseek(f,0,0);

    while(sItem.state != 50){
        token = *tokens[i];
        fprintf(f, "\n");
        lastState = state;
        state = m[state][charToCol(token.lexeme)];
        
        if(state == 50) {
            fprintf(f, "INPUT ACCEPTED\n");
            break;
        }
        
        if(state == 0){
            fprintf(f, "Input not valid\n");
            return;
        }
        if(state > 30){
            rhsNum = l.prodRules[state-30-1].rhsNum;
            for(int p = 0; p < rhsNum; p++){
                printPop(sItem, f);
                stack = pop(stack);
                sItem = top(stack);
            }
            lhs = l.prodRules[state-30-1].lhs;
            go = m[top(stack).state][charToCol(lhs)];
            sItem.c = lhs;
            sItem.state = go;
            stack = push(stack, sItem);
            sItem = top(stack);
            printReduce(sItem, f, state);
            state = go;
        }else{
            sItem.c = token.lexeme;
            sItem.state = state;
            stack = push(stack, sItem);
            sItem = top(stack);
            printShift(sItem, f, lastState);
            printPush(sItem, f);
            i++;
        } 
    }
}


void printPush(StackItem sItem, FILE* f){
    /*
    Function printPush(StackItem sItem, FILE* f)
    - Parameters:
        - sItem: StackItem to be pushed.
        - f: FILE pointer for output.

    With this function we print the item that is being pushed inside the stack.
*/
    fprintf(f, "Pushing the sItem[char: %c | state: %d].\n", sItem.c, sItem.state);
}

void printPop(StackItem sItem, FILE* f){
    /*
    Function printPop(StackItem sItem, FILE* f)
    - Parameters:
        - sItem: StackItem to be popped.
        - f: FILE pointer for output.

    With this function we print the item that is being poped in the stack.
*/
    fprintf(f, "Doing the pop from sItem[char: %c | state: %d].\n", sItem.c, sItem.state);
}

void printReduce(StackItem sItem, FILE* f, int state){
    /*
    Function printReduce(StackItem sItem, FILE* f, int state)
    - Parameters:
        - sItem: StackItem after reduction.
        - f: FILE pointer for output.
        - state: Current state of the automaton.

    With this function we print a message that indicates the reduction.
*/
    fprintf(f, "After the pop(s) using rule %d reducing rhs to the lhs and pushing sItem[char: %c | state: %d].\n", state-30-1, sItem.c, sItem.state);
}

void printShift(StackItem sItem, FILE* f, int state){
    /*
    Function printShift(StackItem sItem, FILE* f, int state)
    - Parameters:
        - sItem: StackItem after shift.
        - f: FILE pointer for output.
        - state: Current state of the automaton.

    This function indicates shifting from the current state. 
*/
    fprintf(f, "Shifting with current state: %d.\n", state);
}



//Returns column from the dfa shift reduce automata table corresponding to the characters ascii
int charToCol(char c){
    /*
    Function charToCol(char c)
    - Parameters:
        - c: Character for which column number is to be returned.

    Returns the column number from the DFA shift reduce automata table corresponding to the ASCII value of the character.
*/
    if(c == 43){
        return 0;
    } else if(c == 42){
        return 1;
    } else if(c == 40){
        return 2;
    } else if(c == 41){
        return 3;
    } else if(c < 58 && c > 47){
        return 4;
    } else if(c == 36){
        return 5;
    } else if(c == 115){
        return 6;
    } else if(c == 101){
        return 7;
    } else if(c == 116){
        return 8;
    } else if(c == 102){
        return 9;
    }
}

//Initialization of dfa shift reduce automaton table
void initShiftReduceMatrix(int sRMatrix[23][10]){
    /*
    Function initShiftReduceMatrix(int sRMatrix[23][10])
    - Parameters:
        - sRMatrix: 2D array representing the DFA shift reduce automata table.

    Initializes the DFA shift reduce automata table with transition states.
*/
    sRMatrix[0][2] = 5;
    sRMatrix[0][4] = 6;
    sRMatrix[0][6] = 1;
    sRMatrix[0][7] = 2;
    sRMatrix[0][8] = 3;
    sRMatrix[0][9] = 4;
    sRMatrix[1][5] = 50;
    sRMatrix[2][0] = 7;
    sRMatrix[2][5] = 31;
    sRMatrix[3][0] = 33;
    sRMatrix[3][1] = 8;
    sRMatrix[3][5] = 33;
    sRMatrix[4][0] = 35;
    sRMatrix[4][1] = 35;
    sRMatrix[4][5] = 35;
    sRMatrix[5][2] = 12;
    sRMatrix[5][4] = 13;
    sRMatrix[5][7] = 9;
    sRMatrix[5][8] = 10;
    sRMatrix[5][9] = 11;
    sRMatrix[6][0] = 37;
    sRMatrix[6][1] = 37;
    sRMatrix[6][5] = 37;
    sRMatrix[7][2] = 5;
    sRMatrix[7][4] = 6;
    sRMatrix[7][8] = 14;
    sRMatrix[7][9] = 4;
    sRMatrix[8][2] = 5;
    sRMatrix[8][4] = 6;
    sRMatrix[8][9] = 15;
    sRMatrix[9][0] = 17;
    sRMatrix[9][3] = 16;
    sRMatrix[10][0] = 33;
    sRMatrix[10][1] = 18;
    sRMatrix[10][3] = 33;
    sRMatrix[11][0] = 35;
    sRMatrix[11][1] = 35;
    sRMatrix[11][3] = 35;
    sRMatrix[12][2] = 12;
    sRMatrix[12][4] = 13;
    sRMatrix[12][7] = 19;
    sRMatrix[12][8] = 10;
    sRMatrix[12][9] = 11;
    sRMatrix[13][0] = 37;
    sRMatrix[13][1] = 37;
    sRMatrix[13][3] = 37;
    sRMatrix[14][0] = 32;
    sRMatrix[14][1] = 8;
    sRMatrix[14][5] = 32;
    sRMatrix[15][0] = 34;
    sRMatrix[15][1] = 34;
    sRMatrix[15][5] = 34;
    sRMatrix[16][0] = 36;
    sRMatrix[16][1] = 36;
    sRMatrix[16][5] = 36;
    sRMatrix[17][2] = 12;
    sRMatrix[17][4] = 13;
    sRMatrix[17][8] = 20;
    sRMatrix[17][9] = 11;
    sRMatrix[18][2] = 12;
    sRMatrix[18][4] = 13;
    sRMatrix[18][9] = 21;
    sRMatrix[19][0] = 17;
    sRMatrix[19][3] = 22;
    sRMatrix[20][0] = 32;
    sRMatrix[20][1] = 18;
    sRMatrix[20][3] = 32;
    sRMatrix[21][0] = 34;
    sRMatrix[21][1] = 34;
    sRMatrix[21][3] = 34;
    sRMatrix[22][0] = 36;
    sRMatrix[22][1] = 36;
    sRMatrix[22][3] = 36;
}

//Lab 2 functions
int stringCompare(char str1[], char str2[]){ 
    /*
    Function stringCompare(char str1[], char str2[])
    - Parameters:
        - str1: First string to compare.
        - str2: Second string to compare.

    We compare two strings. We return 1 if they are equal, otherwise we return 0.
*/
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

void strCopy(char *dest, char *src)
{
    /*
    Function strCopy(char *dest, char *src)
    - Parameters:
        - dest: Destination string where the content of source string will be copied.
        - src: Source string to copy from.

    We copy the content of one string to another.
*/
    while(*src != '\0')
    {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}

int strLen(char str[]){ // Function to get the length of a string
/*
    Function strLen(char str[])
    - Parameters:
        - str: String whose length is to be determined.

    Calculate the length of a string.
*/
    int count = 0;
    int i = 0;
    while(str[i] != '\0'){
        count++;
        i++;
    }

    return count;
}

void strCon(char *str1, char *str2){ // Function to concatenate two strings
/*
    Function strCon(char *str1, char *str2)
    - Parameters:
        - str1: First string.
        - str2: Second string.

    It is useful to concatenate the second string to the end of the first string.
*/
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