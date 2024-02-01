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

#ifndef __PORTAL_H
#define __PORTAL_H

// TQt
#include <tqdbusconnection.h>
#include <tqdbusobject.h>
#include <tqobject.h>


class Portal : public TQObject, public TQT_DBusObjectBase
{
  TQ_OBJECT

  public:
    Portal(const TQT_DBusConnection &connection);
    virtual ~Portal();

    const TQT_DBusConnection connection() { return m_connection; }

  protected:
    virtual bool handleMethodCall(const TQT_DBusMessage &message);

  private:
    TQT_DBusConnection m_connection;
};

#endif // __PORTAL_H