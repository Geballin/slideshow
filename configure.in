dnl fichier configure.in utilisé par autoconf
AC_INIT(slideshow.cpp)
AM_INIT_AUTOMAKE(slideshow, 0.8)
AC_PROG_MAKE_SET
AC_PROG_CXX
AC_PROG_INSTALL

dnl Check for SDL
SDL_VERSION=1.2.12
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS -lSDL_image"

AC_OUTPUT(Makefile)