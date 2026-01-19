# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-src"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-build"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/tmp"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/src/sqliteorm-populate-stamp"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/src"
  "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/src/sqliteorm-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/src/sqliteorm-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/chrystian/Pulpit/serwer_irc/build/_deps/sqliteorm-subbuild/sqliteorm-populate-prefix/src/sqliteorm-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
