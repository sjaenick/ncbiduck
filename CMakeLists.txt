cmake_minimum_required(VERSION 3.16)

project(refassign VERSION 0.0.1)

find_package(OpenMP REQUIRED)
find_package(DuckDB REQUIRED)
find_package(Threads)

#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
#SET (CMAKE_CXX_FLAGS                "-Wall -Wpedantic -g -pg")
SET (CMAKE_CXX_FLAGS                "-Wall -Wpedantic -g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")

add_compile_options(-Wall -Wextra)
add_link_options(-s)

add_library(refassign STATIC ${SOURCES}
  src/scorefunc.cpp
  src/database.cpp
  src/taxon.cpp
  src/refseeker.cpp
)

target_include_directories(refassign PUBLIC src/)

add_executable(createdb src/createdb.cpp)
add_executable(getlineage src/getlineage.cpp)
add_executable(test_all src/test_all.cpp)

target_link_libraries(createdb PUBLIC refassign ${DuckDB_LIBRARIES} OpenMP::OpenMP_CXX)
target_link_libraries(getlineage PUBLIC refassign ${DuckDB_LIBRARIES} OpenMP::OpenMP_CXX)
target_link_libraries(test_all PUBLIC refassign ${DuckDB_LIBRARIES} OpenMP::OpenMP_CXX)

install(TARGETS createdb DESTINATION bin)
install(TARGETS getlineage DESTINATION bin)
install(TARGETS test_all DESTINATION bin)
