Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(${PROJECT_NAME}_TEST
        utils_tests/thread_pool_tests.cpp
        )

target_include_directories(${PROJECT_NAME}_TEST PRIVATE
        ..
        ../src
        )

target_link_libraries(${PROJECT_NAME}_TEST PRIVATE Catch2::Catch2WithMain ${PROJECT_NAME}_LIB)