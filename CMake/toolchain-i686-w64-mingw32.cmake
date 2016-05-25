# CMake toolchain for the 32bit compiler of the gcc-mingw-w64 Debian package

# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-c++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
SET(CMAKE_Fortran_COMPILER i686-w64-mingw32-gfortran)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32/)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

