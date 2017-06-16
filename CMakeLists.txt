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
    include(${CMAKE_SOURCE_DIR}/ConfigDefault.txt)
endif()
# generate configure header file
configure_file ("${CMAKE_SOURCE_DIR}/Configure.txt.in" "${CMAKE_SOURCE_DIR}/src/include/Configure.h")
# call the subdirectory's cmakefile to build
add_subdirectory (src)
add_subdirectory (test)
# find system library for AMQP
find_library (AMQP amqpcpp)
find_library (PTHREAD pthread)
find_library (EV ev)
# find mongocxx and bsoncxx
find_package (libbsoncxx)
find_package (libmongocxx)
# add include
include_directories (src/include ${LIBMONGOCXX_INCLUDE_DIRS} ${LIBBSONCXX_INCLUDE_DIRS}) 
# link libraries
target_link_libraries (cbrain MyLib ${AMQP} ${PTHREAD} ${EV} ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})
# tests
include (CTest) # use CTest module of cmake
add_test (NAME Googletest COMMAND mv_avg_test)
