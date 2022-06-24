/*!
 * \ingroup td_6_group
 * \file parallelfor8.c
 * \brief Exercise 6.4
 *
 * Distributes threads in order to process a value.
 *
 * This program uses the following [OpenMP](https://www.openmp.org/)
 * directive:
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
 * directive:
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
        
        A dynamic schedule does not allow the chunk to be precomputed
        and threads cannot be scheduled during the compilation
        
        This mechanism is handled at runtime
    */

    #pragma omp parallel for schedule(dynamic, 1)
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

