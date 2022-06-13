/*!
 * \ingroup td_4_group
 * \file exiterror.c
 * \brief Exercise 4.8
 *  
 * \author H. Decoudras
 * \version 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "exiterror.h"


void exit_on_error(int assertion)
{
    if (assertion)
    {
        if (errno) 
        {
            fprintf(stderr, "[%d]: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        fprintf(stderr, "An error occured!\n");
        exit(EXIT_FAILURE);
    }
}

