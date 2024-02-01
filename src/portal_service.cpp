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
#include <tqdbusmessage.h>

// TDE
#include <tdemessagebox.h>
#include <kdebug.h>

// Portal
#include "file_chooser_portal.h"
#include "portal_service.h"


static const char *dbusObjectPath = "/org/freedesktop/portal/desktop";
static const char *dbusInterfaceRoot = "org.freedesktop.impl.portal";

TDEPortalService::TDEPortalService(TQT_DBusConnection &connection)
: m_connection(connection)
{
    if (!m_connection.registerObject(dbusObjectPath, this))
        kdFatal() << "Unable to register XDG desktop portal object " << dbusObjectPath << endl;
    else
        kdDebug() << "Registered XDG desktop portal object " << dbusObjectPath << endl;
}

TDEPortalService::~TDEPortalService()
{
    m_connection.unregisterObject(dbusObjectPath);
}

TQT_DBusObjectBase* TDEPortalService::getPortalInterface(TQCString portal)
{
    if (portal == "FileChooser")
        return new TDEFileChooserPortal(m_connection);

    return nullptr;
}

bool TDEPortalService::handleMethodCall(const TQT_DBusMessage &message)
{
    if (!message.interface().startsWith(dbusInterfaceRoot))
        return false;

    TQCString portal = message.interface().mid(TQString(dbusInterfaceRoot).length() + 1).local8Bit();
    TQT_DBusObjectBase *iface = getPortalInterface(portal);
    if (!iface)
    {
        kdWarning() << "Unsupported XDG portal requested: " << portal << endl;
        return false;
    }

    return delegateMethodCall(message, iface);
}

// kate: replace-tabs true; tab-width 4; indent-width 4;