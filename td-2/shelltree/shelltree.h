/*!
 * \ingroup td_2_group
 * \file shelltree.h
 * \brief Exercise 2.5
 *
 * Builds a syntax tree and evaluates shell commands.
 *
 * \author H. Decoudras
 * \version 1
 */

#ifndef DEF_SHELLTREE_H
#define DEF_SHELLTREE_H

#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*!
 * \brief Maximum number of arguments.
 */
#define ARGS_COUNT 50

/*!
 * \brief Maximum length of an identifier.
 */
#define ID_LENGTH 500


/*!
 * \enum expression_type
 * \brief The \ref expression_type enumeration represents
 *        the type of expressions evaluated in the shell.
 */
enum expression_type
{
    /*!
     * \brief Empty shell command.
     */
    EMPTY,              

    /*!
     * \brief Simple shell command.
     */
    SIMPLE,             

    /*!
     * \brief Multiple shell commands separated
     *        by a semicolon.
     */
    SEQUENCE,           

    /*!
     * \brief Shell commands separated by the `&&`
     *        condition.
     */
    SEQUENCE_AND,

    /*!
     * \brief Shell commands separated by the `||`
     *        condition.
     */
    SEQUENCE_OR,        

    /*!
     * \brief Background shell command.
     */
    BACKGROUND,                 

    /*!
     * \brief Piped shell commands.
     */
    PIPE,               

    /*!
     * \brief Redirection of the standard input of
     *        a shell command.
     */
    REDIRECTION_I,

    /*!
     * \brief Redirection of the standard output of
     *        a shell command.
     */
    REDIRECTION_O,      

    /*!
     * \brief Redirection of the standard output of
     *        a shell command with the append mode.
     */
    REDIRECTION_A,     

    /*!
     * \brief Redirection of the standard error output of
     *        a shell command.
     */
    REDIRECTION_E,     

    /*!
     * \brief Redirection of the standard error output and
     *        the standard error output of a shell
     *        command.
     */
    REDIRECTION_EO
};


/*!
 * \brief Type definition of the \ref expression_type
 *        enumeration.
 *
 * \see expression_type
 */
typedef enum expression_type ExpressionType;


/*!
 * \struct expression
 * \brief The \ref expression structure is used
 *        to build a syntax tree of a shell
 *        command.
 * 
 *        An expression is split in multiple command types.
 *
 * \see expression_type
 */
struct expression
{
    /*!
     * Type of the expression.
     */
    ExpressionType type;

    /*!
     * \brief Split sub-expression.
     */
    struct expression* left;

    /*!
     * \brief Split sub-expression.
     */
    struct expression* right;

    /*!
     * \brief Shell command and its arguments.
     */
    char** arguments;
};


/*!
 * \brief Type definition of the \ref expression structure.
 *
 * \see expression
 */
typedef struct expression Expression;


/*!
 * \brief The yyparse() function is used by the `Bison` 
 *        parser.
 *
 *        This function reads tokens, executes actions, 
 *        and ultimately returns when it encounters 
 *        end-of-input or an unrecoverable syntax error. 
 *        You can also write an action which directs 
 *        yyparse to return immediately without reading 
 *        further.
 *
 * \return This function can return the following values:
 *          - **YYACCEPT** if parsing was successful (return is due 
 *            to end-of-input)
 *          - **YYABORT** if parsing failed because of invalid input,
 *            i.e., input that contains a syntax error or that causes
 *            `YYABORT` to be invoked
 *          - **YYNOMEM** if parsing failed due to memory exhaustion 
 */
extern int yyparse(void);

/*!
 * \brief Value of the last shell command run. 
 */
extern int status;

/*!
 * \brief The new_node() function allocates an expression.
 *
 * \param type Type of the expression.
 * \param l Split sub-expression.
 * \param r Split sub-expression.
 * \param args Shell command and its arguments.
 *
 * \return An allocated \ref expression structure.
 *
 * \see expression_type
 * \see expression
 * \see delete_node()
 */
Expression* new_node(ExpressionType type, Expression* l, 
                     Expression* r, char** args);

/*!
 * \brief The delete_node() function deallocates 
 *        an expression.
 *
 * \param e Expression to deallocate.
 */
void delete_node(Expression* e);


/*!
 * \brief The new_args_list() function allocates a
 *        list of arguments of size \ref ARGS_COUNT.
 *
 * \see ARGS_COUNT
 * \see append_to_args_list()
 * \see args_list_size()
 */
char** new_args_list(void);

/*!
 * \brief The append_to_args_list() function appends
 *        a shell command and its argument to the
 *        end of a list of arguments.
 *
 * \param l List of arguments.
 * \param arg Argument to add.
 *
 * \return The list of arguments populated with
 *         the new shell command.
 */
char** append_to_args_list(char** l, char* arg);

/*!
 * \brief The args_list_size() function gets the size of 
 *        a list of arguments.
 *
 * \param l List of arguments.
 *
 * \return The size of a list of arguments.
 */
int args_list_size(char** l);

/*!
 * \brief The end_of_file() function is used by the
 *        `Flex` lexer and exit the program when
 *        the end of the file is detected.
 */
void end_of_file(void);

/*!
 * \brief The yyerror() function reports an error
 *        when the `Bison` parser encounter an error.
 *
 * \param s Error.
 */
void yyerror(char* s);


/*!
 * \brief Processed expression.
 */
Expression* processed;


#endif // DEF_SHELLTREE_H

