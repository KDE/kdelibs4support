/* This file is part of the KDE libraries
    Copyright (c) 2005-2006 David Faure <faure@kde.org>

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

#undef QT_NO_CAST_FROM_ASCII

#include "klibloadertest.h"

#include <QTest>

QTEST_MAIN(KLibLoaderTest)

#include <klibloader.h>
#include <QDir>
#include <QDebug>

// Only new-style plugins are supported, even though we are doing
// loading with a deprecated class.
static const char s_kpluginFactoryModule[] = "klibloadertestmodule5";
#if defined(Q_OS_WIN) || defined(Q_OS_CYGWIN)
static const char s_modExt[] = ".dll";
#else
static const char s_modExt[] = ".so";
#endif
#define MODULE_PATH(mod) QFileInfo(QFINDTESTDATA(QString::fromLatin1(mod) + s_modExt)).canonicalFilePath()

void KLibLoaderTest::initTestCase()
{
    const QString libdir = QDir::currentPath();
    qDebug() << "Adding" << libdir << "to LD_LIBRARY_PATH";
    qputenv("LD_LIBRARY_PATH", qgetenv("LD_LIBRARY_PATH") + ":" + QFile::encodeName(libdir));
}

void KLibLoaderTest::testFactory()
{
    KPluginFactory *factory = KLibLoader::self()->factory(s_kpluginFactoryModule);
    if (!factory) {
        QVERIFY(factory);
    } else {
        QObject *obj = factory->create<QObject>();
        QVERIFY(obj);
        delete obj;
    }
}

void KLibLoaderTest::testFactory_hints()
{
    // the hints will be ignored, but we want to check the call will still compile
    KPluginFactory *factory = KLibLoader::self()->factory(s_kpluginFactoryModule,
            QLibrary::ResolveAllSymbolsHint);
    if (!factory) {
        QVERIFY(factory);
    } else {
        QObject *obj = factory->create<QObject>();
        QVERIFY(obj);
        delete obj;
    }
}

void KLibLoaderTest::testFactory_noexist()
{
    KPluginFactory *factory = KLibLoader::self()->factory("idontexist");
    QVERIFY(!factory);
}

void KLibLoaderTest::testLibrary()
{
    KLibrary *lib = KLibLoader::self()->library(s_kpluginFactoryModule);
    QVERIFY(lib);
    QVERIFY(lib->isLoaded());
    QCOMPARE(QFileInfo(lib->fileName()).canonicalFilePath(),
             MODULE_PATH(s_kpluginFactoryModule));
}

void KLibLoaderTest::testLibrary_hints()
{
    // the hints will be ignored, but we want to check the call will still compile
    KLibrary *lib = KLibLoader::self()->library(s_kpluginFactoryModule,
            QLibrary::ResolveAllSymbolsHint);
    QVERIFY(lib);
    QVERIFY(lib->isLoaded());
    QCOMPARE(QFileInfo(lib->fileName()).canonicalFilePath(),
             MODULE_PATH(s_kpluginFactoryModule));
}

void KLibLoaderTest::testLibrary_noexist()
{
    KLibrary *lib = KLibLoader::self()->library("idontexist");
    QVERIFY(!lib);
}

void KLibLoaderTest::testFindLibrary()
{
    const QString library = KLibLoader::findLibrary(s_kpluginFactoryModule);
    QVERIFY(!library.isEmpty());
    QCOMPARE(QFileInfo(library).canonicalFilePath(),
             MODULE_PATH(s_kpluginFactoryModule));
}

void KLibLoaderTest::testFindLibrary_noexist()
{
    const QString library = KLibLoader::findLibrary("idontexist");
    QVERIFY(library.isEmpty());
}
