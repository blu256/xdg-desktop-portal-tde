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

#ifndef __FILE_CHOOSER_PORTAL_H
#define __FILE_CHOOSER_PORTAL_H

// TQt
#include <tqdbusconnection.h>
#include <tqdbusmessage.h>
#include <tqdbusvariant.h>
#include <tqdbuserror.h>
#include <tqobject.h>

// TDE
#include <tdefile.h>

// Portal
#include "filechooserInterface.h"

struct FileDialogOpts
{
    TQString caption;
    TQString okButtonText;
    TQString startDir;
    TQString startName;
    TQString filters;
    bool multiple = false;
    bool directory = false;
    bool savingMode = false;
    bool modal = true;
    WId windowId = 0;

    inline KFile::Mode mode()
    {
        if (directory) return KFile::Directory;
        return multiple ? KFile::Files : KFile::File;
    }
};

class KFileDialog;

class TDEFileChooserPortal : public TQObject,
                             public org::freedesktop::impl::portal::FileChooserInterface
{
    public:
        TDEFileChooserPortal(TQT_DBusConnection &connection);
        virtual ~TDEFileChooserPortal();

    protected:
        virtual bool OpenFile(const TQT_DBusObjectPath& handle,
                              const TQString& app_id,
                              const TQString& parent_window,
                              const TQString& title,
                              const TQMap<TQString, TQT_DBusVariant> &options,
                              TQ_UINT32& response,
                              TQMap<TQString, TQT_DBusVariant> &results,
                              TQT_DBusError& error);

        virtual bool SaveFile(const TQT_DBusObjectPath& handle,
                              const TQString& app_id,
                              const TQString& parent_window,
                              const TQString& title,
                              const TQMap<TQString, TQT_DBusVariant> &options,
                              TQ_UINT32& response,
                              TQMap<TQString, TQT_DBusVariant> &results,
                              TQT_DBusError& error);

        virtual bool SaveFiles(const TQT_DBusObjectPath& handle,
                               const TQString& app_id,
                               const TQString& parent_window,
                               const TQString& title,
                               const TQMap<TQString, TQT_DBusVariant> &options,
                               TQ_UINT32& response,
                               TQMap<TQString, TQT_DBusVariant> &results,
                               TQT_DBusError& error);

        virtual void handleMethodReply(const TQT_DBusMessage &reply);

    private:
        TQT_DBusConnection m_connection;

        WId parseWindowId(const TQString data);
        TQString parseFilter(const TQT_DBusData data);
        TQString parseFilterList(const TQT_DBusVariant filterData,
                                 const TQT_DBusVariant currentFilterData);

        bool execFileDialog(FileDialogOpts options,
                            const TQT_DBusObjectPath& handle,
                            TQ_UINT32& response,
                            TQMap<TQString, TQT_DBusVariant> &results,
                            TQT_DBusError& error);
};

#endif // __FILE_CHOOSER_PORTAL_H

// kate: replace-tabs true; tab-width 4; indent-width 4;