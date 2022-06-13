/*!
 * \ingroup td_2_group
 * \file evaluator.h
 * \brief Exercise 2.5
 *
 * Executes a shell command.
 *
 * \author H. Decoudras
 * \version 7
 */

#ifndef DEF_EVALUATOR_H
#define DEF_EVALUATOR_H

#include "shelltree.h"


/*!
 * \brief The evaluate_expression() executes a shell command.
 *
 * \param e Shell commands to run.
 *
 * \return This function can return the following values:
 *          - **0** if the shell command has not been 
 *            succesfully executed
 *          - **1** if the shell command has been successfully 
 *            executed
 *
 * \see expression_type
 * \see expression
 */
extern int evaluate_expression(Expression* e);


#endif // DEF_EVALUATOR

