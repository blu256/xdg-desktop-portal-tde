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

#ifndef __UTIL_H
#define __UTIL_H

// TQt
#include <tqdbusvariant.h>
#include <tqdbusdatalist.h>
#include <tqvariant.h>

// TDE
#include <kurl.h>

inline bool check_variant(TQT_DBusVariant variant, TQString signature)
{
    return !variant.signature.isNull() && variant.signature == signature;
}

inline TQString bytelist_to_string(TQT_DBusDataList list)
{
    if (list.type() != TQT_DBusData::Byte) return TQString::null;
    TQCString bytes;
    TQValueList<TQ_UINT8> vl = list.toByteList();
    TQValueList<TQ_UINT8>::iterator vlit;
    for (vlit = vl.begin(); vlit != vl.end(); ++vlit)
    {
        bytes += (*vlit);
    }
    return TQString::fromLocal8Bit(bytes.data(), bytes.size());
}

inline TQT_DBusDataList string_to_bytelist(TQString str)
{
    TQCString bytes = str.latin1();
    TQValueList<TQ_UINT8> vl;
    char *bit;
    for (bit = bytes.begin(); bit != bytes.end(); ++bit)
    {
        vl << (TQ_UINT8)(*bit);
    }
    return TQT_DBusDataList(vl);
}

inline TQT_DBusDataList kurl_list_to_datalist(KURL::List urls)
{
    return TQT_DBusDataList(urls.toStringList());
}

inline bool make_variant(TQVariant variant, TQT_DBusVariant &other)
{
    TQString signature;
    switch (variant.type()) {
        case TQVariant::String:
        case TQVariant::Color:
        case TQVariant::Font:
        case TQVariant::Date:
        case TQVariant::DateTime:
        case TQVariant::Time:
        case TQVariant::KeySequence:
            other.signature = "s";
            other.value = TQT_DBusData::fromString(variant.toString());
            break;

        case TQVariant::Int:
            other.signature = "i";
            other.value = TQT_DBusData::fromInt32(variant.toInt());
            break;

        case TQVariant::UInt:
            other.signature = "u";
            other.value = TQT_DBusData::fromUInt32(variant.toUInt());
            break;

        case TQVariant::Double:
        case TQVariant::LongLong:
        case TQVariant::ULongLong:
            other.signature = "d";
            other.value = TQT_DBusData::fromDouble(variant.toDouble());
            break;

        case TQVariant::Bool:
            other.signature = "b";
            other.value = TQT_DBusData::fromBool(variant.toBool());
            break;

        case TQVariant::StringList:
            other.signature = "as";
            other.value = TQT_DBusData::fromList(TQT_DBusDataList(variant.toStringList()));
            break;

        default:
            if (variant.canCast(TQVariant::String)) {
                other.signature = "s";
                other.value = TQT_DBusData::fromString(variant.toString());
            }
            else {
                kdDebug() << "Cannot cast TQVariant of type " << variant.type()
                          << "to TQT_DBusVariant!" << endl;
                return false;
            }
    }
    return true;
}

#endif // __UTIL_H

// kate: replace-tabs true; tab-width 4; indent-width 4;