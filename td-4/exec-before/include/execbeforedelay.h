/*!
 * \ingroup td_4_group
 * \file execbeforedelay.h
 * \brief Exercise 4.8
 *  
 * \author H. Decoudras
 * \version 1
 */

#ifndef DEF_EXECBEFOREDELAY_H
#define DEF_EXECBEFOREDELAY_H

/*!
 * \brief The exec_before_delay() function executes the \p func 
 *        function within the time specified by the \p delay 
 *        parameter.
 *
 * \param func A function.
 * \param param Function parameter.
 * \param delay Time limit within which the function must be started.
 *
 * \return This function can return the following values:
 *          - **0** if the evaluation of the function has been stopped;
 *          - **1** if the 1 if the evaluation of the function has not
 *            been stopped.
 */
int exec_before_delay(void (*func)(void*), void* param, int delay);


#endif // DEF_EXECBEFOREDELAY_H

