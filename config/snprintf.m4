dnl @synopsis AC_FUNC_SNPRINTF
dnl
dnl Provides a test for a fully C9x complient snprintf
dnl function.
Dnl defines HAVE_SNPRINTF if it is found, and
dnl sets ac_cv_func_snprintf to yes, otherwise to no.
dnl
dnl @version $Id: snprintf.m4,v 1.1 2000/11/23 09:18:56 mikal Exp $
dnl @author Caolan McNamara <caolan@skynet.ie>
dnl
AC_DEFUN([AC_FUNC_SNPRINTF],
[AC_CACHE_CHECK(for working snprintf, ac_cv_func_snprintf,
[AC_TRY_RUN([#include <stdio.h>
int main () { exit (!(3 <= snprintf(NULL,0,"%d",100))); }
], ac_cv_func_snprintf=yes, ac_cv_func_snprintf=no,
ac_cv_func_snprintf=no)])
if test $ac_cv_func_snprintf = yes; then
  AC_DEFINE(HAVE_SNPRINTF)
fi
])
