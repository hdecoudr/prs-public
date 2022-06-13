/*!
 * \ingroup td_6_group
 * \file parallelfor4.c
 * \brief Exercise 6.4
 *
 * Distributes threads in order to process the values 
 * of an array only once.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [for](https://www.openmp.org/spec-html/5.0/openmpsu41.html#x64-1290002.9.2)
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
 *
 * \author H. Decoudras
 * \version 1
 */


#include <omp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*!
 * \brief Size of the aray.
 */
#define N 1000000


/*!
 * \brief Main entry point of the program.
 *
 * Distributes threads in order to process the values
 * of an array only once. 
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [parallel](https://www.openmp.org/spec-html/5.0/openmpse14.html#x54-800002.6)
 *  - [for](https://www.openmp.org/spec-html/5.0/openmpsu41.html#x64-1290002.9.2)
 *  - [critical](https://www.openmp.org/spec-html/5.0/openmpsu89.html#x120-4470002.17.1)
 * 
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{
    int array[N];
    int sum_thread = 0;
    int sum_real = 0; 

    for (int i = 0; i < N; ++i)
    {
        array[i] = 1;
    }

    /* Execute the following block in parallel */

    #pragma omp parallel
    {
        int i;
        int sum_thread_temp = 0;

        /* Assign one iteration to each thread */

        #pragma omp for schedule(static, 1)
            for (i = 0; i < N; ++i)
            {
                sum_thread_temp += array[i];
            }

        /* Mutual exclusion */

        #pragma omp critical
        {
            sum_thread += sum_thread_temp;
        }       
    }

    for (int i = 0; i < N; ++i)
    {
        sum_real += array[i];
    }

    fprintf(
        stdout, 
        "Thread sum: [%d]\n"
        "Real sum  : [%d]\n", 
        sum_thread,
        sum_real
    );

    return EXIT_SUCCESS;
}

