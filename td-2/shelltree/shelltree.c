/*!
 * \ingroup td_2_group
 * \file shelltree.c
 * \brief Exercise 2.5
 *
 * Builds a syntax tree and evaluates shell commands.
 *
 * \author H. Decoudras
 * \version 1
 */

#include "shelltree.h"
#include "display.h"
#include "evaluator.h"

#include <readline/readline.h>
#include <readline/history.h>

#include <stdio.h>


/*!
 * \brief The yyparse_string() function parses a string.
 *
 * \param s String to be parsed.
 *
 * \return This function can return the following values:
 *          - **YYACCEPT** if parsing was successful (return is due 
 *            to end-of-input)
 *          - **YYABORT** if parsing failed because of invalid input, 
 *            i.e., input that contains a syntax error or that causes
 *            `YYABORT` to be invoked
 *          - **YYNOMEM** if parsing failed due to memory exhaustion
 */
extern int yyparse_string(char* s);


/*!
 * \brief Activates `readline`.
 */
static int interactive_mode = 1;

/*!
 * \brief The my_yyparse() function parses a string.
 *
 * \return This function can return the following values:
 *          - **YYACCEPT** if parsing was successful (return is due 
 *            to end-of-input)
 *          - **YYABORT** if parsing failed because of invalid input,
 *            i.e., input that contains a syntax error or that causes
 *            `YYABORT` to be invoked
 *          - **YYNOMEM** if parsing failed due to memory exhaustion
 */
static int my_yyparse(void);


int status = 0;


Expression* new_node(ExpressionType type, Expression* l, 
                     Expression* r, char** args)
{
    Expression* e;

    if ((e = (Expression*)malloc(sizeof(Expression))) == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->type = type;
    e->left = l;
    e->right = r;
    e->arguments = args;
    return e;
}

void delete_node(Expression* e)
{
    if (e == NULL)
    {
        return;
    }

    delete_node(e->left);
    delete_node(e->right);

    if (e->arguments != NULL)
    {
        for (int i = 0; e->arguments[i] != NULL; i++)
        {
            free(e->arguments[i]);
        }

        free(e->arguments);
    }

    free(e);
}

char** new_args_list(void)
{
    char** l = (char**)calloc(ARGS_COUNT + 1, sizeof(char*));
    *l = NULL;
    return l;
}

char** append_to_args_list(char** l, char* arg)
{
    char** new_l = l + args_list_size(l);
    *new_l = (char*)malloc(1 + strlen(arg));
    strcpy(*new_l++, arg);
    *new_l = NULL;
    return l;
}

int args_list_size(char** l) 
{
    char** p;
    for (p = l; *p != NULL; p++);
    return p - l;
}

void end_of_file(void)
{
    exit(EXIT_SUCCESS);
}

/* Call yyparse() */

void yyerror(char* s)
{
    fprintf(stderr, "%s\n", s);
}


/*!
 * \brief Main entry point of the program.
 *
 * Builds a syntax tree and evaluates shell commands.
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \return The following values can be returned:
 *          - [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 *            in case of success
 *          - [EXIT_FAILURE](https://man7.org/linux/man-pages/man3/exit.3.html) 
 *            in case of error
 */
int main(int argc, char** argv)
{
    using_history();
    while (1)
    {
        if (my_yyparse() == 0)
        {
            /* Analysis successful */

            print_expression(processed);
            status = evaluate_expression(processed);
            delete_node(processed);
        }
        else
        {
            /* Analysis error */
        }
    }

    return EXIT_SUCCESS;
}


int my_yyparse(void)
{
    if (interactive_mode)
    {
        char* line = NULL;
        char buffer[1024];
        snprintf(buffer, 1024, "shelltree(%d):", status);
        line = readline(buffer);
        if (line != NULL)
        {
            int ret;
            add_history(line);
            strncat(line, "\n", 1);
            ret = yyparse_string(line);
            free(line);
            return ret;
        }
        else
        {
            end_of_file();
            return -1;
        }
    }

    return yyparse();
}

