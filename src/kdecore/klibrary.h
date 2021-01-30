/* This file is part of the KDE libraries
   Copyright (C) 2007 Bernhard Loos <nhuh.put@web.de.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KLIBRARY_H
#define KLIBRARY_H

#include <kdelibs4support_export.h>

#include <QLibrary>

class KLibraryPrivate;

class KPluginFactory;

/**
 * \class KLibrary klibrary.h <KLibrary>
 *
 * KLibrary searches for libraries in the same way that KPluginLoader searches
 * for plugins.
 *
 * @deprecated since 5.0, use QLibrary and KPluginLoader::findPlugin() instead
 */
class KDELIBS4SUPPORT_DEPRECATED_EXPORT KLibrary : public QLibrary
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
public:
    /**
     * @deprecated since 5.0, use QFunctionPointer
     */
    typedef void (*void_function_ptr)();

    KDELIBS4SUPPORT_DEPRECATED explicit KLibrary(QObject *parent = nullptr);
    KDELIBS4SUPPORT_DEPRECATED explicit KLibrary(const QString &name, QObject *parent = nullptr);
    KLibrary(const QString &name, int verNum, QObject *parent = nullptr);

    ~KLibrary() override;

    /**
     * @deprecated since 4.0, use KPluginLoader::factory
     */
    KDELIBS4SUPPORT_DEPRECATED KPluginFactory *factory(const char *factoryname = nullptr)
    {
        // there is nothing sensible we can do: kdelibs 4 plugins depended on
        // support from Qt that no longer exists
        Q_UNUSED(factoryname) return nullptr;
    }

    /**
     * @deprecated since 5.0, use QLibrary::resolve
     */
    KDELIBS4SUPPORT_DEPRECATED void_function_ptr resolveFunction(const char *name)
    {
        return resolve(name);
    }

    void setFileName(const QString &name);

    bool unload()
    {
        return false;
    }
private:
    KLibraryPrivate *d_ptr;
};

#endif
