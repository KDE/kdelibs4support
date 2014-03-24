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

#include "kde4support_export.h"

#include "kglobal.h"

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QLibrary>
#include <QtCore/QtPlugin>

#include "kpluginfactory.h"
#include "kpluginloader.h"
#include "klibrary.h"

#ifndef KDE_NO_DEPRECATED

class KDE4SUPPORT_DEPRECATED_EXPORT K_EXPORT_COMPONENT_FACTORY_is_deprecated_use_KPluginFactory
{
};

#define K_EXPORT_COMPONENT_FACTORY( libname, factory ) \
    K_EXPORT_COMPONENT_FACTORY_is_deprecated_use_KPluginFactory dummy;

/**
 * \class KLibLoader klibloader.h <KLibLoader>
 *
 * The KLibLoader allows you to load libraries dynamically at runtime.
 * Dependent libraries are loaded automatically.
 *
 * KLibLoader follows the singleton pattern. You can not create multiple
 * instances. Use self() to get a pointer to the loader.
 *
 * @deprecated You have two other possibilites:
 *  - KPluginLoader or KService::createInstance for plugins
 *  - KLibrary for other libraries
 *
 * @see KLibrary
 * @see KPluginLoader
 * @author Torben Weis <weis@kde.org>
 */
class KDE4SUPPORT_DEPRECATED_EXPORT KLibLoader : public QObject //krazy:exclude=dpointer (private class is kept as a global static)
{
    friend class KLibrary;
    friend class KLibraryPrivate;
    friend class KLibLoaderPrivate;

    Q_OBJECT
public:
    /**
     * Loads and initializes a library. Loading a library multiple times is
     * handled gracefully.
     *
     * This is a convenience function that returns the factory immediately
     * @param libname  This is the library name without extension. Usually that is something like
     *                 "libkspread". The function will then search for a file named
     *                 "libkspread.la" in the KDE library paths.
     *                 The *.la files are created by libtool and contain
     *                 important information especially about the libraries dependencies
     *                 on other shared libs. Loading a "libfoo.so" could not solve the
     *                 dependencies problem.
     *
     *                 You can, however, give a library name ending in ".so"
     *                 (or whatever is used on your platform), and the library
     *                 will be loaded without resolving dependencies. Use with caution.
     * @param loadHint provides more control over how the library is loaded
     * @return the KPluginFactory, or 0 if the library does not exist or it does
     *         not have a factory
     * @see library
     */
    KPluginFactory *factory(const QString &libname, QLibrary::LoadHints loadHint = 0);

    /**
     * Loads and initializes a library. Loading a library multiple times is
     * handled gracefully.
     *
     * @param libname  This is the library name without extension. Usually that is something like
     *                 "libkspread". The function will then search for a file named
     *                 "libkspread.la" in the KDE library paths.
     *                 The *.la files are created by libtool and contain
     *                 important information especially about the libraries dependencies
     *                 on other shared libs. Loading a "libfoo.so" could not solve the
     *                 dependencies problem.
     *
     *                 You can, however, give a library name ending in ".so"
     *                 (or whatever is used on your platform), and the library
     *                 will be loaded without resolving dependencies. Use with caution.
     * @param loadHint provides more control over how the library is loaded
     * @return KLibrary is invalid (0) when the library couldn't be dlopened. in such
     * a case you can retrieve the error message by calling KLibLoader::lastErrorMessage()
     *
     * @see factory
     */
    KLibrary *library(const QString &libname, QLibrary::LoadHints loadHint = 0);

    /**
     * Returns an error message that can be useful to debug the problem.
     * Returns QString() if the last call to library() was successful.
     * You can call this function more than once. The error message is only
     * reset by a new call to library().
     * @return the last error message, or QString() if there was no error
     */
    QString lastErrorMessage() const;

    /**
     * Unloads the library with the given name.
     * @param libname  This is the library name without extension. Usually that is something like
     *                 "libkspread". The function will then search for a file named
     *                 "libkspread.la" in the KDE library paths.
     *                 The *.la files are created by libtool and contain
     *                 important information especially about the libraries dependencies
     *                 on other shared libs. Loading a "libfoo.so" could not solve the
     *                 dependencies problem.
     *
     *                 You can, however, give a library name ending in ".so"
     *                 (or whatever is used on your platform), and the library
     *                 will be loaded without resolving dependencies. Use with caution.
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
     * @deprecated use KPluginLoader instead
     */
    static KDE4SUPPORT_DEPRECATED KLibLoader *self();

    /**
     * Helper method which looks for a library in the standard paths
     * ("module" and "lib" resources).
     * Made public for code that doesn't use KLibLoader itself, but still
     * wants to open modules.
     * @param libname of the library. If it is not a path, the function searches in
     *                the "module" and "lib" resources. If there is no extension,
     *                ".la" will be appended.
     * @param cData a KComponentData used to get the standard paths
     * @return the name of the library if it was found, an empty string otherwise
     */
    static QString findLibrary(const QString &libname, const KComponentData &cData = KGlobal::mainComponent());

private:
    ~KLibLoader();

    KLibLoader();
};

#endif
#endif
