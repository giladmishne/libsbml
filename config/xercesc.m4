dnl
dnl Filename    : xercesc.m4
dnl Description : Autoconf macro to check for existence of Xerces-C library
dnl Author(s)   : SBML Development Group <sysbio-team@caltech.edu>
dnl Organization: JST ERATO Kitano Symbiotic Systems Project
dnl Created     : 2003-02-14
dnl Revision    : $Id$
dnl Source      : $Source$
dnl
dnl Copyright 2002 California Institute of Technology and
dnl Japan Science and Technology Corporation.
dnl
dnl This library is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU Lesser General Public License as published
dnl by the Free Software Foundation; either version 2.1 of the License, or
dnl any later version.
dnl
dnl This library is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
dnl MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
dnl documentation provided hereunder is on an "as is" basis, and the
dnl California Institute of Technology and Japan Science and Technology
dnl Corporation have no obligations to provide maintenance, support,
dnl updates, enhancements or modifications.  In no event shall the
dnl California Institute of Technology or the Japan Science and Technology
dnl Corporation be liable to any party for direct, indirect, special,
dnl incidental or consequential damages, including lost profits, arising
dnl out of the use of this software and its documentation, even if the
dnl California Institute of Technology and/or Japan Science and Technology
dnl Corporation have been advised of the possibility of such damage.  See
dnl the GNU Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public License
dnl along with this library; if not, write to the Free Software Foundation,
dnl Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
dnl
dnl The original code contained here was initially developed by:
dnl
dnl     Ben Bornstein
dnl     The Systems Biology Markup Language Development Group
dnl     ERATO Kitano Symbiotic Systems Project
dnl     Control and Dynamical Systems, MC 107-81
dnl     California Institute of Technology
dnl     Pasadena, CA, 91125, USA
dnl
dnl     http://www.cds.caltech.edu/erato
dnl     mailto:sysbio-team@caltech.edu
dnl
dnl Contributor(s):
dnl


dnl
dnl Check --with-xerces[=PREFIX] is specified and Xerces-C++ is installed.
dnl

AC_DEFUN(AC_LIB_XERCES,
[
  if test $with_xerces != no; then

    AC_MSG_CHECKING([for Apache's Xerces-C XML library])

    XERCES_CPPFLAGS=
    XERCES_LDFLAGS=
    XERCES_LIBS=

    if test $with_xerces != yes; then
      XERCES_CPPFLAGS="-I$with_xerces/include"
      XERCES_LDFLAGS="-L$with_xerces/lib"
    fi

    XERCES_LIBS="-lxerces-c"

    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS

    CPPFLAGS="$XERCES_CPPFLAGS $CPPFLAGS"
    LDFLAGS="$XERCES_LDFLAGS $LDFLAGS"
    LIBS="$XERCES_LIBS $LIBS"

    AC_TRY_LINK([
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#ifndef XERCES_HAS_CPP_NAMESPACE
#define XERCES_CPP_NAMESPACE_QUALIFIER
#endif
      ],
      [XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::Initialize();],
      [xerces_found=yes],
      [xerces_found=no])

    AC_LANG_RESTORE

    AC_MSG_RESULT($xerces_found)

    if test $xerces_found = no; then
      AC_MSG_ERROR([Could not find the Xerces XML library.])
    fi
  fi
])
