#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(semaphore, 0.1, gbenison@gmail.com)
AC_CONFIG_SRCDIR([src/semaphore.c])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h unistd.h sys/sem.h])

# Checks for typedefs, structures, and compiler characteristics.

# It's stupid, but not all platforms have union semun, even those that need it.
AC_MSG_CHECKING(for union semun in sys/sem.h)
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
],[
union semun arg;
semctl(0, 0, 0, arg);
], [have_union_semun="1" union_semun=yes ]
msg=yes, [
have_union_semun="0"
msg=no ] )
AC_MSG_RESULT([$msg])
if test x"$union_semun" = "xyes"
then
  AC_DEFINE(HAVE_UNION_SEMUN, 1,
            [ Define if your system's sys/sem.h file defines union semun ])
fi
AC_SUBST(have_union_semun)

# Checks for library functions.
AC_CHECK_FUNCS([atexit])

AC_OUTPUT



