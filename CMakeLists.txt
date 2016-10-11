CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(ma2vsys)

# in top level cmake config to set this for all projets
SET(CMAKE_CXX_STANDARD 14)

# includes control.cmake.utils/FindSDL2.cmake
#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/control.cmake.utils)

##  Boost Library Paths
if (WIN32)
    SET(BOOST_INC       "boost include dir" CACHE PATH "desc")
    SET(BOOST_LIB       "boost lib dir"     CACHE PATH "desc")
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
)

##  Add OpenMP flags to compiler
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${BOOST_INCLUDEDIR}
)

file (GLOB SOURCES      src/*.cpp)
file (GLOB HEADERSPP    include/*.hpp)
file (GLOB HEADERS      include/*.h)

#add_executable (mavsys ${SOURCES} ${HEADERS} ${HEADERSPP})
add_executable (mavsys ${SOURCES})

if (UNIX)
    target_link_libraries (mavsys pthread boost_system boost_chrono)
endif (UNIX)

