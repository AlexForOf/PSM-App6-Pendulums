cmake_minimum_required(VERSION 3.14)

project(Pendulums)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(DCMAKE_BUILD_TYPE=Release)

set(BUILD_SHARED_LIBS OFF)

#set(CMAKE_EXE_LINKER_FLAGS "-static")

include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.x  
)
FetchContent_MakeAvailable(sfml)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Pendulums ${SOURCES})

target_include_directories(Pendulums PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/pendulums
)

target_link_libraries(Pendulums PRIVATE sfml-graphics sfml-window sfml-system)
