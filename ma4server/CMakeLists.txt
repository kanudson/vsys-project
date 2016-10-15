CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(ma4server)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${MA4LIB_INCLUDE_DIR}
    ${SDL2_INC}
    ${BOOST_INCLUDEDIR}
)

file (GLOB SOURCES      src/*.cpp)
file (GLOB HEADERSPP    include/*.hpp)
file (GLOB HEADERS      include/*.h)

add_executable (ma4server ${SOURCES} ${HEADERSPP} ${HEADERS})

target_link_libraries (ma4server ma4lib)
if (WIN32)
    target_link_libraries (ma4server ${SDL2_LIB} ${SDL2_MAINLIB})
endif (WIN32)

if (UNIX)
    target_link_libraries (ma4server pthread boost_system boost_chrono)
endif (UNIX)

