AC_PREREQ([2.61])
AC_INIT([twstool], [0.1.0], [http://www.ga-group.nl/])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_HEADER([./config.h])
AC_CONFIG_SRCDIR([src/])

AM_INIT_AUTOMAKE([foreign])

## compiler check
AC_PROG_CXX
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_RANLIB

DEFAULT_CXXFLAGS="-Wall -W"
AC_SUBST([DEFAULT_CXXFLAGS])

PKG_CHECK_MODULES([QT4_DEPS], [QtCore >= 4.3])
## use rpath if qt4 is not the global installed one
QT4_DEPS_LIBS=$( echo ${QT4_DEPS_LIBS} \
	| sed 's/\(^\| \|\t\)-L\([[^\t ]]*\)/\1-Wl,-rpath,\2 -L\2/g' )

PKG_CHECK_MODULES([libconfig_DEPS], [libconfig++ >= 1.3])

## output
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
