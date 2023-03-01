/*
Group - 41
Praneet Karna - 2020A7PS1202P
Kshitij Tandon - 2020A7PS0972P
Devashish Siwatch - 2020A7PS0113P
Samyak Jain - 2020A7PS0089P
Chaitanya Iyer - 2020A7PS0012P
*/

#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include <time.h>
#include <stdlib.h>

char *enum_to_token_name_string[] = {
    "ID",
    "NUM",
    "RNUM",
    "AND",
    "OR",
    "TRUE",
    "FALSE",
    "PLUS",
    "MINUS",
    "MUL",
    "DIV",
    "LT",
    "LE",
    "GT",
    "GE",
    "EQ",
    "NE",
    "DEF",
    "ENDDEF",
    "DRIVERDEF",
    "DRIVERENDDEF",
    "COLON",
    "RANGEOP",
    "SEMICOL",
    "COMMA",
    "ASSIGNOP",
    "SQBO",
    "SQBC",
    "BO",
    "BC",
    "INTEGER",
    "REAL",
    "BOOLEAN",
    "OF",
    "ARRAY",
    "START",
    "END",
    "DECLARE",
    "MODULE",
    "DRIVER",
    "PROGRAM",
    "GET_VALUE",
    "PRINT",
    "USE",
    "WITH",
    "PARAMETERS",
    "TAKES",
    "INPUT",
    "RETURNS",
    "FOR",
    "IN",
    "SWITCH",
    "CASE",
    "BREAK",
    "DEFAULT",
    "WHILE",
    "EOFILE"
    // "COMMENTMARK"
};

// saving all the tokens generated by the lexer in a file
void save_all_tokens(FILE *input_file, FILE *output_file)
{
    initialize_lexer_variables(input_file);
    Token token;
    while (1)
    {
        token = get_next_token(input_file);
        if (token.token_name == EOFILE)
        {
            break;
        }
        printf("\n%d\t%s\t%s\n", token.line_no, enum_to_token_name_string[token.token_name], token.id.str);
        fprintf(output_file, "%s\t\t%d\t\t%s\n", enum_to_token_name_string[token.token_name], token.line_no, token.id.str);
        //  break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("\033[31mPlease provide correct number of arguments\n\033[0m");
        return 1;
    }
    FILE *input_file, *output_file;
    input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("\033[31mUnable to open input file\033[0m\n");
        return 1;
    }
    output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        printf("\033[31mUnable to open output file\033[0m\n");
        return 1;
    }
    // BUFFER_SIZE = atoi(argv[3]);
    // MAX_LEXEME_LENGTH = BUFFER_SIZE/2;
    BUFFER_SIZE = atoi(argv[3]);
    MAX_LEXEME_LENGTH = BUFFER_SIZE/2;
    printf("BUFFER_SIZE = %d, MAX_LEXEME_LENGTH = %d", BUFFER_SIZE, MAX_LEXEME_LENGTH);
    
    int choice;
    while (1)
    {
        printf("\nChoose an option:\n");
        printf("0. Exit\n");
        printf("1. Remove Comments \n");
        printf("2. Run lexer (Print Tokens) \n");
        printf("3. Run parser (Print Parse Tree) \n");
        printf("4. Find total time taken by lexer and parser \n\n");

        scanf("%d", &choice);
        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            char ch;
            input_file = fopen(argv[1], "r");
            // Open the input file in read mode
            // open the file to save the tokens generated by the lexer
            FILE *comment_file = fopen("removed_comments.txt", "w");
            // If the file doesn't exist or can't be opened, exit the program
            if (comment_file == NULL)
            {
                printf("Unable to open file");
                return 1;
            }
            printf("Output File opened successfully\n");

            removeComments(input_file, comment_file);
            fclose(input_file);
            fclose(output_file);
        }
        else if (choice == 2)
        {
            char ch;
            input_file = fopen(argv[1], "r");
            // open the file to save the tokens generated by the lexer
            FILE *token_file = fopen("token_output.txt", "w");
            // If the file doesn't exist or can't be opened, exit the program
            if (token_file == NULL)
            {
                printf("Unable to open file");
                return 1;
            }
            printf("Token output File opened successfully\n");
            save_all_tokens(input_file, token_file);

            // Close the file
            fclose(token_file);
        }
        else if (choice == 3)
        {
            FILE *input_file;
            input_file = fopen(argv[1], "r");

            if (input_file == NULL)
            {
                printf("Unable to open file");
                return 1;
            }

            int parse = parser_complete_functionality(input_file);

        }
        else if (choice == 4)
        {
            FILE *input_file;

            // Open the input file in read mode
            input_file = fopen(argv[1], "r");

            // If the file doesn't exist or can't be opened, exit the program
            if (input_file == NULL)
            {
                printf("Unable to open file");
                return 1;
            }
            FILE* token_file = fopen("token_output.txt", "w");
            // If the file doesn't exist or can't be opened, exit the program
            if (token_file == NULL)
            {
                printf("Unable to open file");
                return 1;
            }
            clock_t start_time, end_time;

            double total_CPU_time, total_CPU_time_in_seconds;

            start_time = clock();

            // TODO: Run lexer and parser
            int parser = parser_complete_functionality(input_file);
            
            end_time = clock();

            total_CPU_time = (double)(end_time - start_time);

            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("Total CPU time = %lf\n", total_CPU_time);
            printf("Total CPU time in seconds = %lf\n", total_CPU_time_in_seconds);
            // Close the file
            fclose(input_file);
            fclose(token_file);
        }
        else
        {
            printf("\n\033[31mInvalid choice\033[0m\n");
        }
    }

    return 0;
}