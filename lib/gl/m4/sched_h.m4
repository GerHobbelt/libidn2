# sched_h.m4 serial 12
dnl Copyright (C) 2008-2021 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl Written by Bruno Haible.

AC_DEFUN([gl_SCHED_H],
[
  dnl Use AC_REQUIRE here, so that the default behavior below is expanded
  dnl once only, before all statements that occur in other macros.
  AC_REQUIRE([gl_SCHED_H_DEFAULTS])

  AC_REQUIRE([AC_CANONICAL_HOST])

  AC_CHECK_HEADERS_ONCE([sys/cdefs.h])
  AC_CHECK_HEADERS([sched.h], [], [],
    [[#if HAVE_SYS_CDEFS_H
       #include <sys/cdefs.h>
      #endif
    ]])
  gl_NEXT_HEADERS([sched.h])

  if test "$ac_cv_header_sched_h" = yes; then
    HAVE_SCHED_H=1
  else
    HAVE_SCHED_H=0
  fi
  AC_SUBST([HAVE_SCHED_H])

  if test "$HAVE_SCHED_H" = 1; then
    AC_CHECK_TYPE([struct sched_param],
      [HAVE_STRUCT_SCHED_PARAM=1], [HAVE_STRUCT_SCHED_PARAM=0],
      [[#if HAVE_SYS_CDEFS_H
         #include <sys/cdefs.h>
        #endif
        #include <sched.h>
      ]])
  else
    HAVE_STRUCT_SCHED_PARAM=0
    case "$host_os" in
      os2*)
        dnl On OS/2 kLIBC, struct sched_param is in spawn.h.
        AC_CHECK_TYPE([struct sched_param],
          [HAVE_STRUCT_SCHED_PARAM=1], [],
          [#include <spawn.h>])
        ;;
      vms)
        dnl On OpenVMS 7.2 or newer, struct sched_param is in pthread.h.
        AC_CHECK_TYPE([struct sched_param],
          [HAVE_STRUCT_SCHED_PARAM=1], [],
          [#include <pthread.h>])
        ;;
    esac
  fi
  AC_SUBST([HAVE_STRUCT_SCHED_PARAM])

  if test "$ac_cv_header_sys_cdefs_h" = yes; then
    HAVE_SYS_CDEFS_H=1
  else
    HAVE_SYS_CDEFS_H=0
  fi
  AC_SUBST([HAVE_SYS_CDEFS_H])

  dnl Ensure the type pid_t gets defined.
  AC_REQUIRE([AC_TYPE_PID_T])

  dnl Check for declarations of anything we want to poison if the
  dnl corresponding gnulib module is not in use, if it is not common
  dnl enough to be declared everywhere.
  gl_WARN_ON_USE_PREPARE([[#include <sched.h>
    ]], [sched_yield])
])

AC_DEFUN([gl_SCHED_MODULE_INDICATOR],
[
  dnl Use AC_REQUIRE here, so that the default settings are expanded once only.
  AC_REQUIRE([gl_SCHED_H_DEFAULTS])
  gl_MODULE_INDICATOR_SET_VARIABLE([$1])
  dnl Define it also as a C macro, for the benefit of the unit tests.
  gl_MODULE_INDICATOR_FOR_TESTS([$1])
])

AC_DEFUN([gl_SCHED_H_DEFAULTS],
[
  GNULIB_SCHED_YIELD=0;  AC_SUBST([GNULIB_SCHED_YIELD])
  dnl Assume proper GNU behavior unless another module says otherwise.
  HAVE_SCHED_YIELD=1;    AC_SUBST([HAVE_SCHED_YIELD])
  REPLACE_SCHED_YIELD=0; AC_SUBST([REPLACE_SCHED_YIELD])
])
