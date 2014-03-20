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

#include <QtTest>

QTEST_MAIN(KLibLoaderTest)

#include <klibloader.h>
//#include <kstandarddirs.h>
#include <QtCore/QDir>
#include <QDebug>

void KLibLoaderTest::initTestCase()
{
    const QString libdir = QDir::currentPath();
    qputenv("LD_LIBRARY_PATH", qgetenv("LD_LIBRARY_PATH") + ":" + QFile::encodeName(libdir));
    //KGlobal::dirs()->addResourceDir( "module", libdir );
    //qDebug( "initTestCase: added %s to 'module' resource", qPrintable(libdir) );
}

void KLibLoaderTest::testNonWorking()
{
    KPluginFactory *factory = KPluginLoader("idontexist2").factory();
    QVERIFY(!factory);
}

// We need a module to dlopen, which uses a standard factory (e.g. not an ioslave)
static const char s_kgenericFactoryModule[] = "klibloadertestmodule";

void KLibLoaderTest::testFindLibrary()
{
    const QString library = KLibLoader::findLibrary(s_kgenericFactoryModule);
    QVERIFY(!library.isEmpty());
    const QString libraryPath = QFileInfo(library).canonicalFilePath();
#if defined(Q_OS_WIN) || defined(Q_OS_CYGWIN)
    const QString expectedPath = QFileInfo(QDir::currentPath()  + '/' + s_kgenericFactoryModule + ".dll").canonicalFilePath();
#else
    const QString expectedPath = QFileInfo(QDir::currentPath()  + '/' + s_kgenericFactoryModule + ".so").canonicalFilePath();
#endif
    QCOMPARE(library, expectedPath);
}

static const char s_kpluginFactoryModule[] = "klibloadertestmodule4";

// new loader, old plugin
void KLibLoaderTest::testWorking_KPluginLoader_KGenericFactory()
{
    KPluginLoader loader(s_kgenericFactoryModule);
    KPluginFactory *factory = loader.factory();
    if (!factory) {
        qWarning() << "error=" << loader.errorString();
        QVERIFY(factory);
    } else {
        QObject *obj = factory->create<QObject>();
        if (!obj) {
            qWarning() << "Error creating object";
        }
        QVERIFY(obj);
        delete obj;
    }
}

// new loader, new plugin
void KLibLoaderTest::testWorking_KPluginLoader_KPluginFactory()
{
    KPluginLoader loader(s_kpluginFactoryModule);
    KPluginFactory *factory = loader.factory();
    if (!factory) {
        qWarning() << "error=" << loader.errorString();
        QVERIFY(factory);
    } else {
        QObject *obj = factory->create<QObject>();
        if (!obj) {
            qWarning() << "Error creating object";
        }
        QVERIFY(obj);
        delete obj;
    }
}
