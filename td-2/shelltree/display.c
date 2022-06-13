/*!
 * \ingroup td_2_group
 * \file display.c
 * \brief Exercise 2.5
 *
 * Displays a tree representation of shell commands.
 *
 * \author H. Decoudras
 * \version 1
 */

#include "display.h"
#include "shelltree.h"

#include <stdio.h>


/*!
 * \brief The indent_empty() function indents an empty 
 *        shell command.
 *
 * \param indent Number of spaces to display.
 * \param line_count Number of underline and pipe characters
 *                   to display.
 */
static void indent_empty(int indent, int line_count);

/*!
 * \brief The indent_not_empty() function indents a non 
 *         empty shell command
 *  
 *  \param indent Number of spaces to display.
 *  \param line_count Number of underline and pipe characters
 *                    to display.
 */
static void indent_not_empty(int indent, int line_count);

/*!
 * \brief Displays a tree representation of shell commands.
 *
 * \param e Shell commands.
 * \param indent Number of spaces to display.
 * \param line_count Number of underline and pipe characters
 *                   to display.
 *
 * \see indent_empty()
 * \see indent_not_empty()
 * \see expression
 */
static void print_expression_recursive(Expression* e, int indent, 
                                       int line_count);

/*!
 * \brief String representation of the type of a 
 *        shell command.
 *
 * \see expression_type
 */
static const char* string_type[] = {
    "EMPTY",            
    "SIMPLE",           
    "SEQUENCE",         
    "SEQUENCE_AND",     
    "SEQUENCE_OR",      
    "BACKGROUND",       
    "PIPE",             
    "REDIRECTION_I",    
    "REDIRECTION_O",    
    "REDIRECTION_A",    
    "REDIRECTION_E",    
    "REDIRECTION_EO"
};


void print_expression(Expression* e)
{
    print_expression_recursive(e, 4, 4);
}


void indent_empty(int indent, int line_count)
{
    for (int i = 1; i <= indent + line_count; i++)
    {
        if (i % line_count == 0)
        {
            putchar('|');
        }
        else
        {
            putchar(' ');
        }
    }
}

void indent_not_empty(int indent, int line_count)
{
    for (int i = 1; i < indent; i++)
    {
        if (i % line_count == 0)
        {
            putchar('|');
        }
        else
        {
            putchar(' ');
        }
    }

    putchar('+');
    for (int i = 2; i < line_count; i++)
    {
        if (indent % line_count == 0)
        {
            putchar('-');
        }
    }

    putchar('>');
}

void print_expression_recursive(Expression* e, int indent, int line_count)
{
    if (e == NULL)
    {
        return;
    }

    switch (e->type)
    {
        case EMPTY:
        {
            indent_not_empty(indent, line_count);
            printf("%s\n", string_type[e->type]);
            break;
        }

        case SIMPLE:
        {
            indent_not_empty(indent, line_count);
            printf("%s ", string_type[e->type]);
            for (int i = 0; e->arguments[i] != NULL; i++)
            {
                printf("[%s]", e->arguments[i]);
            }

            putchar('\n');
            break;
        }

        case REDIRECTION_I:
        case REDIRECTION_O:
        case REDIRECTION_A:
        case REDIRECTION_E:
        case REDIRECTION_EO:
        {
            indent_not_empty(indent, line_count);
            printf(
                "%s file [%s]\n", 
                string_type[e->type], 
                e->arguments[0]
            );
            print_expression_recursive(
                e->left, 
                indent + line_count, 
                line_count
            );
            break;
        }

        case BACKGROUND:
        {
            indent_not_empty(indent, line_count);
            printf("%s\n", string_type[e->type]);
            print_expression_recursive(
                e->left, 
                indent + line_count, 
                line_count
            );
            break;
        }

        default:
        {
            indent_not_empty(indent, line_count);
            printf("%s\n", string_type[e->type]);
            print_expression_recursive(
                e->left, 
                indent + line_count, 
                line_count
            );
            indent_empty(indent, line_count);
            putchar('\n');
            print_expression_recursive(
                e->right, 
                indent + line_count, 
                line_count
            );
        }
    }
}

