#include "lexer.c"

// data structures for interpreter
int lineNo = 0; // if this is eqeual to 0 then we should execute immediatly
char* lines[10]; // preallocate enough room for 10 lines
int lineNos[10]; // store line numbers


void line(); // forward declaration
void statement(); // forward declaration
void expr_list(); // forward declaration
void expr(); // forward declaration

int main()
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else
    {
        getChar();
        do
        {
            lex();
            line();
        } while (nextToken != EOF);
    }
}
// lex() must be called before this function !!
void line() {
    if (nextToken == NUMBER) {
        lineNo = atoi(lexeme);

        // take whatevrr is left on the line and store it for later exec

        // consume the token by looking at the line number and storing 
        // the statement that follows in the right place in our stored program
        // BUT NOT FOR THIS ASSIGNMENT
        // CALL lex() to get the next token
        lex(); // need this for lab3 but not lab4
    }

    statement(); // note that statement MUST have an extra call to lex()
    if (nextToken != CR) {
        printf("Error: unexpected token found . Expected CR but found %d\n", nextToken);
    }
}

void statement() {
    switch (nextToken) {
        case PRINT:
            lex(); // consume the PRINT token
            expr_list();
            // unconditially printf("\n");
            break;

        case IF:
            lex();
            expr();

            lex();
            relop();

            lex();
            expr();
            statement();
            // we do not need an extra call to lex() here because expr() will do it for us
            break;

        case LET:
            lex(); // consume LET
            if (nextToken != IDENT) {
                printf("Error: expected IDENT token after LET, but found %d\n", nextToken);
                exit(1);
            }
            lex(); // consume IDENT
            if (nextToken != EQUALS_OP) {
                printf("Error: expected EQUALS_OP token after IDENT in LET statement, but found %d\n", nextToken);
            }
            lex(); // consume EQUALS_OP

            expr();
            break;

        case INPUT:
            lex();
            break;

        case GOTO:
            lex();

            lex(); // extra call to lex to look for the line number
            break;

        case GOSUB:
            lex();
            break;

        case RETURN:
        case CLEAR:
        case LIST:
        case RUN:
        case END:
            lex();
            break;
        default:
            printf("Error: unexpected token found in statement: %d\n", nextToken);
    } // INPUT DOES NOT NEED EXTRA CALL TO LEX ... NEITHER D
}

// mkaes an extra call to lex() to look for the comma
// lex has ALREADY been called before expr_list
void expr_list() {
    if (nextToken == STRING) {
        // do nothing for this assignment
       // printf("%s \n", lexeme);
       // printf("%s\n", lexeme);
    }
    else {
        expr();
    }
    lex();
    while (nextToken == COMMA) {
        // next assignment printf("\t");
        if (nextToken == STRING) {
            // do nothing for this assignment
           // in the next assignment we will need to print the string
        }
        else {
            expr();
        }
        lex(); // extra call to lex()
        // there are only two valid tokens AT THIS SPOT
        if (nextToken != COMMA && nextToken != CR){
            printf("Unexpected token found, expecting COMMA or CR but found %d\n", nextToken);
            exit(1);
        }
    }
}

void expr() {
    // TODO
}