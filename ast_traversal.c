#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeADT.h"
#include "function_hash.h"
#include "symbol_hash.h"
#include "ast_traversal.h"

SYMBOL_TABLE_WRAPPER current_symbol_table_wrapper;
void populateSymboltabeValue(TREENODE datatype,SYMBOL_TABLE_VALUE value){
    if (strcmp(datatype->name, "integer") == 0)
        {
            value->isarray = false;
            value->symbol_table_value_union.not_array.type = integer;
        }
        else if (strcmp(datatype->name, "real") == 0)
        {
            value->isarray = false;
            value->symbol_table_value_union.not_array.type = real;
        }
        else if (strcmp(datatype->name, "boolean") == 0)
        {
            value->isarray = false;
            value->symbol_table_value_union.not_array.type = boolean;
        }
        else
        {
            value->isarray = true;
            TREENODE range1 = datatype->child;
            TREENODE range2 = datatype->child->next;
            TREENODE elementType = datatype->child->next->next;

            // bottom range
            if (strcmp(range1->name, "plus") == 0)
            {
                range1 = range1->child;
                value->symbol_table_value_union.array.is_bottom_sign_plus = true;
            }
            else if (strcmp(range1->name, "minus") == 0)
            {
                range1 = range1->child;
                value->symbol_table_value_union.array.is_bottom_sign_plus = false;
            }
            else
            {
                value->symbol_table_value_union.array.is_bottom_sign_plus = true;
            }
            if (strcmp(range1->name, "num") == 0)
            {
                value->symbol_table_value_union.array.bottom_range.bottom = atoi(range1->lexeme);
                value->symbol_table_value_union.array.is_bottom_dynamic = false;
            }
            else
            {
                value->symbol_table_value_union.array.bottom_range.bottom_var = range1->lexeme;
                value->symbol_table_value_union.array.is_bottom_dynamic = true;
            }

            // top range
            if (strcmp(range2->name, "plus") == 0)
            {
                range2 = range2->child;
                value->symbol_table_value_union.array.is_top_sign_plus = true;
            }
            else if (strcmp(range2->name, "minus") == 0)
            {
                range2 = range2->child;
                value->symbol_table_value_union.array.is_top_sign_plus = false;
            }
            else
            {
                value->symbol_table_value_union.array.is_top_sign_plus = true;
            }
            if (strcmp(range2->name, "num") == 0)
            {
                value->symbol_table_value_union.array.top_range.top = atoi(range2->lexeme);
                value->symbol_table_value_union.array.is_top_dynamic = false;
            }
            else
            {
                value->symbol_table_value_union.array.top_range.top_var = range2->lexeme;
                value->symbol_table_value_union.array.is_top_dynamic = true;
            }

            // element type
            if (strcmp(elementType->lexeme, "integer") == 0)
            {
                value->symbol_table_value_union.array.element_type = integer;
            }
            else if (strcmp(elementType->lexeme, "real") == 0)
            {
                value->symbol_table_value_union.array.element_type = real;
            }
            else
            {
                value->symbol_table_value_union.array.element_type = boolean;
            }
        }
}
void addListtoSymbolTable(TREENODE root)
{
    // used when root->child is id and root->child->next is datatype
    TREENODE ListHead = root;
    TREENODE temp = ListHead->child; // points to first child in Listhead
    while (temp != NULL)
    {

        TREENODE datatype = temp->next;
        SYMBOL_TABLE_VALUE value = (SYMBOL_TABLE_VALUE)malloc(sizeof(symbol_table_value));
        populateSymboltabeValue(datatype,value);
        symbol_insert(current_symbol_table_wrapper->symbol_table, temp->lexeme, value);
        temp = temp->child;
    }
}

void insert_symbol_table_at_end(SYMBOL_TABLE_WRAPPER wrapper, SYMBOL_TABLE_WRAPPER temp)
{
    if (wrapper->child == NULL)
    {
        wrapper->child = temp;
        temp->parent = wrapper;
    }
    else
    {
        SYMBOL_TABLE_WRAPPER iter = wrapper->child;
        while (iter->next != NULL)
        {
            iter = iter->next;
        }
        iter->next = temp;
        temp->next = NULL;
        temp->parent = wrapper;
    }
}

void populate_function_and_symbol_tables(TREENODE root)
{
    if (root == NULL)
    {
        // printf("Currently at %s",head->name);
        return;
    }
    else
    {
        if (strcmp(root->name, "Module1") == 0)
        {
            printf("REACHED MODULE1 NODE\n");
            FUNCTION_TABLE_VALUE value = (FUNCTION_TABLE_VALUE)malloc(sizeof(function_table_value));
            value->input_list = root->child->next;
            if (strcmp(root->child->next->next->name, "OutputPlistHead") == 0)
                value->output_list = root->child->next->next;
            else
                value->output_list = NULL;
            value->symbol_table_wrapper = (SYMBOL_TABLE_WRAPPER)malloc(sizeof(symbol_table_wrapper));
            function_table_insert(function_table, root->child->lexeme, value);
            current_symbol_table_wrapper = value->symbol_table_wrapper;
        }
        else if (strcmp(root->name, "InputPlistHead") == 0)
        {
            addListtoSymbolTable(root);
        }

        // else if (strcmp(root->name, "OutputPlistHead") == 0)
        // {

        //     addListtoSymbolTable(root);
        // }

        else if (strcmp(root->name, "DRIVER_MODULE_STMTS") == 0)
        {
            printf("REACHED DRIVER NODE\n");
            FUNCTION_TABLE_VALUE value = (FUNCTION_TABLE_VALUE)malloc(sizeof(function_table_value));
            value->input_list = NULL;
            value->output_list = NULL;
            value->symbol_table_wrapper = (SYMBOL_TABLE_WRAPPER)malloc(sizeof(symbol_table_wrapper));
            function_table_insert(function_table, "driver", value);
            current_symbol_table_wrapper = value->symbol_table_wrapper;
        }
        else if (strcmp(root->name, "DECLARESTMT") == 0)
        {
            TREENODE datatype = root->child->next;
            TREENODE idListHead = root->child;
            TREENODE temp = idListHead->child; // points to first child in idList
            while (temp != NULL)
            {
                SYMBOL_TABLE_VALUE value = (SYMBOL_TABLE_VALUE)malloc(sizeof(symbol_table_value));
                populateSymboltabeValue(datatype,value);
                symbol_insert(current_symbol_table_wrapper->symbol_table, temp->lexeme, value);
                temp = temp->child;
            }
        }
        else if (strcmp(root->name, "ITERATIVESTMT_WHILE") == 0)
        {
            SYMBOL_TABLE_WRAPPER temp = (SYMBOL_TABLE_WRAPPER)malloc(sizeof(symbol_table_wrapper));
            insert_symbol_table_at_end(current_symbol_table_wrapper, temp);
            current_symbol_table_wrapper = temp;
        }
        else if (strcmp(root->name, "ITERATIVESTMT_FOR") == 0)
        {
            SYMBOL_TABLE_WRAPPER temp = (SYMBOL_TABLE_WRAPPER)malloc(sizeof(symbol_table_wrapper));
            insert_symbol_table_at_end(current_symbol_table_wrapper, temp);
            current_symbol_table_wrapper = temp;
            SYMBOL_TABLE_VALUE value = (SYMBOL_TABLE_VALUE)malloc(sizeof(symbol_table_value));
            value->isarray = false;
            value->symbol_table_value_union.not_array.type = integer;
            symbol_insert(current_symbol_table_wrapper->symbol_table, root->child->name, value);
        }
    }
    populate_function_and_symbol_tables(root->child);
    populate_function_and_symbol_tables(root->next);
}
