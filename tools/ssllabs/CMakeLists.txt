cmake_minimum_required(VERSION 3.2)
project(ssllabs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_INSTALL_PREFIX /usr/local/)

set(RUNTIME_INSTALL_DIR $(CMAKE_INSTALL_PREFIX}/bin CACHE PATH "Install dir for executables"))

set(SOURCE_FILES ssllabs.cpp)

# linked libraries
set(LIBS ${LIBS} curl)

add_executable(ssltest ${SOURCE_FILES})
add_library(ssllabs STATIC IMPORTED)
set_target_properties(ssllabs PROPERTIES IMPORTED_LOCATION ../../lib/src/libssllabs.a)
target_link_libraries(ssltest ssllabs ${LIBS})
