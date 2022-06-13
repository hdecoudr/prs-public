/*!
 * \ingroup td_6_group
 * \file parallelfor6.c
 * \brief Exercise 6.4
 *
 * Distributes threads in order to process a value.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [for](https://www.openmp.org/spec-html/5.0/openmpsu41.html#x64-1290002.9.2)
 *
 * \author H. Decoudras
 * \version 1
 */


#include <omp.h>

#include <stdlib.h>
#include <stdio.h>


/*!
 * \brief Main entry point of the program.
 *
 * Distributes threads in order to process a value. 
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directives:
 *
 *  - [for](https://www.openmp.org/spec-html/5.0/openmpsu41.html#x64-1290002.9.2)
 * 
 * \return [EXIT_SUCCESS](https://man7.org/linux/man-pages/man3/exit.3.html)
 */
int main(void)
{
    int i;

    /* 
        Distribute iterations 
        
        The first iteration is handled by the first thread,
        the second iteration is handled by the second thread,
        and so on.

        A static schedule allows the chunk to be precomputed
        and threads can be scheduled during the compilation.
    */

    #pragma omp parallel for schedule(static, 1)
        for (i = 0; i < 40; ++i)
        {
            fprintf(
                stdout, 
                "[%d] assigned to [%i]\n", 
                omp_get_thread_num(), 
                i
            );
        }

    return EXIT_SUCCESS;
}

