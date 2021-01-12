# DO NOT EDIT! GENERATED AUTOMATICALLY!
# Copyright (C) 2002-2021 Free Software Foundation, Inc.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <https://www.gnu.org/licenses/>.
#
# As a special exception to the GNU General Public License,
# this file may be distributed as part of a program that
# contains a configuration script generated by Autoconf, under
# the same distribution terms as the rest of that program.
#
# Generated by gnulib-tool.
#
# This file represents the compiled summary of the specification in
# gnulib-cache.m4. It lists the computed macro invocations that need
# to be invoked from configure.ac.
# In projects that use version control, this file can be treated like
# other built files.


# This macro should be invoked from ./configure.ac, in the section
# "Checks for programs", right after AC_PROG_CC, and certainly before
# any checks for libraries, header files, types and library functions.
AC_DEFUN([lgl_EARLY],
[
  m4_pattern_forbid([^gl_[A-Z]])dnl the gnulib macro namespace
  m4_pattern_allow([^gl_ES$])dnl a valid locale name
  m4_pattern_allow([^gl_LIBOBJS$])dnl a variable
  m4_pattern_allow([^gl_LTLIBOBJS$])dnl a variable

  # Pre-early section.
  AC_REQUIRE([gl_USE_SYSTEM_EXTENSIONS])
  AC_REQUIRE([gl_PROG_AR_RANLIB])

  AC_REQUIRE([AM_PROG_CC_C_O])
  # Code from module absolute-header:
  # Code from module attribute:
  # Code from module c-ctype:
  # Code from module c-ctype-tests:
  # Code from module c-strcase:
  # Code from module c99:
  # Code from module ctype:
  # Code from module ctype-tests:
  # Code from module extensions:
  # Code from module extern-inline:
  # Code from module gettext-h:
  # Code from module gperf:
  # Code from module havelib:
  # Code from module iconv:
  # Code from module iconv-h:
  # Code from module iconv-h-tests:
  # Code from module iconv-tests:
  # Code from module iconv_open:
  # Code from module include_next:
  # Code from module inline:
  # Code from module intprops:
  # Code from module intprops-tests:
  # Code from module inttypes:
  # Code from module inttypes-incomplete:
  # Code from module inttypes-tests:
  # Code from module isblank:
  # Code from module isblank-tests:
  # Code from module lib-msvc-compat:
  # Code from module lib-symbol-versions:
  # Code from module lib-symbol-visibility:
  # Code from module libc-config:
  # Code from module limits-h:
  # Code from module limits-h-tests:
  # Code from module multiarch:
  # Code from module snippet/arg-nonnull:
  # Code from module snippet/c++defs:
  # Code from module snippet/unused-parameter:
  # Code from module snippet/warn-on-use:
  # Code from module ssize_t:
  # Code from module std-gnu11:
  # Code from module stdalign:
  # Code from module stdalign-tests:
  # Code from module stdbool:
  # Code from module stdbool-tests:
  # Code from module stddef:
  # Code from module stddef-tests:
  # Code from module stdint:
  # Code from module stdint-tests:
  # Code from module striconv:
  # Code from module striconv-tests:
  # Code from module string:
  # Code from module string-tests:
  # Code from module strverscmp:
  # Code from module strverscmp-tests:
  # Code from module sys_types:
  # Code from module sys_types-tests:
  # Code from module test-framework-sh:
  # Code from module test-framework-sh-tests:
  # Code from module unistr/base:
  # Code from module unistr/u8-check:
  # Code from module unistr/u8-check-tests:
  # Code from module unistr/u8-mbtoucr:
  # Code from module unistr/u8-mbtoucr-tests:
  # Code from module unistr/u8-uctomb:
  # Code from module unistr/u8-uctomb-tests:
  # Code from module unitypes:
  # Code from module verify:
  # Code from module verify-tests:
  # Code from module wchar:
  # Code from module wchar-tests:
])

# This macro should be invoked from ./configure.ac, in the section
# "Check for header files, types and library functions".
AC_DEFUN([lgl_INIT],
[
  AM_CONDITIONAL([GL_COND_LIBTOOL], [true])
  gl_cond_libtool=true
  gl_m4_base='lib/gl/m4'
  m4_pushdef([AC_LIBOBJ], m4_defn([lgl_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([lgl_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([lgl_LIBSOURCES]))
  m4_pushdef([lgl_LIBSOURCES_LIST], [])
  m4_pushdef([lgl_LIBSOURCES_DIR], [])
  gl_COMMON
  gl_source_base='lib/gl'
  AC_REQUIRE([gl_EXTERN_INLINE])
  AC_SUBST([LIBINTL])
  AC_SUBST([LTLIBINTL])
  AC_DEFUN([gl_HAVE_MODULE_HAVELIB])
  AM_ICONV
  m4_ifdef([gl_ICONV_MODULE_INDICATOR],
    [gl_ICONV_MODULE_INDICATOR([iconv])])
  gl_ICONV_H
  gl_FUNC_ICONV_OPEN
  if test $REPLACE_ICONV_OPEN = 1; then
    AC_LIBOBJ([iconv_open])
  fi
  if test $REPLACE_ICONV = 1; then
    AC_LIBOBJ([iconv])
    AC_LIBOBJ([iconv_close])
  fi
  gl_INLINE
  gl_LD_OUTPUT_DEF
  gl_LD_VERSION_SCRIPT
  gl_VISIBILITY
  gl___INLINE
  gl_LIMITS_H
  gl_MULTIARCH
  gt_TYPE_SSIZE_T
  AM_STDBOOL_H
  gl_STDDEF_H
  gl_STDINT_H
  if test $gl_cond_libtool = false; then
    gl_ltlibdeps="$gl_ltlibdeps $LTLIBICONV"
    gl_libdeps="$gl_libdeps $LIBICONV"
  fi
  gl_HEADER_STRING_H
  gl_FUNC_STRVERSCMP
  if test $HAVE_STRVERSCMP = 0; then
    AC_LIBOBJ([strverscmp])
    gl_PREREQ_STRVERSCMP
  fi
  gl_STRING_MODULE_INDICATOR([strverscmp])
  gl_SYS_TYPES_H
  AC_PROG_MKDIR_P
  gl_LIBUNISTRING_LIBHEADER([0.9.4], [unistr.h])
  gl_LIBUNISTRING_MODULE([0.9], [unistr/u8-check])
  gl_MODULE_INDICATOR([unistr/u8-mbtoucr])
  gl_LIBUNISTRING_MODULE([0.9], [unistr/u8-mbtoucr])
  gl_MODULE_INDICATOR([unistr/u8-uctomb])
  gl_LIBUNISTRING_MODULE([0.9], [unistr/u8-uctomb])
  gl_LIBUNISTRING_LIBHEADER([0.9.4], [unitypes.h])
  # End of code from modules
  m4_ifval(lgl_LIBSOURCES_LIST, [
    m4_syscmd([test ! -d ]m4_defn([lgl_LIBSOURCES_DIR])[ ||
      for gl_file in ]lgl_LIBSOURCES_LIST[ ; do
        if test ! -r ]m4_defn([lgl_LIBSOURCES_DIR])[/$gl_file ; then
          echo "missing file ]m4_defn([lgl_LIBSOURCES_DIR])[/$gl_file" >&2
          exit 1
        fi
      done])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([expected source file, required through AC_LIBSOURCES, not found])])
  ])
  m4_popdef([lgl_LIBSOURCES_DIR])
  m4_popdef([lgl_LIBSOURCES_LIST])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    lgl_libobjs=
    lgl_ltlibobjs=
    if test -n "$lgl_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $lgl_LIBOBJS; do echo "$i"; done | sed -e "$sed_drop_objext" | sort | uniq`; do
        lgl_libobjs="$lgl_libobjs $i.$ac_objext"
        lgl_ltlibobjs="$lgl_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([lgl_LIBOBJS], [$lgl_libobjs])
    AC_SUBST([lgl_LTLIBOBJS], [$lgl_ltlibobjs])
  ])
  gltests_libdeps=
  gltests_ltlibdeps=
  m4_pushdef([AC_LIBOBJ], m4_defn([lgltests_LIBOBJ]))
  m4_pushdef([AC_REPLACE_FUNCS], m4_defn([lgltests_REPLACE_FUNCS]))
  m4_pushdef([AC_LIBSOURCES], m4_defn([lgltests_LIBSOURCES]))
  m4_pushdef([lgltests_LIBSOURCES_LIST], [])
  m4_pushdef([lgltests_LIBSOURCES_DIR], [])
  gl_COMMON
  gl_source_base='lib/gltests'
changequote(,)dnl
  lgltests_WITNESS=IN_`echo "${PACKAGE-$PACKAGE_TARNAME}" | LC_ALL=C tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ | LC_ALL=C sed -e 's/[^A-Z0-9_]/_/g'`_GNULIB_TESTS
changequote([, ])dnl
  AC_SUBST([lgltests_WITNESS])
  gl_module_indicator_condition=$lgltests_WITNESS
  m4_pushdef([gl_MODULE_INDICATOR_CONDITION], [$gl_module_indicator_condition])
  gl_CTYPE_H
  gl_INTTYPES_H
  gl_INTTYPES_INCOMPLETE
  gl_FUNC_ISBLANK
  if test $HAVE_ISBLANK = 0; then
    AC_LIBOBJ([isblank])
  fi
  gl_MODULE_INDICATOR([isblank])
  gl_CTYPE_MODULE_INDICATOR([isblank])
  gl_STDALIGN_H
  AC_REQUIRE([gt_TYPE_WCHAR_T])
  AC_REQUIRE([gt_TYPE_WINT_T])
  gl_WCHAR_H
  m4_popdef([gl_MODULE_INDICATOR_CONDITION])
  m4_ifval(lgltests_LIBSOURCES_LIST, [
    m4_syscmd([test ! -d ]m4_defn([lgltests_LIBSOURCES_DIR])[ ||
      for gl_file in ]lgltests_LIBSOURCES_LIST[ ; do
        if test ! -r ]m4_defn([lgltests_LIBSOURCES_DIR])[/$gl_file ; then
          echo "missing file ]m4_defn([lgltests_LIBSOURCES_DIR])[/$gl_file" >&2
          exit 1
        fi
      done])dnl
      m4_if(m4_sysval, [0], [],
        [AC_FATAL([expected source file, required through AC_LIBSOURCES, not found])])
  ])
  m4_popdef([lgltests_LIBSOURCES_DIR])
  m4_popdef([lgltests_LIBSOURCES_LIST])
  m4_popdef([AC_LIBSOURCES])
  m4_popdef([AC_REPLACE_FUNCS])
  m4_popdef([AC_LIBOBJ])
  AC_CONFIG_COMMANDS_PRE([
    lgltests_libobjs=
    lgltests_ltlibobjs=
    if test -n "$lgltests_LIBOBJS"; then
      # Remove the extension.
      sed_drop_objext='s/\.o$//;s/\.obj$//'
      for i in `for i in $lgltests_LIBOBJS; do echo "$i"; done | sed -e "$sed_drop_objext" | sort | uniq`; do
        lgltests_libobjs="$lgltests_libobjs $i.$ac_objext"
        lgltests_ltlibobjs="$lgltests_ltlibobjs $i.lo"
      done
    fi
    AC_SUBST([lgltests_LIBOBJS], [$lgltests_libobjs])
    AC_SUBST([lgltests_LTLIBOBJS], [$lgltests_ltlibobjs])
  ])
  LIBTESTS_LIBDEPS="$gltests_libdeps"
  AC_SUBST([LIBTESTS_LIBDEPS])
])

# Like AC_LIBOBJ, except that the module name goes
# into lgl_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([lgl_LIBOBJ], [
  AS_LITERAL_IF([$1], [lgl_LIBSOURCES([$1.c])])dnl
  lgl_LIBOBJS="$lgl_LIBOBJS $1.$ac_objext"
])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into lgl_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([lgl_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [lgl_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parameterization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([lgl_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_define([lgl_LIBSOURCES_DIR], [lib/gl])
      m4_append([lgl_LIBSOURCES_LIST], _gl_NAME, [ ])
    ])
  ])
])

# Like AC_LIBOBJ, except that the module name goes
# into lgltests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([lgltests_LIBOBJ], [
  AS_LITERAL_IF([$1], [lgltests_LIBSOURCES([$1.c])])dnl
  lgltests_LIBOBJS="$lgltests_LIBOBJS $1.$ac_objext"
])

# Like AC_REPLACE_FUNCS, except that the module name goes
# into lgltests_LIBOBJS instead of into LIBOBJS.
AC_DEFUN([lgltests_REPLACE_FUNCS], [
  m4_foreach_w([gl_NAME], [$1], [AC_LIBSOURCES(gl_NAME[.c])])dnl
  AC_CHECK_FUNCS([$1], , [lgltests_LIBOBJ($ac_func)])
])

# Like AC_LIBSOURCES, except the directory where the source file is
# expected is derived from the gnulib-tool parameterization,
# and alloca is special cased (for the alloca-opt module).
# We could also entirely rely on EXTRA_lib..._SOURCES.
AC_DEFUN([lgltests_LIBSOURCES], [
  m4_foreach([_gl_NAME], [$1], [
    m4_if(_gl_NAME, [alloca.c], [], [
      m4_define([lgltests_LIBSOURCES_DIR], [lib/gltests])
      m4_append([lgltests_LIBSOURCES_LIST], _gl_NAME, [ ])
    ])
  ])
])

# This macro records the list of files which have been installed by
# gnulib-tool and may be removed by future gnulib-tool invocations.
AC_DEFUN([lgl_FILE_LIST], [
  build-aux/config.rpath
  lib/arg-nonnull.h
  lib/attribute.h
  lib/c++defs.h
  lib/c-ctype.c
  lib/c-ctype.h
  lib/c-strcase.h
  lib/c-strcasecmp.c
  lib/c-strncasecmp.c
  lib/cdefs.h
  lib/gettext.h
  lib/iconv.c
  lib/iconv.in.h
  lib/iconv_close.c
  lib/iconv_open-aix.gperf
  lib/iconv_open-hpux.gperf
  lib/iconv_open-irix.gperf
  lib/iconv_open-osf.gperf
  lib/iconv_open-solaris.gperf
  lib/iconv_open-zos.gperf
  lib/iconv_open.c
  lib/libc-config.h
  lib/limits.in.h
  lib/stdbool.in.h
  lib/stddef.in.h
  lib/stdint.in.h
  lib/striconv.c
  lib/striconv.h
  lib/string.in.h
  lib/strverscmp.c
  lib/sys_types.in.h
  lib/unistr.in.h
  lib/unistr/u8-check.c
  lib/unistr/u8-mbtoucr.c
  lib/unistr/u8-uctomb-aux.c
  lib/unistr/u8-uctomb.c
  lib/unitypes.in.h
  lib/unused-parameter.h
  lib/warn-on-use.h
  m4/00gnulib.m4
  m4/__inline.m4
  m4/absolute-header.m4
  m4/ctype.m4
  m4/extensions.m4
  m4/extern-inline.m4
  m4/gnulib-common.m4
  m4/host-cpu-c-abi.m4
  m4/iconv.m4
  m4/iconv_h.m4
  m4/iconv_open.m4
  m4/include_next.m4
  m4/inline.m4
  m4/inttypes.m4
  m4/isblank.m4
  m4/ld-output-def.m4
  m4/ld-version-script.m4
  m4/lib-ld.m4
  m4/lib-link.m4
  m4/lib-prefix.m4
  m4/libunistring-base.m4
  m4/limits-h.m4
  m4/multiarch.m4
  m4/off_t.m4
  m4/pid_t.m4
  m4/ssize_t.m4
  m4/std-gnu11.m4
  m4/stdalign.m4
  m4/stdbool.m4
  m4/stddef_h.m4
  m4/stdint.m4
  m4/string_h.m4
  m4/strverscmp.m4
  m4/sys_types_h.m4
  m4/visibility.m4
  m4/warn-on-use.m4
  m4/wchar_h.m4
  m4/wchar_t.m4
  m4/wint_t.m4
  m4/zzgnulib.m4
  tests/init.sh
  tests/macros.h
  tests/signature.h
  tests/test-c-ctype.c
  tests/test-ctype.c
  tests/test-iconv-h.c
  tests/test-iconv.c
  tests/test-init.sh
  tests/test-intprops.c
  tests/test-inttypes.c
  tests/test-isblank.c
  tests/test-limits-h.c
  tests/test-stdalign.c
  tests/test-stdbool.c
  tests/test-stddef.c
  tests/test-stdint.c
  tests/test-striconv.c
  tests/test-string.c
  tests/test-strverscmp.c
  tests/test-sys_types.c
  tests/test-verify-try.c
  tests/test-verify.c
  tests/test-verify.sh
  tests/test-wchar.c
  tests/unistr/test-u8-check.c
  tests/unistr/test-u8-mbtoucr.c
  tests/unistr/test-u8-uctomb.c
  tests=lib/arg-nonnull.h
  tests=lib/c++defs.h
  tests=lib/ctype.in.h
  tests=lib/dummy.c
  tests=lib/intprops.h
  tests=lib/inttypes.in.h
  tests=lib/isblank.c
  tests=lib/stdalign.in.h
  tests=lib/unused-parameter.h
  tests=lib/verify.h
  tests=lib/warn-on-use.h
  tests=lib/wchar.in.h
])
