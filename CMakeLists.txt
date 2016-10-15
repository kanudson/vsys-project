CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(ma4vsys)

# in top level cmake config to set this for all projets
SET(CMAKE_CXX_STANDARD 14)

# includes control.cmake.utils/FindSDL2.cmake

##  Boost Library Paths
if (WIN32)
    SET(BOOST_INC       "boost include dir" CACHE PATH "desc")
    SET(BOOST_LIB       "boost lib dir"     CACHE PATH "desc")

    SET(SDL2_INC        "sdl2 include dir"  CACHE PATH "desc")
    SET(SDL2_LIB        "sdl2 lib"          CACHE FILEPATH "desc")
    SET(SDL2_MAINLIB    "sdl2 main lib"     CACHE FILEPATH "desc")
endif (WIN32)

## All the Boost Library stuff
SET(BOOST_INCLUDEDIR ${BOOST_INC})
SET(BOOST_LIBRARYDIR ${BOOST_LIB})
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
ADD_DEFINITIONS( -DBOOST_ALL_NO_LIB )
FIND_PACKAGE(Boost REQUIRED
    program_options
    filesystem
    system
    thread
)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

##  Add OpenMP flags to compiler
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

ADD_SUBDIRECTORY(ma4lib)
ADD_SUBDIRECTORY(ma4server)
