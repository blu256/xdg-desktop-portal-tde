/*******************************************************************************
  XDG desktop portal implementation for TDE
  Copyright © 2024 Mavridis Philippe <mavridisf@gmail.com>

  This program or library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

  Improvements and feedback are welcome!
*******************************************************************************/

// TQt
#include <tqdbusconnection.h>

// TDE
#include <kuniqueapplication.h>
#include <tdeaboutdata.h>
#include <tdecmdlineargs.h>
#include <tdelocale.h>

// Portal
#include "portal_service.h"


static const char description[] = I18N_NOOP("TDE XDG desktop portal");
static TDECmdLineOptions options[] = {};

int main(int argc, char **argv)
{
    TDEAboutData about("xdg-desktop-portal-tde", "xdg-desktop-portal-tde", "1.0",
                       description, TDEAboutData::License_LGPL,
                       I18N_NOOP("Copyright © 2024 Mavridis Philippe"));

    TDECmdLineArgs::init(argc, argv, &about);
    TDECmdLineArgs::addCmdLineOptions(options);
    KUniqueApplication::addCmdLineOptions();

    if (!KUniqueApplication::start())
        return 0;

    KUniqueApplication app;

    TQT_DBusConnection connection = TQT_DBusConnection::sessionBus();
    if (!connection.isConnected())
        tqFatal("Failed to connect to session bus!");

    if (!connection.requestName("org.freedesktop.impl.portal.desktop.tde"))
        tqFatal("Failed to register XDG portal service!");

    TDEPortalService portal(connection);

    return app.exec();
}

// kate: replace-tabs true; tab-width 4; indent-width 4;