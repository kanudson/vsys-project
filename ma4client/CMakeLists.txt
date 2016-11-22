CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

PROJECT(ma4client)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${MA4LIB_INCLUDE_DIR}
    ${SDL2_INC}
    ${BOOST_INCLUDEDIR}
)

file (GLOB SOURCES      src/*.cpp)
file (GLOB HEADERSPP    include/*.hpp)
file (GLOB HEADERS      include/*.h)
add_executable (ma4client ${SOURCES} ${HEADERSPP} ${HEADERS})

target_link_libraries (ma4client ma4lib)
if (WIN32)
    target_link_libraries (ma4client ${SDL2_LIB} ${SDL2_MAINLIB})
    target_link_libraries (ma4client ${Boost_LIBRARIES})
endif (WIN32)

if (UNIX)
    target_link_libraries (ma4client pthread boost_system boost_chrono)
endif (UNIX)

