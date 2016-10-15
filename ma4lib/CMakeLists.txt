CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(ma4lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${BOOST_INCLUDEDIR}
)

file (GLOB SOURCES      src/*.cpp)
file (GLOB HEADERSPP    include/ma4lib/*.hpp)
file (GLOB HEADERS      include/ma4lib/*.h)

add_library (ma4lib STATIC ${SOURCES} ${HEADERSPP} ${HEADERS})

if (WIN32)
    target_link_libraries (ma4lib ${SDL2_MAINLIB})
endif (WIN32)

if (UNIX)
    target_link_libraries (ma4lib pthread boost_system boost_chrono)
endif (UNIX)

#   make include paths for headers available for other projects in the solution
set(MA4LIB_INCLUDE_DIR
    "${PROJECT_SOURCE_DIR}/include"
    PARENT_SCOPE
    )
