#include <stdio.h>
#define MAX_LEXEME_LENGTH 100

// List of valid token names in the language
typedef enum valid_token_names
{
    ID,
    NUM,
    RNUM,
    AND,
    OR,
    TRUE,
    FALSE,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LT,
    LE,
    GT,
    GE,
    EQ,
    NE,
    DEF,
    ENDDEF,
    DRIVERDEF,
    DRIVERENDDEF,
    COLON,
    RANGEOP,
    SEMICOL,
    COMMA,
    ASSIGNOP,
    SQBO,
    SQBC,
    BO,
    BC,
    COMMENTMARK
} valid_token_names;

// Defining token and its structure
typedef struct token
{
    valid_token_names token_name;
    union
    {
        struct
        {
            char str[MAX_LEXEME_LENGTH];
        } id;
        int numeric_value;
        double real_numeric_value;
    };
    int line_no;
} Token;
FILE *getStream(FILE *fp);

// removes comments from the testcase file and creates a new file without comments
void removeComments(char *testcaseFile, char *cleanFile);

// returns the next valid token from the file
Token get_next_token();

// returns the next character from the file
char get_next_char();