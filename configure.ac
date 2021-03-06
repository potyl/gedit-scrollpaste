dnl Process this file with autoconf to produce a configure script.

GEDIT_MAJOR=2.20.0
GEDIT_PLUGINS_MAJOR=2.20.0

AC_INIT(
	gedit-scrollpaste,
	0.0.3,
	https://github.com/potyl/gedit-scrollpaste/issues,
	https://github.com/potyl/gedit-scrollpaste)

AC_CONFIG_SRCDIR([src])
AC_CONFIG_MACRO_DIR([m4])
AC_PREFIX_PROGRAM([gedit])

AC_SUBST(GEDIT_PLUGINS_MAJOR)

AC_PREREQ(2.59)

AM_INIT_AUTOMAKE([-Wno-portability 1.8])

AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_DISABLE_STATIC

AC_PATH_PROG(GCONFTOOL, gconftool-2)

AC_PROG_INTLTOOL([0.35.0])

AC_PROG_LIBTOOL

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# make sure we keep ACLOCAL_FLAGS around for maintainer builds to work
AC_SUBST(ACLOCAL_AMFLAGS, "$ACLOCAL_FLAGS -I m4")

# ================================================================
# Gettext stuff.
# ================================================================

GETTEXT_PACKAGE=gedit-scrollpaste
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])
AM_GLIB_GNU_GETTEXT

# ================================================================
# Start of pkg-config checks
# ================================================================

PKG_CHECK_MODULES(GEDIT, [
	gtksourceview-2.0 >= 2.8.1
	gedit-2.20 >= 2.20.0
])
GEDIT_LIBS="${GEDIT_LIBS}"
AC_SUBST(GEDIT_LIBS)
AC_SUBST(GEDIT_CFLAGS)

AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)
			      
GNOME_COMPILE_WARNINGS(yes)


# For building the plugin for the OS X native application (the one installed 
# through a dmg image) we need to compile the plugin in 32 bit mode (i386) and
# to install the plugin in /Applications/gedit.app/Contents/Resources
AC_MSG_CHECKING([for Mac OS X platform])
case "$host" in
	*-*-darwin*)
		platform_osx=yes
	;;
	*)
		platform_osx=no
	;;
esac

if test "$platform_osx" = "yes"; then

	AC_ARG_ENABLE(
		osx,
		AS_HELP_STRING(
			[--enable-osx],
			[build an OS X native plugin]
		),
		enable_osx=$enableval, enable_osx=yes
	)

	if test x$enable_osx = xyes; then
		# For an native OS X plugin, we need to build for i386 and to install
		# in /Applications/gedit.app/Contents/Resources
		CFLAGS="$CFLAGS -arch i386"
		prefix=/Applications/gedit.app/Contents/Resources
	fi
fi


AC_ARG_ENABLE(deprecations,
              [AC_HELP_STRING([--enable-deprecations],
                              [warn about deprecated usages [default=no]])],,
              [enable_deprecations=no])

if test "x$enable_deprecations" = "xyes"
then
	DISABLE_DEPRECATED_CFLAGS="\
		-DG_DISABLE_DEPRECATED		\
		-DGDK_DISABLE_DEPRECATED	\
		-DGTK_DISABLE_DEPRECATED	\
		-DGDK_PIXBUF_DISABLE_DEPRECATED	\
		-DGNOME_DISABLE_DEPRECATED"
	AC_SUBST(DISABLE_DEPRECATED_CFLAGS)
fi

PLUGIN_LIBTOOL_FLAGS="-module -avoid-version"
AC_SUBST(PLUGIN_LIBTOOL_FLAGS)

GEDIT_PLUGINS_LIBS_DIR="$libdir/gedit-2/plugins"
AC_SUBST(GEDIT_PLUGINS_LIBS_DIR)

GEDIT_PLUGINS_DATA_DIR="$datadir/gedit-2/plugins"
AC_SUBST(GEDIT_PLUGINS_DATA_DIR)

AC_CONFIG_FILES([
Makefile
src/Makefile
po/Makefile.in
])

AC_OUTPUT

