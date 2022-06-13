/*!
 * \ingroup td_5_group
 * \file nqueensthrd.c
 * \brief Exercise 5.5
 *
 * Finds all possible solutions to the n-queens
 * problem using threads.
 *
 * The argument of the program determines the number
 * of threads used and the size of the chessboard.
 *
 * This program uses the following functions:
 *
 *  - [pthread_create(pthread_t\* thread, const pthread_attr_t\* attr, void\* (\*start_routine)(void\*), void\* arg](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
 *  - [pthread_join(pthread_t thread, void\*\* retval)](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
 *  - [pthread_mutex_lock(pthread_mutex_t\* mutex)](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
 *  - [pthread_mutex_unlock(pthread_mutex_t\* mutex)](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
 * 
 * \author H. Decoudras
 * \version 1
 */

#include <pthread.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
 * \brief The exit_on_error() function exits the program
 *        if the \p assertion parameter is evaluated
 *        to `TRUE`.
 *
 * If the assertion is evaluated to `TRUE`, then the error 
 * number and its associated message are displayed.
 *
 * \param assertion Assertion to be evaluated.
 */
static void exit_on_error(int assertion);


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
static int ok(int chessboard_size, int row, int column, chessboard e);

/*!
 * \brief The worker() function determines if a configuration
 *        of the n-queens problem is valid or not.
 *
 * \param p The thread number.
 *
 * \return Always `NULL`.
 *
 * \see nqueens()
 */
static void* worker(void* p);

/*!
 * \brief Number of possible solutions.
 */
static int count = 0;

/*!
 * \brief Number of threads.
 */
static int threads_count;

/*!
 * \brief Mutex used to secure the access to
 *        the \ref count variable.
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/*!
 * \brief Main entry point of the program.
 * 
 * Finds all possible solutions to the n-queens
 * problem using threads.
 *
 * The argument of the program determines the number
 * of threads used and the size of the chessboard.
 *
 * This program uses the following functions:
 *
 *  - [pthread_create(pthread_t\* thread, const pthread_attr_t\* attr, void\* (\*start_routine)(void\*), void\* arg](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
 *  - [pthread_join(pthread_t thread, void\*\* retval)](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
 *  - [pthread_mutex_lock(pthread_mutex_t\* mutex)](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
 *  - [pthread_mutex_unlock(pthread_mutex_t\* mutex)](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
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

    threads_count = atoi(argv[1]);

    chessboard  c;
    memset(c, '\0', sizeof(c));
    
    
    int         indexes[threads_count];
    pthread_t   threads[threads_count];

    for (int i = 0, result; i < threads_count; ++i)
    {
        indexes[i] = i;
        result = pthread_create(&threads[i], NULL, worker, &indexes[i]);
        exit_on_error(result);
    }

    for (int i = 0, result; i < threads_count; ++i)
    {
        result = pthread_join(threads[i], NULL);
        exit_on_error(result);
    }
     
    fprintf(
        stdout, 
        "-------------------------\n"
        "Chessboard size: [%6d]   \n"
        "Solutions found: [%6d]   \n"
        "-------------------------\n",
        threads_count,
        count
    );

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(
        stderr, 
        "Use:\n  %s <threads_count [<=16]>\n",
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

void exit_on_error(int error)
{
    if (error)
    {
        fprintf(stderr, "[%d]: %s\n", error, strerror(error));
        exit(EXIT_FAILURE);
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
                pthread_mutex_lock(&mutex);
	            (*count)++;
                pthread_mutex_unlock(&mutex);
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

void* worker(void* p)
{
    int i = *(int*)p;
    chessboard c;
    memset(c, '\0', sizeof(c));
    c[0][i] = 1;
    nqueens(threads_count, 1, c, &count);
    
    return NULL;
}

