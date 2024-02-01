################################################################################
# XDG Desktop Portal implementation for TDE                                    #
# Copyright (C) 2024 Mavridis Philippe <mavridisf@gmail.com>                   #
#                                                                              #
# Improvements and feedback are welcome!                                       #
# This software is licensed under the terms of the GNU GPL v3 license.         #
################################################################################

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${DBUS_TQT_INCLUDE_DIRS}
)

link_directories(
    ${DBUS_TQT_LIBRARY_DIRS}
)

set(filechooser_HDRS filechooserInterface.h filechooserProxy.h introspectableInterface.h)
set(filechooser_SRCS filechooserInterface.cpp filechooserProxy.cpp introspectableInterface.cpp)

add_custom_command(
    OUTPUT ${filechooser_HDRS} ${filechooser_SRCS}
    COMMAND ${DBUSXML2QT3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/filechooser.xml
    DEPENDS filechooser.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

tde_add_library(portalinterfaces STATIC_PIC AUTOMOC
    SOURCES ${filechooser_SRCS}
    LINK ${DBUS_TQT_LIBRARIES}
)

configure_file(org.freedesktop.impl.portal.desktop.tde.service.cmake
               org.freedesktop.impl.portal.desktop.tde.service @ONLY)

configure_file(xdg-desktop-portal-tde.desktop.cmake
               xdg-desktop-portal-tde.desktop @ONLY)

install(FILES tde.portal DESTINATION ${SHARE_INSTALL_PREFIX}/xdg-desktop-portal/portals)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.impl.portal.desktop.tde.service
        DESTINATION ${DBUS_SESSION_DIRECTORY})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/xdg-desktop-portal-tde.desktop
        DESTINATION ${SHARE_INSTALL_PREFIX}/applications)