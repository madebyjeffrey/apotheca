
cmake_minimum_required(VERSION 2.6)
project(apotheca)

find_package(PNG)
find_package(Freetype)

option(USE_CLANG "build application with clang" ON)



if (USE_CLANG)
    SET (CMAKE_C_COMPILER_INIT               "/usr/bin/clang")
    SET (CMAKE_C_FLAGS_INIT                  "-Wall -std=c99")
    SET (CMAKE_C_FLAGS_DEBUG_INIT            "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL_INIT       "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE_INIT          "-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT   "-O2 -g")

    SET (CMAKE_CXX_COMPILER_INIT             "/usr/bin/clang++")
    SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
    SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

endif (USE_CLANG)

SET (CMAKE_C_FLAGS "-std=c99")

if (PNG_FOUND)
        include_directories(${PNG_INCLUDE_DIRS})
        add_definitions(${PNG_DEFINITIONS})
endif (PNG_FOUND)

if (FREETYPE_FOUND)
        include_directories(${FREETYPE_INCLUDE_DIRS})
endif (FREETYPE_FOUND)



add_subdirectory(eikon)
add_subdirectory(demo)


