/*!
 * \ingroup td_4_group
 * \file execsafe.h
 * \brief Exercise 4.9
 *
 * \author H. Decoudras
 * \version 1
 */

#ifndef DEF_EXECSAFE_H
#define	DEF_EXECSAFE_H


/*!
 * \brief The test() function registers a signal \p sig
 *        and triggers a function \p func.
 *
 * \param func Function to trigger.
 * \param sig Signal to register.
 *
 * \return This function can return the following values:
 *          - **-1** if an error has been detected
 *          - **0** if no error has been detected
 */
int test(void (*func)(void), int sig);


#endif // DEF_EXECSAFE_H

