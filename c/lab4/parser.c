#include "lexer.c"
#define strcpy

// https://en.wikipedia.org/wiki/Tiny_BASIC

// data structures needed for the interpreter
int lineno = 0;   // if this is equal to 0 then we should execute immediately
char* lines[1000];  // preallocate enough room for 10 lines
int linenos[1000];
int lineindex = 0; // keeps track of how many lines we have and where we are and where the next line should 
// be stored

// heres the symbol table
int symboltable[26]; // positino 0 = A , 1 = B, ... , Z = 25
int symboldefined[26]; // store a 1 in a given position if that symbol has been defined

void line();
void statement();
void expr_list();
int expression();
int term();
int factor();
void relop();

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
        int previndex = search(linenos, lineno); // has this line number been used ?
        if (previndex < 0) {
            linenos[lineindex] = lineno; // never used , store it in the next available spot
        }
        linenos[lineindex] = lineno;
        // this is our special lex_endl() function to extract the rest of the line out of the file
        lex_endl(); // sets a global variable names restOfLine that we need to store
        // in the right place in our lines data structure

        if (previndex < 0){
            // if we dont have a hit in the search for the line number then we need to store this line in the next available spot which is lineindex
            lines[lineindex] = malloc(1000); // preallocate enough room for 1000 characters in each line
            strcpy(lines[lineindex], restOfLine);
            printf("Stored this line: %s at line number: %d which is index: %d\n", restOfLine, lineno, lineindex);
            lineindex++;
        } else {
            strcpy(lines[previndex], restOfLine);
            printf("Overwriting line number: %d at index: %d with this line: %s\n", lineno, previndex, restOfLine);
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
    switch(nextToken) {
        case PRINT:
            lex(); // consume print and look for what we are going to print starting with " or the first token of an expression
            expr_list(); // unconditionally printf("\n");
            break;

        case IF:
            lex(); // consumes IF and then looks for the first token of the expression
            expression(); // expression() calls lex(); so we dont have to do it before relop()
            relop(); // relop() calls lex(); so we dont have to do it before the next expression
            expression(); // expression() calls lex(); so we dont have to do it before THEN
                if (nextToken != THEN) {
                    printf("Expecting THEN but found: %d\n", nextToken);
                }
            lex(); // consume THEN and look for the first token of the statement after THEN
            statement();
            break;

        case GOTO:
            lex(); // consume GOTO and look for the first token of the expression
            expression(); // no extra call to lex to look for the carriage return
            break;

        case INPUT:
            lex(); // consume INPUT and look for the first token of the variable list
            if (nextToken != VAR) {
                printf("Expecting IDENT but found: %d\n", nextToken);
            }
            lex(); // consume the first variable and look for the comma or carriage return
            while (nextToken == COMMA) {
                lex(); // consume the comma and look for the next variable
                lex(); // consume the second variable and look for the comma or carriage return
            }
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
        
        case GOSUB:
            lex(); // consume GOSUB and look for the first token of the expression
            expression(); // calls lex();

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
        case RUN:
            sort();
        case RETURN:
        case END:
             lex(); // this IS the extra call to lex() since nothing comes after these keywords
            break;
    }
}

// makes an extra call to lex() to look for the comma
// lex has ALREADY been called before expr_list
void expr_list() {
    if (nextToken == STRING) {
        // extra call to lex() to look for the comma or carriage return after the string
        lex();
        printf("%s\t", lexeme);
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
            lex();
            printf("%s\t", lexeme);
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

int term() {
    int result = factor();
    lex(); // look for multOP or divOP
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex(); // move past the * or /
            if (nextToken == MULT_OP) {
                result *= factor();
            } else {
                result /= factor();
            }
            lex(); // look for the next multOP or divOP
    }
    printf("What? Expecting VAR or NUMBER or LEFT_PAREN but found: %d\n", nextToken);
    return -999;
}

int factor() {
    if (nextToken == VAR || nextToken == NUMBER) {
        //do nothing for this assignment
        int pos = lexeme[0] - 'A';
        if (symboldefined[pos] == 0) {
            printf("How? Variable %c used before being defined.\n", lexeme[0]);
        }
        return symboltable[pos]; // return the value of the variable or number  
    }
    else if (nextToken == NUMBER){
        return atoi(lexeme); // convert the lexeme to an integer and return it
    }
    else if (nextToken == LEFT_PAREN){
        lex();
        expression();
        if (nextToken != RIGHT_PAREN) {
            printf("Expecting right paren but found: %d\n", nextToken);
        }
    }

    // big if statement , no extra call to lex() at the end . grabs VAR, or a NUMBER, or an (expression) (var, number or a left_praran)

    // look back at the grammar for all the possibilities for a factor ... you need if else if to handle identifiers, numbers, and parenthesized expressions
    // you should make very sure NOT to have an extra call to lex() here (finally!) because expression() and term() are the ones that need the extra calls to lex() to look for +, -, *, or /
    // return the value of the defined variable
    // placeholder return value
}

void relop() { // relationship operator
    if (nextToken == EQUALS_OP) {
        lex();
    } else if (nextToken == LT_OP) {
        lex();

        if (nextToken == EQUALS_OP || nextToken == RT_OP) {
            lex();
        }
    } else if (nextToken == RT_OP) {
        lex();
        // Check for >=
        if (nextToken == EQUALS_OP) {
            lex();
        }
    } else {
        printf("Expecting relop but found: %d\n", nextToken);
    }
}

/*
Im starting to think that i will have to have a var_list function
and possibly even a var function.

But then number doesnt require a function so maybe not ...

Keeping track of the calls to lex() is the hardest part of this assignment
*/