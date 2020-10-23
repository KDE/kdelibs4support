/* This file is part of the KDE libraries
   Copyright (C) 1999 Torben Weis <weis@kde.org>
   Copyright (C) 2000 Michael Matz <matz@kde.org>

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
#include "klibloader.h"

#include <QFile>
#include <QDir>
#include <QTimer>
#include <QStack>
#include <QCoreApplication>
#include <QObjectCleanupHandler>

#include "kcomponentdata.h"
#include "kdebug.h"
#include "klocalizedstring.h"

class KLibLoaderPrivate
{
public:
    KLibLoader instance;
    QObjectCleanupHandler cleanuphandler;
    QString errorString;
};

Q_GLOBAL_STATIC(KLibLoaderPrivate, kLibLoaderPrivate)

#define KLIBLOADER_PRIVATE KLibLoaderPrivate *const d = kLibLoaderPrivate

KLibLoader *KLibLoader::self()
{
    qWarning() << "Using deprecated KLibLoader!";
    return &kLibLoaderPrivate()->instance;
}

KLibLoader::KLibLoader()
    : QObject(nullptr)
{
}

KLibLoader::~KLibLoader()
{
}

extern QString makeLibName(const QString &libname);

extern KDELIBS4SUPPORT_DEPRECATED_EXPORT QString findLibrary(const QString &name);

#ifdef Q_OS_WIN
// removes "lib" prefix, if present
QString fixLibPrefix(const QString &libname)
{
    int pos = libname.lastIndexOf(QLatin1Char('/'));
    if (pos >= 0) {
        QString file = libname.mid(pos + 1);
        QString path = libname.left(pos);
        if (!file.startsWith(QLatin1String("lib"))) {
            return libname;
        }
        return path + QLatin1Char('/') + file.mid(3);
    }
    if (!libname.startsWith(QLatin1String("lib"))) {
        return libname;
    }
    return libname.mid(3);
}
#endif

//static
QString KLibLoader::findLibrary(const QString &_name, const KComponentData &cData)
{
    Q_UNUSED(cData); // removed as part of the KF5 changes
    return ::findLibrary(_name);
}

KLibrary *KLibLoader::library(const QString &_name, QLibrary::LoadHints hint)
{
    if (_name.isEmpty()) {
        return nullptr;
    }

    KLibrary *lib = new KLibrary(_name);

    // Klibrary search magic did work?
    if (lib->fileName().isEmpty()) {
        kLibLoaderPrivate()->errorString = i18n("Library \"%1\" not found", _name);
        delete lib;
        return nullptr;
    }

    lib->setLoadHints(hint);

    lib->load();

    if (!lib->isLoaded()) {
        kLibLoaderPrivate()->errorString = lib->errorString();
        delete lib;
        return nullptr;
    }

    kLibLoaderPrivate()->cleanuphandler.add(lib);

    return lib;
}

QString KLibLoader::lastErrorMessage() const
{
    return kLibLoaderPrivate()->errorString;
}

void KLibLoader::unloadLibrary(const QString &)
{
}

KPluginFactory *KLibLoader::factory(const QString &_name, QLibrary::LoadHints hint)
{
    Q_UNUSED(hint)

    KPluginLoader plugin(_name);
    return plugin.factory();
}

