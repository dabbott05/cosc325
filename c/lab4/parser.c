#include "lexer.c"

// https://en.wikipedia.org/wiki/Tiny_BASIC

// data structures needed for the interpreter
int lineno = 0;   // if this is equal to 0 then we should execute immediately
char* lines[1000];  // preallocate enough room for 10 lines
int linenos[1000];
int lineindex = 0; // keeps track of how many lines we have and where we are and where the next line should 
// be stored
int linei = 0; // keeops track of which lineindex we are on while running a program
int reti = 0; // keeps track of the index we should return too after a gosub is executed

// heres the symbol table
int symboltable[26]; // positino 0 = A , 1 = B, ... , Z = 25
int symboldefined[26]; // store a 1 in a given position if that symbol has been defined

void line();
void statement();
void expr_list();
int expression();
int term();
int factor();
int relop();
void var_list();

void killIf(){
    while (nextChar != '\n' && nextChar != 0){
        getChar();
    }
    lex();
}

// hardcoded swao of the positions to be applied in the 2 different data strructures
void swap(int j, int k){
    char* tmpc = lines[j];
    lines[j] = lines[k];
    lines[k] = tmpc;

    // now lets take care of swapping the ints in the linenos array
    int tmp = linenos[j];
    linenos[j] = linenos[k];
    linenos[k] = tmp;
}

// insertion sort
void sort() {
    int i = 1;
    while (i < lineindex) {
        int j = i;
        while (j > 0 && linenos[j - 1] > linenos[j]) {
            swap(j, j-1);
            j = j-1;
        }
        i = i+1;
    }
}

//search thorugh linenos and retunr index if found , otherwise return -1
int search(int lineno) {
    for (int i=0; i < lineindex; i++) {
        if (linenos[i] == lineno) {
            return i;
        }
    }
    return -1;
}

int findLine(int lineno){
    int hit = search(lineno);
    if (hit < 0) {
        printf("Could not find line number %d\n" , lineno);
    }
    return hit;
}

/******************************************************/
/* main driver */
int main()
{
// prep the symbol table
for (int i = 0; i < 26; i++) {
    symboldefined[i] = 0;
}

  /* Open the input data file and process its contents */
if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
else
{
    getChar();
    do
    {
    lex(); // prime the pump so we can consume line number
    line();
    } while (nextToken != EOF);
}
}

// lex() MUST be called before this function
void line() {
    if (nextToken == NUMBER) {
        lineno = atoi(lexeme);
        int previndex = search(lineno); // has this line number been used ?
        if (previndex < 0) {
            linenos[lineindex] = lineno; // never used , store it in the next available spot
            lex_endl();
            lines[lineindex] = malloc(1000); // preallocate enough room for 1000 characters in each line
            strcpy(lines[lineindex], restOfLine);
            lineindex++;
        }
        else{
            // this line number has been used before so we need to overwrite the line that was previously there
            lex_endl();
            strcpy(lines[previndex], restOfLine);
        }
    }
        else{
            statement(); // our nextToken starts the statement - as per grammar rules
    }
    if (nextToken != CR && nextToken != EOF) {
        printf("***************Expecting CR, but found: %d instead!***************\n", nextToken);
    }
    }

// lex() MUST have already been called before here
void statement() {
    int targetlineno; // this is only used for GOTO and GOSUB
    int if_a;
    int if_b;
    int op;
    switch(nextToken) {
        case PRINT:
            lex(); // consume print and look for what we are going to print starting with " or the first token of an expression
            expr_list(); // unconditionally printf("\n");
            break;

        case IF:
            lex(); // consumes IF and then looks for the first token of the expression
            if_a = expression(); // expression() calls lex(); so we dont have to do it before relop()
            op = relop(); // relop() calls lex(); so we dont have to do it before the next expression
            if_b = expression();
                if (nextToken == THEN) {
                    lex();
                }
            switch(op) {
                case 0:
                    if (if_a < if_b) {
                        statement();
                    }
                    else {
                        killIf();
                    }
                    break;
                case 1:
                    if (if_a > if_b) {
                        statement();
                    }
                    else {
                        killIf();
                    }
                    break;
                case 2:
                    if (if_a == if_b) {
                        statement();
                    }
                    else {
                        killIf();
                    }
                    break;
                case 3:
                    if (if_a <= if_b) {
                        statement();
                    }
                    else {
                        killIf();
                    }
                    break;
                case 4:
                    if (if_a >= if_b) {
                        statement();
                    }
                    else{
                        killIf();
                    }
                    break;
                case 5:
                    if (if_a != if_b) {
                        statement();
                    }
                    else {
                        killIf();
                        // force the lexer to read to the end of the line
                    }
                    break;
            }
            break;

        case INPUT:
            lex(); // consume INPUT 
            var_list(); // This will trigger the scanf for each variable in the list
            break;

        case LET:
            lex(); // consume LET and look for the token of the first variable
            if (nextToken != VAR) {
                printf("Expecting IDENT but found: %d\n", nextToken);
            }
            int pos = lexeme[0] - 'A';
            symboldefined[pos] = 1; // mark this symbol as defined in the symboldefined array

            lex(); // consume the variable and look for the equals sign
            if (nextToken != EQUALS_OP) {
                printf("Expecting EQ but found: %d\n", nextToken);
            }
            lex(); // consume the equals sign and look for the first token of the expression
            symboltable[pos] = expression(); // calls lex();

            break;

        case GOTO:
            lex(); // consume GOTO and look for the first token of the expression
            targetlineno = expression(); // no extra call to lex to look for the carriage return
            // find the linei we are supposed to GOTO
            linei = findLine(targetlineno)-1;
            break;
        
        case GOSUB:
            lex(); // consume GOSUB and look for the first token of the expression
            targetlineno = expression(); // calls lex();
            reti = linei; // save the index of the line we should return to
            linei = findLine(targetlineno)-1; // remember this is from a recursive call where we are 
            // in the middle of a for loop that will execute linei++ so we need to set it to the 
            // targetindex -1

            break;
            
        case CLEAR:
            lineindex = 0; // reset the line index to 0 so that we will overwrite all the lines we have stored so far
            lex();
            break;
        case LIST:
            sort();
            for (int i = 0; i < lineindex; i++) {
                printf("%d: %s\n", linenos[i], lines[i]);
            }
            lex();
            break;
        case RUN:
            sort();
            for(linei=0; linei < lineindex; linei++){
                in_str = lines[linei];
                stri = 0;
                getChar();
                lex();
                line();
            }
            // update lexer to read in a string instead of always reading in a file
            // do this by adding a flag variable to the lexer to indicate wether it should be grabbing the nextToken from a file or from 
            // a string also add a function that will set a global string to be processed and simultaneously set the flag
            // we need to clear the flag after the program has run and set it to continue reading from the file
            lex();
            break;
        case RETURN:
            linei = reti;
            lex();
            break;
        case REM:
            killIf();
            lex();
            break;
        case END:
            linei = lineindex;
            stri = -1; // this puts us back into file reading mode
            lex(); // this IS the extra call to lex() since nothing comes after these keywords
            break;
    }
}

// makes an extra call to lex() to look for the comma
// lex has ALREADY been called before expr_list
void expr_list() {
    if (nextToken == STRING) {
        // extra call to lex() to look for the comma or carriage return after the string
        lexeme[strlen(lexeme)-1] = '\0';
        printf("%s\t", lexeme + 1);
        lex();
        // do nothing else for this assignment
        // but in the next assignment you will need to print something!
    } else {
        printf("%d\t", expression());
        // expression ends with an extra call to lex() so we are already looking for the comma or carriage return by the time we get back here
    }
    while (nextToken == COMMA) {
        lex(); // move past the comma (i.e., consume the comma by looking for the next token after the comma)
        // next assignment: printf("\t");
        if (nextToken == STRING) {
            // extra call to lex() to look for the comma or carriage return after the string
            lexeme[strlen(lexeme)-1] = '\0';
            printf("%s\t", lexeme + 1);
            lex();
            // do nothing else for this assignment
            // but in the next assignment you will need to print something
        } else {
            printf("%d\t", expression());
            // no extra call to lex() here because expression() will have already called lex() for us when it was looking for +, -, *, or /
        }
        // there are only two valid tokens AT THIS SPOT
        if (nextToken != COMMA && nextToken != CR) {
            printf("Expecting COMMA or CR but found: %d\n", nextToken);
        }
    }
    printf("\n");
}

int expression() {
    if(nextToken == ADD_OP || nextToken == SUB_OP) {
        lex(); // move past the leading + or - if it was there otherwise, the current nextToken is part of the term so no need to call lex()
    }
    int result = term();
    // no need to call lex() here because term() will have already called lex() for us when it was looking for * or /
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        int op = nextToken;
        lex(); // move past the + or -
        if (nextToken == ADD_OP) {
            result += term();
        } else {
            result -= term();
        // remember, term() will have already called lex() for us when it was looking for * or / so no need to call it again here
    }
}
    return result;
    // no need for extra call to lex() here because the while loop will have already called lex() for us when it was looking for + or -
}

void var_list() {
    int varpos[26];
    int varcnt = 0;
    if (nextToken != VAR) {
        printf("Expecting VAR but found %d\n", nextToken);
    }
    else {
        varpos[varcnt++] = lexeme[0] - 'A';
        lex();
    }
    while (nextToken == COMMA) {
        lex();
        if (nextToken != VAR) {
            printf("Expecting VAR but found: %d\n", nextToken);
        }
        else {
            varpos[varcnt++] = lexeme[0] - 'A';
            lex();
            
        }
    }
    // we need to use scanf to read from the console
    // and convert whatever they type in into a number
    // and then store that number into the symboltable
    // at the right spot
    // varpos[1] we found during parsing
    for (int i = 0; i < varcnt; i++) {
        scanf("%d", &symboltable[varpos[i]]); // direct memory address of where the VAR is
        symboldefined[varpos[i]] = 1;
    }
}

int term() {
    int result = factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        int op = nextToken;
        lex(); // move past the * or /
            if (op == MULT_OP) {
                result *= factor();
            } else {
                int denom = factor();
                if (denom != 0) {
                    result /= denom;
                }
            }
    }
    return result;
}

int factor() {
    int result = 0; // placeholder value
    if (nextToken == VAR){
        //do nothing for this assignment
        int pos = lexeme[0] - 'A';
        result = symboltable[pos];
        lex();
    }
    else if (nextToken == NUMBER){
        result = atoi(lexeme);
        lex();
    }
    else if (nextToken == LEFT_PAREN){
        lex();
        result = expression();
        lex();
    }
    return result;

    // big if statement , no extra call to lex() at the end . grabs VAR, or a NUMBER, or an (expression) (var, number or a left_praran)

    // look back at the grammar for all the possibilities for a factor ... you need if else if to handle identifiers, numbers, and parenthesized expressions
    // you should make very sure NOT to have an extra call to lex() here (finally!) because expression() and term() are the ones that need the extra calls to lex() to look for +, -, *, or /
    // return the value of the defined variable
    // placeholder return value
}

// 0: <
// 1: >
// 2: ==
// 3: <=
// 4: >=
// 5: <> or ><
int relop() { // relationship operator - this was confusing
    if (nextToken == EQUALS_OP) {
        lex();
        return 2;
    } else if (nextToken == LT_OP) {
        lex();
        if (nextToken == EQUALS_OP) {
            lex(); return 3;
        } else if (nextToken == RT_OP) {
            lex(); return 5;
        }
        return 0;
    } else if (nextToken == RT_OP) {
        lex();
        if (nextToken == EQUALS_OP) {
            lex(); return 4;
        } else if (nextToken == LT_OP) {
            lex(); return 5;
        }
        return 1;
    }
    return -1;
}