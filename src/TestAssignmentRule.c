/**
 * Filename    : TestAssignmentRule.c
 * Description : AssignmentRule unit tests
 * Author(s)   : SBML Development Group <sysbio-team@caltech.edu>
 * Organization: JST ERATO Kitano Symbiotic Systems Project
 * Created     : 2002-11-26
 * Revision    : $Id$
 * Source      : $Source$
 *
 * Copyright 2002 California Institute of Technology and
 * Japan Science and Technology Corporation.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
 * documentation provided hereunder is on an "as is" basis, and the
 * California Institute of Technology and Japan Science and Technology
 * Corporation have no obligations to provide maintenance, support,
 * updates, enhancements or modifications.  In no event shall the
 * California Institute of Technology or the Japan Science and Technology
 * Corporation be liable to any party for direct, indirect, special,
 * incidental or consequential damages, including lost profits, arising
 * out of the use of this software and its documentation, even if the
 * California Institute of Technology and/or Japan Science and Technology
 * Corporation have been advised of the possibility of such damage.  See
 * the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * The original code contained here was initially developed by:
 *
 *     Ben Bornstein
 *     The Systems Biology Markup Language Development Group
 *     ERATO Kitano Symbiotic Systems Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *     http://www.cds.caltech.edu/erato
 *     mailto:sysbio-team@caltech.edu
 *
 * Contributor(s):
 */


#include <check.h>

#include "sbml/common.h"
#include "sbml/FormulaParser.h"
#include "sbml/AssignmentRule.h"


static AssignmentRule_t *AR;


void
AssignmentRuleTest_setup (void)
{
  AR = AssignmentRule_create();

  if (AR == NULL)
  {
    fail("AssignmentRule_create() returned a NULL pointer.");
  }
}


void
AssignmentRuleTest_teardown (void)
{
  AssignmentRule_free(AR);
}

START_TEST (test_AssignmentRule_L2_create)
{
  fail_unless( SBase_getTypeCode  (AR) == SBML_ASSIGNMENT_RULE, NULL );
  fail_unless( SBase_getMetaId    (AR) == NULL, NULL );
  fail_unless( SBase_getNotes     (AR) == NULL, NULL );
  fail_unless( SBase_getAnnotation(AR) == NULL, NULL );

  fail_unless( Rule_getFormula(AR) == NULL, NULL );
  fail_unless( Rule_getMath   (AR) == NULL, NULL );

  fail_unless( AssignmentRule_getVariable(AR) == NULL, NULL );
  fail_unless( AssignmentRule_getType    (AR) == RULE_TYPE_SCALAR, NULL );
}
END_TEST


START_TEST (test_AssignmentRule_L2_createWith)
{
  ASTNode_t        *math = SBML_parseFormula("y + 1");
  AssignmentRule_t *ar   = AssignmentRule_createWith("x", math);


  fail_unless( SBase_getTypeCode  (ar) == SBML_ASSIGNMENT_RULE, NULL );
  fail_unless( SBase_getMetaId    (ar) == NULL, NULL );
  fail_unless( SBase_getNotes     (ar) == NULL, NULL );
  fail_unless( SBase_getAnnotation(ar) == NULL, NULL );

  fail_unless( Rule_getFormula(ar) == NULL, NULL );
  fail_unless( Rule_getMath   (ar) == math, NULL );

  fail_unless( !strcmp(AssignmentRule_getVariable(ar), "x"), NULL );

  fail_unless( AssignmentRule_getType(ar) == RULE_TYPE_SCALAR, NULL );

  AssignmentRule_free(ar);
}
END_TEST


START_TEST (test_AssignmentRule_free_NULL)
{
  AssignmentRule_free(NULL);
}
END_TEST


START_TEST (test_AssignmentRule_setVariable)
{
  char *variable = "x";


  AssignmentRule_setVariable(AR, variable);

  fail_unless( !strcmp(AssignmentRule_getVariable(AR), variable), NULL );
  fail_unless( AssignmentRule_isSetVariable(AR), NULL );

  if (AssignmentRule_getVariable(AR) == variable)
  {
    fail("AssignmentRule_setVariable(...) did not make a copy of string.");
  }

  /* Reflexive case (pathological) */
  AssignmentRule_setVariable(AR, AssignmentRule_getVariable(AR));
  fail_unless( !strcmp(AssignmentRule_getVariable(AR), variable), NULL );

  AssignmentRule_setVariable(AR, NULL);
  fail_unless( !AssignmentRule_isSetVariable(AR), NULL );

  if (AssignmentRule_getVariable(AR) != NULL)
  {
    fail("AssignmentRule_setVariable(AR, NULL) did not clear string.");
  }
}
END_TEST


Suite *
create_suite_AssignmentRule (void)
{
  Suite *suite = suite_create("AssignmentRule");
  TCase *tcase = tcase_create("AssignmentRule");


  tcase_add_checked_fixture( tcase,
                             AssignmentRuleTest_setup,
                             AssignmentRuleTest_teardown );

  tcase_add_test( tcase, test_AssignmentRule_L2_create     );
  tcase_add_test( tcase, test_AssignmentRule_L2_createWith );
  tcase_add_test( tcase, test_AssignmentRule_free_NULL     );
  tcase_add_test( tcase, test_AssignmentRule_setVariable   );

  suite_add_tcase(suite, tcase);

  return suite;
}
