/*!
 * \ingroup td_5_group
 * \file hellogoodbye.c
 * \brief Exercise 5.1
 *
 * Displays messages from threads.
 *
 * This program uses the following function:
 *
 *  - [pthread_create(pthread_t\* thread, const pthread_attr_t\* attr, void\* (\*start_routine)(void\*), void\* arg](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
 *  - [pthread_join(pthread_t thread, void\*\* retval)](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
 * 
 * \author H. Decoudras
 * \version 1
 */

#include <unistd.h>
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
 * \brief The hello_goodbye() function displays
 *        a message the thread identifier
 *        of the routine and a message on the
 *        standard output.
 *
 * \param p Unused.
 *
 * \return Always `NULL`
 */
static void* hello_goodbye(void* p);


/*!
 * \brief Number of threads.
 */
static int threads_count;


/*!
 * \brief Main entry point of the program.
 *
 * Displays messages from threads.
 *
 * This program uses the following function:
 *
 *  - [pthread_create(pthread_t\* thread, const pthread_attr_t\* attr, void\* (\*start_routine)(void\*), void\* arg](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
 *  - [pthread_join(pthread_t thread, void\*\* retval)](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
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
    pthread_t threads[threads_count];
    
    /* Spawn some threads */

    for (int i = 0, result; i < threads_count; ++i)
    {
        result = pthread_create(&threads[i], NULL, hello_goodbye, NULL);
        exit_on_error(result);
    }

    /* Join the threads */

    for (int i = 0, result; i < threads_count; ++i)
    {
        result = pthread_join(threads[i], NULL);
        exit_on_error(result);
    }

    return EXIT_SUCCESS;
}


void use(const char* program)
{
    fprintf(
        stderr, "Use:\n  %s <threads_count>\n",
        program
    );
    exit(EXIT_FAILURE);
}

void exit_on_argv_error(int argc, char** argv)
{
    if (argc < 2)
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

void* hello_goodbye(void* p)
{
    printf("[%lx]: Hello!\n", pthread_self()); 
    sleep(1);
    printf("[%lx]: Bye!\n", pthread_self()); 

    return NULL;
}

