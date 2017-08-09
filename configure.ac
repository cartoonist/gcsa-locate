#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([gcsa-locate], [0.0.1], [ali.ghaffaari@mpi-inf.mpg.de], [gcsa_locate],
        [https://github.com/cartoonist/gcsa-locate])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.cc])

AC_DEFINE_UNQUOTED([LAST_MOD_DATE], ["`git log --pretty=format:'%ad' --date=format:'%b %e %Y' -n 1`"], [Last commit date.])
AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_LANG([C++])

AX_CHECK_COMPILE_FLAG([-std=c++14], [CXXFLAGS="$CXXFLAGS -std=c++14"],
		      [AC_MSG_ERROR([Compiler does not provide -std=c++14 flag.])])
AC_OPENMP([], [AC_MSG_ERROR([Unable to find OpenMP support])])

# Checks for libraries.
PKG_CHECK_MODULES([ZLIB], [zlib >= 1.2.8], [], [AC_MSG_ERROR([Unable to find zlib])])
AC_CHECK_LIB([z], [compress])
AC_CHECK_LIB([bz2], [BZ2_bzCompress], [], [AC_MSG_ERROR([Unable to find libbz2])])
PKG_CHECK_MODULES([SEQAN2], [seqan-2 >= 2.3.2], [],
                  [AC_MSG_ERROR([Unable to find seqan-2])])
PKG_CHECK_MODULES([SDSL], [sdsl-lite >= 2.1.0], [],
                  [AC_MSG_ERROR([Unable to find sdsl-lite headers or library])])
AC_CHECK_LIB([sdsl], [main])
PKG_CHECK_MODULES([DIVSUFSORT], [libdivsufsort >= 2.0.1], [], [AC_MSG_ERROR([Unable to find libdivsufsort])])
PKG_CHECK_MODULES([DIVSUFSORT64], [libdivsufsort64 >= 2.0.1], [],
             [AC_MSG_ERROR([Unable to find libdivsufsort64])])
AC_CHECK_LIB([divsufsort], [divsufsort])
AC_CHECK_LIB([divsufsort64], [divsufsort])
PKG_CHECK_MODULES([GCSA2], [gcsa2 >= 1.0.0], [],
                  [AC_MSG_ERROR([Unable to find gcsa2 headers or library])])
AC_CHECK_LIB([gcsa2], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
