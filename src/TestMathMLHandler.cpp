/**
 * Filename    : TestMathMLHandler.cpp
 * Description : MathMLHandler unit tests
 * Author(s)   : SBW Development Group <sysbio-team@caltech.edu>
 * Organization: Caltech ERATO Kitano Systems Biology Project
 * Created     : 2002-05-06
 * Revision    : $Id$
 * Source      : $Source$
 *
 * Copyright 2003 California Institute of Technology and
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
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *     http://www.cds.caltech.edu/erato
 *     mailto:sysbio-team@caltech.edu
 *
 * Contributor(s):
 */


#include "sbml/common.h"

#include "sbml/MathMLHandler.hpp"
#include "sbml/MathMLReader.h"
#include "sbml/MathMLUnicodeConstants.hpp"

#include "sbml/FormulaFormatter.h"


BEGIN_C_DECLS


#define XML_HEADER     "<?xml version='1.0' encoding='ascii'?>\n"
#define MATHML_HEADER  "<math xmlns='http://www.w3.org/1998/Math/MathML'>\n"
#define MATHML_FOOTER  "</math>"

/**
 * Wraps the string s in the appropriate XML or MathML boilerplate.
 */
#define wrapXML(s)     XML_HEADER s
#define wrapMathML(s)  XML_HEADER MATHML_HEADER s MATHML_FOOTER


static MathMLDocument_t *D;


void
MathMLHandlerTest_setup (void)
{
  D = NULL;
}


void
MathMLHandlerTest_teardown (void)
{
  MathMLDocument_free(D);
}


START_TEST (test_element_math)
{
  const char* s = wrapXML("<math xmlns='http://www.w3.org/1998/Math/MathML'/>");
  

  D = readMathMLFromString(s);
  fail_unless(D->math == NULL, NULL);
}
END_TEST


START_TEST (test_element_cn_default)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<cn> 12345.7 </cn>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n)        == AST_REAL, NULL );
  fail_unless( ASTNode_getReal(n)        == 12345.7 , NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0       , NULL );
}
END_TEST


START_TEST (test_element_cn_real)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<cn type='real'> 12345.7 </cn>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n)        == AST_REAL, NULL );
  fail_unless( ASTNode_getReal(n)        == 12345.7 , NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0       , NULL );
}
END_TEST


START_TEST (test_element_cn_integer)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<cn type='integer'> 12345 </cn>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n)        == AST_INTEGER, NULL );
  fail_unless( ASTNode_getInteger(n)     == 12345, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0    , NULL );
}
END_TEST


START_TEST (test_element_cn_rational)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<cn type='rational'> 12342 <sep/> 2342342 </cn>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );


  fail_unless( ASTNode_getType(n)        == AST_RATIONAL, NULL );
  fail_unless( ASTNode_getNumerator(n)   == 12342  , NULL );
  fail_unless( ASTNode_getDenominator(n) == 2342342, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0      , NULL );
}
END_TEST


START_TEST (test_element_cn_e_notation)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<cn type='e-notation'> 12.3 <sep/> 5 </cn>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n)        == AST_REAL_E, NULL );
  fail_unless( ASTNode_getMantissa(n)    == 12.3, NULL );
  fail_unless( ASTNode_getExponent(n)    == 5   , NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0   , NULL );
}
END_TEST


START_TEST (test_element_ci)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<ci> x </ci>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_NAME  , NULL );
  fail_unless( !strcmp(ASTNode_getName(n), "x"), NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0  , NULL );
}
END_TEST


START_TEST (test_element_csymbol_time)
{
  ASTNode_t*  n;
  const char* s = wrapMathML
  (
    "<csymbol encoding='text' "
    "definitionURL='http://www.sbml.org/sbml/symbols/time'> t </csymbol>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_NAME_TIME, NULL );
  fail_unless( !strcmp(ASTNode_getName(n), "t")   , NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0     , NULL );
}
END_TEST


START_TEST (test_element_csymbol_delay)
{
  ASTNode_t*  n;
  const char* s = wrapMathML
  (
    "<csymbol encoding='text' "
    "definitionURL='http://www.sbml.org/sbml/symbols/delay'> delay </csymbol>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_NAME_DELAY, NULL );
  fail_unless( !strcmp(ASTNode_getName(n), "delay"), NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0      , NULL );
}
END_TEST


START_TEST (test_element_constants_true)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<true/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_CONSTANT_TRUE, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0, NULL );
}
END_TEST


START_TEST (test_element_constants_false)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<false/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n)== AST_CONSTANT_FALSE, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0, NULL );
}
END_TEST


START_TEST (test_element_constants_notanumber)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<notanumber/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_REAL, NULL );
  fail_unless( isnan(ASTNode_getReal(n))     , NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0, NULL );
}
END_TEST


START_TEST (test_element_constants_pi)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<pi/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_CONSTANT_PI, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0, NULL );
}
END_TEST


START_TEST (test_element_constants_infinity)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<infinity/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_REAL     , NULL );
  fail_unless( util_isInf(ASTNode_getReal(n)) == 1, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0     , NULL );
}
END_TEST


START_TEST (test_element_constants_exponentiale)
{
  ASTNode_t*  n;
  const char* s = wrapMathML("<exponentiale/>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  fail_unless( ASTNode_getType(n) == AST_CONSTANT_E, NULL );
  fail_unless( ASTNode_getNumChildren(n) == 0, NULL );
}
END_TEST


START_TEST (test_element_abs)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><abs/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "abs(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_and)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <and/> <ci>a</ci> <ci>b</ci> <ci>c</ci> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "and(a, b, c)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccos)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccos/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "acos(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccosh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccosh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arccosh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccot)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccot/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arccot(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccoth)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccoth/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arccoth(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccsc)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccsc/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arccsc(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arccsch)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arccsch/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arccsch(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arcsec)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arcsec/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arcsec(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arcsech)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arcsech/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arcsech(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arcsin)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arcsin/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "asin(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arcsinh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arcsinh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arcsinh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arctan)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arctan/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "atan(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_arctanh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><arctanh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "arctanh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_ceiling)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><ceiling/><cn> 1.6 </cn></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "ceil(1.6)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_cos)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><cos/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "cos(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_cosh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><cosh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "cosh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_cot)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><cot/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "cot(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_coth)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><coth/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "coth(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_csc)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><csc/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "csc(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_csch)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><csch/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "csch(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_eq)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <eq/> <ci>a</ci> <ci>b</ci> <ci>c</ci> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "eq(a, b, c)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_exp)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><exp/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "exp(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_factorial)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><factorial/><cn> 5 </cn></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "factorial(5)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_floor)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><floor/><cn> 1.2 </cn></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "floor(1.2)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_function_call_1)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply> <ci> foo </ci> <ci> x </ci> </apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "foo(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_function_call_2)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <plus/> <cn> 1 </cn>"
    "                <apply> <ci> f </ci> <ci> x </ci> </apply>"
    "</apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "1 + f(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_geq)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <geq/> <cn>1</cn> <ci>x</ci> <cn>0</cn> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "geq(1, x, 0)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_gt)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <gt/> <infinity/>"
    "              <apply> <minus/> <infinity/> <cn>1</cn> </apply>"
    "</apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "gt(INF, INF - 1)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_lambda)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<lambda>"
    "  <bvar> <ci>x</ci> </bvar>"
    "  <apply> <sin/>"
    "          <apply> <plus/> <ci>x</ci> <cn>1</cn> </apply>"
    "  </apply>"
    "</lambda>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "lambda(x, sin(x + 1))"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_leq)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <leq/> <cn>0</cn> <ci>x</ci> <cn>1</cn> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "leq(0, x, 1)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_ln)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><ln/><ci> a </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "log(a)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_log_1)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <log/> <logbase> <cn type='integer'> 3 </cn> </logbase>"
    "               <ci> x </ci>"
    "</apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "log(3, x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_log_2)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply> <log/> <ci> x </ci> </apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "log10(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_lt)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <lt/> <apply> <minus/> <infinity/> <infinity/> </apply>"
    "              <cn>1</cn>"
    "</apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "lt(INF - INF, 1)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_neq)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <neq/> <notanumber/> <notanumber/> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "neq(NaN, NaN)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_not)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply> <not/> <ci> TooShabby </ci> </apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "not(TooShabby)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_or)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <or/> <ci>a</ci> <ci>b</ci> <ci>c</ci> <ci>d</ci> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "or(a, b, c, d)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_piecewise)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<piecewise>"
    "  <piece>"
    "    <apply> <minus/> <ci>x</ci> </apply>"
    "    <apply> <lt/> <ci>x</ci> <cn>0</cn> </apply>"
    "  </piece>"
    "  <piece>"
    "    <cn>0</cn>"
    "    <apply> <eq/> <ci>x</ci> <cn>0</cn> </apply>"
    "  </piece>"
    "  <piece>"
    "    <ci>x</ci>"
    "    <apply> <gt/> <ci>x</ci> <cn>0</cn> </apply>"
    "  </piece>"
    "</piecewise>"
  );

  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "piecewise(-x, lt(x, 0), 0, eq(x, 0), x, gt(x, 0))"),
               NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_piecewise_otherwise)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<piecewise>"
    "  <piece>"
    "    <cn>0</cn>"
    "    <apply> <lt/> <ci>x</ci> <cn>0</cn> </apply>"
    "  </piece>"
    "  <otherwise>"
    "    <ci>x</ci>"
    "  </otherwise>"
    "</piecewise>"
  );

  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "piecewise(0, lt(x, 0), x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_power)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><power/> <ci>x</ci> <cn>3</cn> </apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "pow(x, 3)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_root_1)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <root/> <degree> <cn type='integer'> 3 </cn> </degree>"
    "               <ci> a </ci>"
    "</apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "root(3, a)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_root_2)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply> <root/> <ci> a </ci> </apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "sqrt(a)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_sec)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><sec/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "sec(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_sech)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><sech/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "sech(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_sin)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><sin/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "sin(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_sinh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><sinh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "sinh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_tan)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><tan/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "tan(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_tanh)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML("<apply><tanh/><ci> x </ci></apply>");


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);
  fail_unless( !strcmp(f, "tanh(x)"), NULL );

  safe_free(f);
}
END_TEST


START_TEST (test_element_xor)
{
  ASTNode_t*  n;
  char*       f;
  const char* s = wrapMathML
  (
    "<apply> <xor/> <ci>a</ci> <ci>b</ci> <ci>b</ci> <ci>a</ci> </apply>"
  );


  D = readMathMLFromString(s);
  n = D->math;

  fail_unless( n != NULL, NULL );

  f = SBML_formulaToString(n);

  fail_unless( !strcmp(f, "xor(a, b, b, a)"), NULL );

  safe_free(f);
}
END_TEST


Suite *
create_suite_MathMLHandler (void)
{
  Suite *suite = suite_create("MathMLHandler");
  TCase *tcase = tcase_create("MathMLHandler");


  tcase_add_checked_fixture( tcase,
                             MathMLHandlerTest_setup,
                             MathMLHandlerTest_teardown );

  tcase_add_test( tcase, test_element_math                   );
  tcase_add_test( tcase, test_element_cn_default             );
  tcase_add_test( tcase, test_element_cn_real                );
  tcase_add_test( tcase, test_element_cn_integer             );
  tcase_add_test( tcase, test_element_cn_rational            );
  tcase_add_test( tcase, test_element_cn_e_notation          );
  tcase_add_test( tcase, test_element_ci                     );
  tcase_add_test( tcase, test_element_csymbol_time           );
  tcase_add_test( tcase, test_element_csymbol_delay          );
  tcase_add_test( tcase, test_element_constants_true         );
  tcase_add_test( tcase, test_element_constants_false        );
  tcase_add_test( tcase, test_element_constants_notanumber   );
  tcase_add_test( tcase, test_element_constants_pi           );
  tcase_add_test( tcase, test_element_constants_infinity     );
  tcase_add_test( tcase, test_element_constants_exponentiale );
  tcase_add_test( tcase, test_element_abs                    );
  tcase_add_test( tcase, test_element_and                    );
  tcase_add_test( tcase, test_element_arccos                 );
  tcase_add_test( tcase, test_element_arccosh                );
  tcase_add_test( tcase, test_element_arccot                 );
  tcase_add_test( tcase, test_element_arccoth                );
  tcase_add_test( tcase, test_element_arccsc                 );
  tcase_add_test( tcase, test_element_arccsch                );
  tcase_add_test( tcase, test_element_arcsec                 );
  tcase_add_test( tcase, test_element_arcsech                );
  tcase_add_test( tcase, test_element_arcsin                 );
  tcase_add_test( tcase, test_element_arcsinh                );
  tcase_add_test( tcase, test_element_arctan                 );
  tcase_add_test( tcase, test_element_arctanh                );
  tcase_add_test( tcase, test_element_ceiling                );
  tcase_add_test( tcase, test_element_cos                    );
  tcase_add_test( tcase, test_element_cosh                   );
  tcase_add_test( tcase, test_element_cot                    );
  tcase_add_test( tcase, test_element_coth                   );
  tcase_add_test( tcase, test_element_csc                    );
  tcase_add_test( tcase, test_element_csch                   );
  tcase_add_test( tcase, test_element_eq                     );
  tcase_add_test( tcase, test_element_exp                    );
  tcase_add_test( tcase, test_element_factorial              );
  tcase_add_test( tcase, test_element_floor                  );
  tcase_add_test( tcase, test_element_function_call_1        );
  tcase_add_test( tcase, test_element_function_call_2        );
  tcase_add_test( tcase, test_element_geq                    );
  tcase_add_test( tcase, test_element_gt                     );
  tcase_add_test( tcase, test_element_lambda                 );
  tcase_add_test( tcase, test_element_leq                    );
  tcase_add_test( tcase, test_element_ln                     );
  tcase_add_test( tcase, test_element_log_1                  );
  tcase_add_test( tcase, test_element_log_2                  );
  tcase_add_test( tcase, test_element_lt                     );
  tcase_add_test( tcase, test_element_neq                    );
  tcase_add_test( tcase, test_element_not                    );
  tcase_add_test( tcase, test_element_or                     );
  tcase_add_test( tcase, test_element_piecewise              );
  tcase_add_test( tcase, test_element_piecewise_otherwise    );
  tcase_add_test( tcase, test_element_power                  );
  tcase_add_test( tcase, test_element_root_1                 );
  tcase_add_test( tcase, test_element_root_2                 );
  tcase_add_test( tcase, test_element_sec                    );
  tcase_add_test( tcase, test_element_sech                   );
  tcase_add_test( tcase, test_element_sin                    );
  tcase_add_test( tcase, test_element_sinh                   );
  tcase_add_test( tcase, test_element_tan                    );
  tcase_add_test( tcase, test_element_tanh                   );
  tcase_add_test( tcase, test_element_xor                    );

  suite_add_tcase(suite, tcase);

  return suite;
}


END_C_DECLS
