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
#include <tqregexp.h>
#include <tqdbusobjectpath.h>

// TDE
#include <tdefiledialog.h>
#include <kpushbutton.h>
#include <twin.h>
#include <kdebug.h>

// Portal
#include "file_chooser_portal.h"
#include "file_chooser_portal.moc"
#include "util.h"

#define OPTION_VALID(opt, sig) \
    options.keys().contains(opt) && check_variant(options[opt], sig)


TDEFileChooserPortal::TDEFileChooserPortal(TQT_DBusConnection &connection)
    : m_connection(connection)
{}

TDEFileChooserPortal::~TDEFileChooserPortal()
{}

bool TDEFileChooserPortal::OpenFile(const TQT_DBusObjectPath& handle,
                                    const TQString& app_id,
                                    const TQString& parent_window,
                                    const TQString& title,
                                    const TQMap<TQString, TQT_DBusVariant> &options,
                                    TQ_UINT32& response,
                                    TQMap<TQString, TQT_DBusVariant> &results,
                                    TQT_DBusError& error)
{
    FileDialogOpts opts;

    opts.caption = title;

    if (OPTION_VALID("accept_label", "s"))
        opts.okButtonText = options["accept_label"].value.toString();

    if (OPTION_VALID("directory", "b"))
        opts.directory = options["directory"].value.toBool();

    if (OPTION_VALID("multiple", "b"))
        opts.multiple = options["multiple"].value.toBool();

    if (OPTION_VALID("modal", "b"))
        opts.modal = options["modal"].value.toBool();

    if (OPTION_VALID("filters", "a(sa(us))"))
        opts.filters = parseFilterList(options["filters"], options["current_filter"]);

    opts.windowId = parseWindowId(parent_window);

    return execFileDialog(opts, handle, response, results, error);
}

bool TDEFileChooserPortal::SaveFile(const TQT_DBusObjectPath& handle,
                                    const TQString& app_id,
                                    const TQString& parent_window,
                                    const TQString& title,
                                    const TQMap<TQString, TQT_DBusVariant> &options,
                                    TQ_UINT32& response,
                                    TQMap<TQString, TQT_DBusVariant> &results,
                                    TQT_DBusError& error)
{
    FileDialogOpts opts;

    opts.caption = title;

    if (OPTION_VALID("accept_label", "s"))
        opts.okButtonText = options["accept_label"].value.toString();

    if (OPTION_VALID("directory", "b"))
        opts.directory = options["directory"].value.toBool();

    if (OPTION_VALID("multiple", "b"))
        opts.multiple = options["multiple"].value.toBool();

    if (OPTION_VALID("modal", "b"))
        opts.modal = options["modal"].value.toBool();

    if (OPTION_VALID("filters", "a(sa(us))"))
        opts.filters = parseFilterList(options["filters"], options["current_filter"]);

    if (OPTION_VALID("current_folder", "ay"))
        opts.startDir = bytelist_to_string(options["current_folder"].value.toList().toByteList());

    if (OPTION_VALID("current_name", "s"))
        opts.startName = options["current_name"].value.toString();

    opts.windowId = parseWindowId(parent_window);

    return execFileDialog(opts, handle, response, results, error);
}

bool TDEFileChooserPortal::SaveFiles(const TQT_DBusObjectPath& handle,
                                     const TQString& app_id,
                                     const TQString& parent_window,
                                     const TQString& title,
                                     const TQMap<TQString, TQT_DBusVariant> &options,
                                     TQ_UINT32& response,
                                     TQMap<TQString, TQT_DBusVariant> &results,
                                     TQT_DBusError& error)
{
    return false;
}

void TDEFileChooserPortal::handleMethodReply(const TQT_DBusMessage &reply)
{
    m_connection.send(reply);
}

bool TDEFileChooserPortal::execFileDialog(FileDialogOpts options,
                                          const TQT_DBusObjectPath& handle,
                                          TQ_UINT32& response,
                                          TQMap<TQString, TQT_DBusVariant> &results,
                                          TQT_DBusError& error)
{
    KFileDialog *dialog = new KFileDialog(options.startDir, TQString::null,
                                          nullptr, "xdg-tde-file-chooser",
                                          options.modal);
    uint mode = KFile::LocalOnly;
    if (options.savingMode)
    {
        dialog->setOperationMode(KFileDialog::Saving);
    }
    else {
        mode |= KFile::ExistingOnly;
    }
    dialog->setMode(mode | options.mode());

    if (!options.caption.isNull())
            dialog->setPlainCaption(options.caption);

    if (!options.okButtonText.isNull())
            dialog->okButton()->setText(options.okButtonText);

    if (!options.filters.isNull())
            dialog->setFilter(options.filters);

    dialog->setSelection(options.startName);

    if (options.windowId > 0) KWin::setMainWindow(dialog, options.windowId);

    if (dialog->exec() == TQDialog::Accepted)
    {
        response = 0;
        TQT_DBusDataList urls = kurl_list_to_datalist(dialog->selectedURLs());
        TQT_DBusVariant var = TQT_DBusData::fromList(urls).getAsVariantData().toVariant();
        results.insert("uris", var);
    }
    else response = 1;
    return true;
}

WId TDEFileChooserPortal::parseWindowId(const TQString data)
{
    if (!data.startsWith("x11:"))
    {
        kdWarning() << "[FileChooser] Window Identifiers are currently only "
                    << "supported for X11. Created dialog will be parentless."
                    << endl;
        return 0;
    }

    bool ok;
    WId wid = data.mid(4).toInt(&ok, 16);
    return ok ? wid : 0;
}

TQString TDEFileChooserPortal::parseFilter(const TQT_DBusData data)
{
    TQStringList patternList;

    TQValueList<TQT_DBusData> filterData = data.toStruct();

    TQString label = filterData[0].toString();
    TQValueList<TQT_DBusData> patterns = filterData[1].toTQValueList();

    TQValueList<TQT_DBusData>::iterator fp;
    for (fp = patterns.begin(); fp != patterns.end(); ++fp)
    {
        TQValueList<TQT_DBusData> patternData = (*fp).toStruct();
        bool isMime = (patternData[0].toUInt32() == 1);
        if (isMime) {
            // KFileDialog cannot handle both a mime and a simple
            // extension filter, so in case we get a mimetype,
            // we just get the associated extensions from the
            // MIME system
            TQString mime = patternData[1].toString();
            patternList += KMimeType::mimeType(mime)->patterns();
        }
        else {
            TQString patternString = patternData[1].toString();

            // Regex patterns like *.[hH][tT][mM][lL] are unnecessary for us
            // and causes a problem with the save dialog's extension autoselection
            TQString finalPattern = patternString;
            if (TQRegExp("[*.](?:[[](?:[A-Za-z]{2})[]])+").search(patternString) != -1)
            {
                finalPattern = "*.";
                int pos = patternString.find('[');
                while (pos != -1)
                {
                    finalPattern += patternString[pos + 1];
                    pos = patternString.find('[', patternString.find(']', pos));
                }
            }
            patternList += finalPattern;
        }
    }

    TQString patternString = patternList.join(" ");

    return TQString("%1|%2 (%3)").arg(patternString, label, patternString);
}

TQString TDEFileChooserPortal::parseFilterList(const TQT_DBusVariant filterData,
                                               const TQT_DBusVariant currentFilterData)
{
    TQStringList filterList;
    TQValueList<TQT_DBusData> filters = filterData.value.toTQValueList();
    if (filters.count() > 0) {
        TQValueList<TQT_DBusData>::iterator f;
        for (f = filters.begin(); f != filters.end(); ++f)
                filterList += parseFilter((*f));
    }

    if (check_variant(currentFilterData, "(sa(us))"))
    {
        TQString currentFilter = parseFilter(currentFilterData.value);
        if (filterList.contains(currentFilter))
        {
            // We have no way to affect the filter selection of the dialog,
            // so we just move the current filter to the top of the list
            // to get it selected automatically
            filterList.remove(currentFilter);
            filterList.prepend(currentFilter);
        }
    }

    return filterList.join("\n");
}

// kate: replace-tabs true; tab-width 4; indent-width 4;