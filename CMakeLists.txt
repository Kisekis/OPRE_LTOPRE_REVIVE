cmake_minimum_required(VERSION 3.21)
project(EP)

set(CMAKE_CXX_STANDARD 14)
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


include_directories(${ROOT_DIR}/lib/src)
link_directories(${ROOT_DIR}/lib/w64)


add_executable(EP EP.cpp Graph.cpp LT-OPRE.cpp my_random.cpp OPRE.cpp Revive.cpp)
#
target_link_libraries(EP -lglpk_4_65)


