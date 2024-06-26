cmake_minimum_required(VERSION 3.16)

project(qhotkey
    VERSION 1.5.0
    DESCRIPTION "Global hotkey library for Qt software"
    HOMEPAGE_URL "https://github.com/ffiirree/QHotkey"
    LANGUAGES CXX
)

option(QHOTKEY_EXAMPLES "Build examples"        OFF)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Qt
set(CMAKE_AUTOMOC ON)

if(NOT QT_DEFAULT_MAJOR_VERSION)
    set(QT_DEFAULT_MAJOR_VERSION 5 CACHE STRING "Qt version to use (5 or 6), defaults to 5")
endif()

if(QT_DEFAULT_MAJOR_VERSION EQUAL 6)
    find_package(Qt${QT_DEFAULT_MAJOR_VERSION} 6.2.0 COMPONENTS Core Gui REQUIRED)
else()
    find_package(Qt${QT_DEFAULT_MAJOR_VERSION} COMPONENTS Core Gui REQUIRED)
endif()

add_library(qhotkey QHotkey/qhotkey.cpp)
add_library(QHotkey::QHotkey ALIAS qhotkey)

target_include_directories(qhotkey
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QHotkey>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(qhotkey
    PUBLIC
        Qt${QT_DEFAULT_MAJOR_VERSION}::Core 
        Qt${QT_DEFAULT_MAJOR_VERSION}::Gui
)

if(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    mark_as_advanced(CARBON_LIBRARY)

    target_sources(qhotkey PRIVATE QHotkey/qhotkey_mac.cpp)
    target_link_libraries(qhotkey PRIVATE ${CARBON_LIBRARY})
elseif(WIN32)
    target_sources(qhotkey PRIVATE QHotkey/qhotkey_win.cpp)
else()
    find_package(X11 REQUIRED)

    target_sources(qhotkey PRIVATE QHotkey/qhotkey_x11.cpp)

    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(qhotkey PRIVATE ${X11_LIBRARIES})

    if(QT_DEFAULT_MAJOR_VERSION EQUAL 5)
        find_package(Qt${QT_DEFAULT_MAJOR_VERSION} COMPONENTS X11Extras REQUIRED)

        target_link_libraries(qhotkey PRIVATE Qt${QT_DEFAULT_MAJOR_VERSION}::X11Extras)
    endif()
endif()

target_compile_options(qhotkey
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8 /DUNICODE /D_UNICODE /DNOMINMAX /Zc:preprocessor /Zc:__cplusplus>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(qhotkey PUBLIC QHOTKEY_SHARED PRIVATE QHOTKEY_LIBRARY)
endif()

set_target_properties(qhotkey
    PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION}
        INTERFACE_QHotkey_MAJOR_VERSION ${PROJECT_VERSION_MAJOR}
        COMPATIBLE_INTERFACE_STRING QHotkey_MAJOR_VERSION

        DEBUG_POSTFIX "d"
)

# Examples

if(QHOTKEY_EXAMPLES)
    add_subdirectory(HotkeyTest)
endif()

# Package

# General settings
set(CPACK_PACKAGE_VENDOR "Skycoder42")
set(CPACK_PACKAGE_CONTACT "Shatur")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# CPACK: DEB Specific Settings
set(CPACK_DEBIAN_PACKAGE_NAME "libqhotkey")
set(CPACK_DEBIAN_PACKAGE_SECTION "Libraries")

# Set dependencies
if(QT_DEFAULT_MAJOR_VERSION EQUAL 6)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6x11extras6 (>= 6.2.0)")
else()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5x11extras5 (>= 5.15.2)")
endif()

include(CPack)

# Install

include(CMakePackageConfigHelpers) 

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QHotkeyConfigVersion.cmake
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

include(GNUInstallDirs)

message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/QHotkey)

install(
    TARGETS qhotkey EXPORT QHotkeyConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QHotkey/qhotkey.h             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/QHotkey/QHotkey               DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QHotkeyConfigVersion.cmake    DESTINATION ${INSTALL_CONFIGDIR})

install(EXPORT QHotkeyConfig DESTINATION ${INSTALL_CONFIGDIR})

export(TARGETS qhotkey FILE QHotkeyConfig.cmake)
