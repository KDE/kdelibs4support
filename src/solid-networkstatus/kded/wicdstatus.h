/*  This file is part of the KDE project

    Copyright (c) 2011 Lamarque V. Souza <lamarque@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NETWORKSTATUS_WICDSTATUS_H
#define NETWORKSTATUS_WICDSTATUS_H

#include "systemstatusinterface.h"

#include <QDBusInterface>

class WicdStatus : public SystemStatusInterface
{
    Q_OBJECT
public:
    WicdStatus( QObject *parent = nullptr );

    Solid::Networking::Status status() const override;
    bool isSupported() const override;
    QString serviceName() const override;

private Q_SLOTS:
    void wicdStateChanged();

private:
    class Wicd {
    public:
        enum ConnectionStatus {
            NOT_CONNECTED = 0,
            CONNECTING = 1,
            WIRELESS = 2,
            WIRED = 3,
            SUSPENDED = 4,
            Unknown = 16
        };
    };

    mutable QDBusInterface m_wicd;
    Solid::Networking::Status cachedState;
};

#endif

