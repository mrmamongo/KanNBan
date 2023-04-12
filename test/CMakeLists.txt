Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(KanNBan_Tests test.cpp)
target_link_libraries(KanNBan_Tests PRIVATE Catch2::Catch2WithMain)