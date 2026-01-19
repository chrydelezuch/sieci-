cmake_minimum_required(VERSION 3.14)
project(ServerIRC VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(server_irc
    src/main.cpp
    src/CryptoUtils.cpp
    src/Database.cpp
    src/User.cpp
    src/UserMenager.cpp
)

target_include_directories(server_irc PRIVATE include)

find_package(OpenSSL REQUIRED)
target_link_libraries(server_irc PRIVATE OpenSSL::Crypto)

include(FetchContent)

FetchContent_Declare(
  sqliteOrm
  GIT_REPOSITORY https://github.com/fnc12/sqlite_orm
  GIT_TAG dev
)

FetchContent_MakeAvailable(sqliteOrm)

target_link_libraries(server_irc PRIVATE sqlite_orm::sqlite_orm)
