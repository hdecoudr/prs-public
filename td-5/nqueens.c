/*!
 * \ingroup td_5_group
 * \file nqueens.c
 * \brief Exercise 5.5
 *
 * Finds all possible solutions to the n-queens
 * problem.
 *
 * \author H. Decoudras
 * \version 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*!
 * \brief Resets the style of the font of the terminal.
 */
#define RESET "\033[0m"

/*!
 * \brief Sets the terminal font to bold and red.
 */
#define BOLD_RED "\033[1m\033[31m"


/*!
 * \brief Chessboard maximum size.
 */
#define CHESSBOARD_SIZE 16


/*!
 * \brief The use() function displays how to use the
 *        program.
 *
 *        This function always exits the program.
 *
 * \param program Name of the program.
 */
static void use(const char* program);

/*!
 * \brief The exit_on_argv_error() function exits the
 *        program if the provided arguments are not
 *        valid.
 *
 *        This function calls the use() one.
 *
 * \param argc Number of arguments of the program.
 * \param argv Arguments of the program.
 *
 * \see use()
 */
static void exit_on_argv_error(int argc, char** argv);


/*!
 * \brief Chessboard type definition.
 */
typedef int chessboard[CHESSBOARD_SIZE][CHESSBOARD_SIZE];


/*!
 * \brief The nqueens() function finds all possible solutions
 *        for the n-queens problem.
 *
 * \param chessboard_size Size of the chessboard.
 * \param row Row being analyzed.
 * \param c Chessboard.
 * \param count Number of solutions.
 *
 * \see ok()
 * \see print_chessboard()
 */
static void nqueens(int chessboard_size, int row, 
                    chessboard c, int* count);

/*!
 * \brief The ok() function checks if a queen is well placed
 *        or not.
 *
 * \param chessboard_size Size of the chessboard.
 * \param row Row being analyzed.
 * \param column Column being analyzed.
 * \param c Chessboard.
 *
 * \return This function can return the following values:
 *          - **0** if a queen is not well placed
 *          - **1** if a queen is well placed
 */
static int ok(int chessboard_size, int row, int column, chessboard c);


/*!
 * \brief Displays the chessboard on the standard output
 *        of the terminal.
 *
 *        Queens are displayed by the `Q` character.
 *
 * \param chessboard_size Size of the chessboard.
 * \param c Chessboard.
 */
static void print_chessboard(int chessboard_size, chessboard c);


/*!
 * \brief Main entry point of the program.
 *
 * Finds all possible solutions to the n-queens
 * problem.
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
    exit_on_argv_error(argc, argv);

    chessboard  c;
    memset(c, '\0', sizeof(c));
    
    int count = 0;
    int chessboard_size = atoi(argv[1]);

    nqueens(chessboard_size, 0, c, &count);
     
    fprintf(
        stdout, 
        "-------------------------\n"
        "Chessboard size: [%6d]   \n"
        "Solutions found: [%6d]   \n"
        "-------------------------\n",
        chessboard_size,
        count
    );

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(
        stderr, "Use:\n  %s <chessboard_size [<=16]>\n",
        program
    );
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc != 2)
    {
        use(argv[0]);
    }

    if (atoi(argv[1]) > CHESSBOARD_SIZE)
    {
        use(argv[0]);
    }
}

void nqueens(int chessboard_size, int row, chessboard c, int* count)
{
    for (int column = 0; column < chessboard_size; ++column)
    {
        if (ok(chessboard_size, row, column, c))
        {
            if (row == chessboard_size - 1)
            {
                #ifdef DISPLAY
                    print_chessboard(chessboard_size, c);
                #endif
	            (*count)++;
            }
            else
            {
	            c[row][column]++;
	            nqueens(chessboard_size, row + 1, c, count);
	            c[row][column] = 0;
            }        
        }
    }
}

int ok(int chessboard_size, int row, int column, chessboard c)
{
    int ro;
    int co;
 
    /* Check the left side of the row */

    for (ro = 0; ro < row; ++ro)
    {
        if (c[ro][column])
        {
            return 0;
        }
    }

    /* Check the upper left diagonal */

    for (ro = row - 1, co = column - 1; ro >= 0 && co >= 0; --ro, --co)
    {
        if (c[ro][co])
        {
            return 0;
        }
    }

    /* Check the upper right diagonal */

    for (ro = row - 1, co = column + 1; 
         ro >= 0 && co <= chessboard_size; 
         --ro, ++co)
    {
        if (c[ro][co])
        {
            return 0;
        }
    }

    return 1;
}

void print_chessboard(int chessboard_size, chessboard c)
{

    for (int i = 0; i < chessboard_size; ++i)
    {
        fprintf(stdout, "[");
        for (int j = 0; j < chessboard_size; ++j)
        {
            fprintf(stdout, " %s ", c[i][j] ? BOLD_RED "Q" RESET : "X");
        }

        fprintf(stdout, "]\n");
    }

    fprintf(stdout, "\n");
}

