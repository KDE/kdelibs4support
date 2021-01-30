/* This file is part of the KDE libraries
   Copyright (C) 1999 Torben Weis <weis@kde.org>

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
#ifndef KLIBLOADER_H
#define KLIBLOADER_H

#include "kdelibs4support_export.h"

#include "kglobal.h"

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QLibrary>
#include <QtPlugin>

#include "kpluginfactory.h"
#include "kpluginloader.h"
#include "klibrary.h"

#ifndef KDELIBS4SUPPORT_NO_DEPRECATED

class KDELIBS4SUPPORT_DEPRECATED_EXPORT K_EXPORT_COMPONENT_FACTORY_is_deprecated_use_KPluginFactory
{
};

#define K_EXPORT_COMPONENT_FACTORY( libname, factory ) \
    K_EXPORT_COMPONENT_FACTORY_is_deprecated_use_KPluginFactory dummy;

/**
 * \class KLibLoader klibloader.h <KLibLoader>
 *
 * @deprecated since 4.0, use KPluginLoader, KService::createInstance or
 * QLibrary instead.
 *
 * @see QLibrary, KPluginLoader
 * @author Torben Weis <weis@kde.org>
 */
class KDELIBS4SUPPORT_DEPRECATED_EXPORT KLibLoader : public QObject //krazy:exclude=dpointer (private class is kept as a global static)
{
    friend class KLibLoaderPrivate;

    Q_OBJECT
public:
    /**
     * Loads a factory from a plugin.
     *
     * @deprecated since 4.0, use KPluginLoader::factory()
     */
    KPluginFactory *factory(const QString &libname, QLibrary::LoadHints loadHint = {});

    /**
     * Loads and initializes a library.
     *
     * @deprecated since 4.0, use QLibrary directly, or KPluginLoader for
     * plugins; KPluginLoader::findPlugin() can be used if the library is
     * installed in the plugin directory.
     */
    KLibrary *library(const QString &libname, QLibrary::LoadHints loadHint = {});

    /**
     * Returns an error message that can be useful to debug the problem.
     *
     * Returns QString() if the last call to library() was successful.
     * You can call this function more than once. The error message is only
     * reset by a new call to library().
     *
     * @return the last error message, or QString() if there was no error
     */
    QString lastErrorMessage() const;

    /**
     * @deprecated since 4.0, does nothing.
     */
    void unloadLibrary(const QString &libname);

    /**
     * Returns a pointer to the factory.
     *
     * Use this function to get an instance of KLibLoader.
     *
     * @return a pointer to the loader. If no loader exists until now
     *         then one is created.
     *
     * @deprecated since 4.0, use KPluginLoader, KService::createInstance or
     * QLibrary instead.
     */
    static KDELIBS4SUPPORT_DEPRECATED KLibLoader *self();

    /**
     * @deprecated since 5.0, if the library is in a plugin directory, use
     * KPluginLoader::findPlugin(); if it is in a library directory, pass the
     * name directly to QLibrary.
     */
    static KDELIBS4SUPPORT_DEPRECATED QString findLibrary(const QString &libname, const KComponentData &cData = KGlobal::mainComponent());

private:
    ~KLibLoader() override;

    KLibLoader();
};

#endif
#endif
