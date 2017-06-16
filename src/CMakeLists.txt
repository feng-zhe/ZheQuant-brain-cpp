include_directories (include)
aux_source_directory (. DIR_MYLIB_CPP)
# declare the library name to create
add_library (MyLib ${DIR_MYLIB_CPP})
# for mongocxx
find_package (libbsoncxx)
find_package (libmongocxx)
# add include
target_include_directories (MyLib
    PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
    PRIVATE ${LIBBSONCXX_INCLUDE_DIRS}
)
target_link_libraries (MyLib
    PRIVATE ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES} 
)
