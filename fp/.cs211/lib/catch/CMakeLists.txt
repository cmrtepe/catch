cmake_minimum_required(VERSION 3.3)
project(Catch2
        VERSION 2020.1.0
        DESCRIPTION "A reusable main for Catch2"
        LANGUAGES CXX)

###
### MAIN LIBRARY SETUP
###

add_library(catch
        include/catch.hxx
        src/catch-main.cxx)

set_target_properties(catch PROPERTIES
        CXX_STANDARD            14
        CXX_STANDARD_REQUIRED   On
        CXX_EXTENSIONS          Off
        VERSION                 ${PROJECT_VERSION})

target_include_directories(catch SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_include_directories(catch PRIVATE
        include)

###
### LIBRARY INSTALLATION
###

include(GNUInstallDirs)
install(TARGETS catch EXPORT Catch2Config
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT Catch2Config DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/Catch2)
export(TARGETS catch FILE Catch2Config.cmake)

