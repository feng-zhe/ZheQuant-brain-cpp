cmake_minimum_required (VERSION 3.1)
project (CBRAIN)
# enable c++11
set (CMAKE_CXX_STANDARD 11)
# target excutable file
add_executable (cbrain main.cpp)
# include the customized configuration file or use defaults
if(EXISTS "${CMAKE_SOURCE_DIR}/Config.txt")
    include(${CMAKE_SOURCE_DIR}/Config.txt)
else()
    include(${CMAKE_SOURCE_DIR}/ConfigTemplate.txt)
endif()
# generate configure header file
configure_file ("${CMAKE_SOURCE_DIR}/Configure.txt.in" "${CMAKE_SOURCE_DIR}/src/include/Configure.h")
# add include header file directory
include_directories (src/include external_modules/rapidjson/include)
# call the subdirectory's cmakefile to build
add_subdirectory (src)
add_subdirectory (test)
# find system library for AMQP
find_library (AMQP amqpcpp)
find_library (PTHREAD pthread)
find_library (EV ev)
# link libraries
target_link_libraries (cbrain MyLib ${AMQP} ${PTHREAD} ${EV})
# tests
include (CTest) # use CTest module of cmake
add_test (NAME Googletest COMMAND mv_avg_test)
