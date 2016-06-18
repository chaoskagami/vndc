#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([vndc], [0.3], [chaos.kagami@gmail.com], [vndc], [http://github.com/chaoskagami/vndc])
AC_CONFIG_SRCDIR([LICENSE])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT64_T

# Check for SDL2
SDL_VERSION=2.0.0
AM_PATH_SDL2([$SDL_VERSION], :, AC_MSG_ERROR([SDL was not found.]))

# Check for SDL2 mixer
PKG_CHECK_MODULES([SDL2_mixer], [SDL2_mixer >= 2.0.0])

# Check for SDL2 image
PKG_CHECK_MODULES([SDL2_image], [SDL2_image >= 2.0.0])

# Check for SDL2 ttf
PKG_CHECK_MODULES([SDL2_ttf], [SDL2_ttf >= 2.0.0])

# Check for gl
PKG_CHECK_MODULES([gl], [gl > 0])

# Check for glew
PKG_CHECK_MODULES([glew], [glew > 0])

CXXFLAGS="$CXXFLAGS $SDL_CFLAGS $SDL2_mixer_CFLAGS $SDL2_image_CFLAGS $SDL2_ttf_CFLAGS $gl_CFLAGS $glew_CFLAGS"
LIBS="$LIBS $SDL_LIBS $SDL2_mixer_LIBS $SDL2_image_LIBS $SDL2_ttf_LIBS $gl_LIBS $glew_LIBS"
LDFLAGS="$LDFLAGS $SDL_LDFLAFS $SDL2_mixer_LDFLAGS $SDL2_image_LDFLAGS $SDL2_ttf_LDFLAGS $gl_LDFLAGS $glew_LDFLAGS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

