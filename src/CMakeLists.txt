################################################################################
# XDG Desktop Portal implementation for TDE                                    #
# Copyright (C) 2024 Mavridis Philippe <mavridisf@gmail.com>                   #
#                                                                              #
# Improvements and feedback are welcome!                                       #
# This software is licensed under the terms of the GNU GPL v3 license.         #
################################################################################

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TDE_INCLUDE_DIR}
    ${TQT_INCLUDE_DIRS}
    ${DBUS_TQT_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/interfaces
)

link_directories(
    ${TQT_LIBRARY_DIRS}
    ${DBUS_TQT_LIBRARY_DIRS}
    ${CMAKE_BINARY_DIR}/interfaces
)

tde_add_executable(
    xdg-desktop-portal-tde AUTOMOC

    SOURCES
        main.cpp
        portal_service.cpp
        file_chooser_portal.cpp

    LINK
        tdecore-shared
        tdeio-shared
        ${DBUS_TQT_LIBRARIES}
        portalinterfaces-static

    DESTINATION ${LIBEXEC_INSTALL_DIR}
)

# kate: replace-tabs true; tab-width 4; indent-width 4;