include(FetchContent)
FetchContent_Declare(
        cli11
        GIT_REPOSITORY git@github.com:CLIUtils/CLI11.git
        GIT_TAG v2.7.2
        GIT_SHALLOW TRUE
        GIT_SUBMODULES_RECURSE TRUE
    )

set(CLI11_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cli11)

