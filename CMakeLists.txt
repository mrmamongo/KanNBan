cmake_minimum_required(VERSION 3.8)

project("KanNBan" VERSION 2.0.0 LANGUAGES CXX)

set(BUILD_TESTS "Build Tests" ON)
set(BUILD_COVERAGE "Build Coverage" OFF)

function(add_source_files name folder_src)
    foreach(index IN LISTS ARGN)
        message("arguments at index ${index}: ${ARGV${index}}")
        list(APPEND temp "${CMAKE_CURRENT_SOURCE_DIR}/${folder_src}/${name}/${index}")
    endforeach()
    set(${name} ${temp} PARENT_SCOPE)
endfunction()

set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME})

list(APPEND COMPILE_DEF "PROJECT_COMPILE")

add_compile_definitions(${COMPILE_DEF})
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

add_subdirectory(third_party/nlohmann_json)
add_subdirectory(third_party/fmt)
find_package(Threads REQUIRED)

add_source_files(config src
        config.hpp
        config.cpp
        )

add_executable(${PROJECT_NAME}
        ${config}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        )

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

target_link_directories(${PROJECT_NAME} PRIVATE
        # TODO: Добавить clientserver
        "${CLIENTSERVER_DIR}/$<$<CONFIG:Debug>:debug>/lib"
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        Threads::Threads
        nlohmann_json::nlohmann_json
        fmt::fmt
        )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/resources/config.json" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMAND_EXPAND_LISTS)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
