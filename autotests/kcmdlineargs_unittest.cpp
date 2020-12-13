/* This file is part of the KDE project

   Copyright 1999      Waldo Bastian <bastian@kde.org>
   Copyright 2014      Albert Astals Cid <aacid@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <QTest>

#include <kcmdlineargs.h>

class KCmdLineArgsTest : public QObject
{
public Q_SLOTS:
    void testmakeURL()
    {
        // Check how KCmdLineArgs::url() works
        QUrl u = KCmdLineArgs::makeURL(QByteArray("/tmp"));
        QCOMPARE(u.toLocalFile(), QLatin1String("/tmp"));
        u = KCmdLineArgs::makeURL(QByteArray("foo"));
        QCOMPARE(u.toLocalFile(), QDir::currentPath() + QLatin1String("/foo"));
        u = KCmdLineArgs::makeURL(QByteArray("http://www.kde.org"));
        QCOMPARE(u.toString(), QLatin1String("http://www.kde.org"));

        QFile file(QLatin1String("a:b"));
#ifndef Q_OS_WIN
        bool ok = file.open(QIODevice::WriteOnly);
        Q_UNUSED(ok) // silence warnings
        QVERIFY(ok);
#endif
        u = KCmdLineArgs::makeURL(QByteArray("a:b"));
        QVERIFY(u.isLocalFile());
        QVERIFY(u.toLocalFile().endsWith(QLatin1String("a:b")));
    }
};

QTEST_MAIN(KCmdLineArgsTest)
