/*!
 * \ingroup td_2_group
 * \file display.h
 * \brief Exercise 2.5
 *
 * Displays a tree representation of shell commands.
 *
 * \author H. Decoudras
 * \version 1
 */

#ifndef DEF_DISPLAY_H
#define DEF_DISPLAY_H

#include "shelltree.h"


/*!
 * \brief The print_expression() function displays
 *        a tree representation of shell commands.
 *
 * \param e Shell commands.
 *
 * \see expression_type
 * \see expression
 */
extern void print_expression(Expression* e);


#endif // DEF_DISPLAY_H

