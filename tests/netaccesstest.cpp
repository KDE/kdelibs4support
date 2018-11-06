/*
 *  Copyright (C) 2004 David Faure   <faure@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include <QApplication>
#include <QUrl>
#include <QDebug>
#include <kio/netaccess.h>
#include <kio/job.h>
#include <QFile>
#include <QTemporaryFile>

int main(int argc, char **argv)
{
    QApplication::setApplicationName("netaccesstest");
    QApplication app(argc, argv);
    QUrl srcURL("http://download.kde.org/README");
    QTemporaryFile tmpFile;

    if (!tmpFile.open()) {
        qCritical() << "temporary file creation failed";
        return 1;
    }

    QUrl tmpURL(QUrl::fromLocalFile(tmpFile.fileName()));

    for (uint i = 0; i < 4; ++i) {
        qDebug() << "file_copy";
        KIO::Job *job = KIO::file_copy(srcURL, tmpURL, -1, KIO::Overwrite);
        if (!KIO::NetAccess::synchronousRun(job, nullptr)) {
            qCritical() << "file_copy failed: " << KIO::NetAccess::lastErrorString();
            return 1;
        } else {
            QFile f(tmpURL.path());
            if (!f.open(QIODevice::ReadOnly)) {
                qCritical() << "Cannot open: " << f.fileName() << ". The error was: " << f.errorString();
                return 2;
            }
        }
    }

    return 0;
}

