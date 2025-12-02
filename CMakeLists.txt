cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MovieSorter CXX)

include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/whoshuu/cpr.git
    GIT_TAG 1.10.5 
)

set(CPR_BUILD_TESTS OFF CACHE BOOL "")
set(CPR_BUILD_EXAMPLES OFF CACHE BOOL "")
set(CPR_GENERATE_CMAKE_CONFIG OFF CACHE BOOL "")
set(CURL_DISABLE_LDAP ON CACHE BOOL "") 

FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(
    run 
    src/main.cpp
    src/api_client.cpp
    src/films.cpp
    src/genres.cpp
    src/film_record.cpp
    src/primary_index.cpp
    src/inverted_index.cpp
    src/trie.cpp
)

target_include_directories(run PRIVATE 
    include 
    ${nlohmann_json_SOURCE_DIR}/include
)

target_link_libraries(run PRIVATE cpr::cpr)
