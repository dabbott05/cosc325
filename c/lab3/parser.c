
#include "lexer.c"

void line(); // forward declaration

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
        // consume the token by looking at the line number and storing 
        // the statement that follows in the right place in our stored program
        // BUT NOT FOR THIS ASSIGNMENT
        // CALL lex() to get the next token
        lex();
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
            break;
        case IF:
            lex();

            lex(); // extra call to lex to look for the character term
            break;
        case LET:
            lex();

            lex(); // extra call to lex to look for the expression
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