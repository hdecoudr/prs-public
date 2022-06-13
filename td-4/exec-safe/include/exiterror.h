/*!
 * \ingroup td_4_group
 * \file exiterror.h
 * \brief Exercise 4.9
 *
 * \author H. Decoudras
 * \version 1
 */

#ifndef DEF_EXITERROR_H
#define DEF_EXITERROR_H


/*!
 * \brief The exit_on_error() function exits the program
 *        if the \p assertion parameter is evaluated
 *        to `TRUE`.
 *
 * If the assertion is evaluated to `TRUE` and
 * [errno](https://man7.org/linux/man-pages/man3/errno.3.html)
 * is set, then the error number and its associated message
 * are displayed. Otherwise, a generic message is displayed.
 *
 * \param assertion Assertion to be evaluated.
 */
void exit_on_error(int assertion);


#endif // DEF_EXITERROR_H

