/*!
 * \ingroup td_5_group
 * \file calcmutex.c
 * \brief Exercise 5.3
 *
 * Accesses a global variable from multiple threads.
 *
 * This program uses the following function:
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*!
 * \brief Number of elements to calculate.
 */
#define RESULT_SIZE  10000000


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
 * \brief The parallel_for() function performs
 *        a calculation on a global variable.
 *
 * \param p Unused.
 *
 * \return Always `NULL`
 */
static void* parallel_for(void* p);

/*!
 * \brief Calculation results
 */
static double result[RESULT_SIZE];

/*!
 * \brief Number of threads.
 */
static int threads_count;

/*!
 * \brief sample global variable.
 */
static int k = 0;

/*!
 * \brief Mutex used to lock the \ref k
 *        global variable.
 */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/*!
 * \brief Main entry point of the program.
 *
 * Accesses a global variable from multiple threads.
 *
 * This program uses the following function:
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
    pthread_t threads_array[threads_count];
   
    /* Spawn some threads */

    for(int i = 0, result; i < threads_count; i++) 
    {
        result = pthread_create(
            &threads_array[i], 
            NULL, 
            parallel_for, 
            NULL
        );
        exit_on_error(result);
    }
    
    /* Join the threads */

    for (int i = 0, result; i < threads_count; i++) 
    {
        result = pthread_join(threads_array[i], NULL);
        exit_on_error(result);
    }

    fprintf(stdout, "%d\n", k); 
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
    if (argc != 2)
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

void* parallel_for(void* p) 
{
    /* Prevent a race condition */
 
    pthread_mutex_lock(&mutex);

    for(unsigned long i = 0; i < RESULT_SIZE; i++)
    { 
        ++k;
    }

    pthread_mutex_unlock(&mutex);
 
    return NULL;
}

