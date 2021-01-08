cmake_minimum_required(VERSION 3.0.0)

project(IRMorph)


IF(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
    set(SDL2_LIBRARY ${SDL2_LIBRARY} "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/SDL2.lib")
    set(SDL2MAIN_LIBRARY ${SDL2MAIN_LIBRARY} "${CMAKE_SOURCE_DIR}/SDL2/lib/x64/SDL2main.lib")
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/SDL2/include")
    message(${SDL2_LIBRARY})
    if(NOT EXISTS ${SDL2_LIBRARY} AND NOT EXISTS ${SDL2MAIN_LIBRARY} AND NOT EXISTS ${SDL2_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2 not found, please rename the folder SDL2-devel-XX to SDL2 and add the folder to the root directory, SDL2 can be found on SDL website")
    else()
        message("SDL2 Found")
        find_package(SDL2 REQUIRED)
    endif()
ENDIF()

IF(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules (SDL2 REQUIRED sdl2)
ENDIF()

set(SOURCES_DIR "${CMAKE_SOURCE_DIR}/src")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/include")
set(PROJECT_HEADER_DIR "${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES_FILES "${SOURCES_DIR}/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${HEADER_DIR}/*.h")

add_executable(IRMorph ${SOURCES_FILES})
target_link_libraries(IRMorph ${SDL2_LIBRARIES})
target_include_directories(IRMorph PRIVATE ${SDL2_INCLUDE_DIRS} ${PROJECT_HEADER_DIR})

IF(WIN32)
add_custom_command(TARGET IRMorph POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/SDL2/lib/x64/SDL2.dll
        ${CMAKE_BINARY_DIR}/$<CONFIGURATION>)
ENDIF()

add_custom_command(TARGET IRMorph POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/ressources
        ${CMAKE_BINARY_DIR}/ressources)