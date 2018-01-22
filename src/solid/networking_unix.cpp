/*
    Copyright 2006-2007 Will Stephenson <wstephenson@kde.org>
    Copyright 2006-2007 Kevin Ottens <ervin@kde.org>

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

//#include <KDebug>

#include "networking.h"
#include "networking_p.h"

#include "org_kde_solid_networking_client.h"

Solid::NetworkingPrivate::NetworkingPrivate()
    : netStatus(Solid::Networking::Unknown),
      connectPolicy(Solid::Networking::Managed),
      disconnectPolicy(Solid::Networking::Managed),
      iface(nullptr)
{
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher("org.kde.kded5", QDBusConnection::sessionBus(),
            QDBusServiceWatcher::WatchForOwnerChange, this);
    connect(watcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
            this, SLOT(serviceOwnerChanged(QString,QString,QString)));

    initialize();
}

Solid::NetworkingPrivate::~NetworkingPrivate()
{
    delete iface;
}

void Solid::NetworkingPrivate::initialize()
{
    delete iface;
    iface  = new OrgKdeSolidNetworkingClientInterface("org.kde.kded5",
            "/modules/networkstatus",
            QDBusConnection::sessionBus(),
            this);

    //connect( iface, SIGNAL(statusChanged(uint)), globalNetworkManager, SIGNAL(statusChanged(Networking::Status)) );
    connect(iface, SIGNAL(statusChanged(uint)), this, SLOT(serviceStatusChanged(uint)));

    QDBusReply<uint> reply = iface->status();
    if (reply.isValid()) {
        netStatus = (Solid::Networking::Status)reply.value();
    } else {
        netStatus = Solid::Networking::Unknown;
    }
}

void Solid::NetworkingPrivate::serviceStatusChanged(uint status)
{
//    qDebug() ;
    netStatus = (Solid::Networking::Status)status;
    switch (netStatus) {
    case Solid::Networking::Unknown:
        break;
    case Solid::Networking::Unconnected:
    case Solid::Networking::Disconnecting:
    case Solid::Networking::Connecting:
        if (disconnectPolicy == Solid::Networking::Managed) {
            emit Solid::Networking::notifier()->shouldDisconnect();
        } else if (disconnectPolicy == Solid::Networking::OnNextStatusChange) {
            setDisconnectPolicy(Solid::Networking::Manual);
            emit Solid::Networking::notifier()->shouldDisconnect();
        }
        break;
    case Solid::Networking::Connected:
        if (disconnectPolicy == Solid::Networking::Managed) {
            emit Solid::Networking::notifier()->shouldConnect();
        } else if (disconnectPolicy == Solid::Networking::OnNextStatusChange) {
            setConnectPolicy(Solid::Networking::Manual);
            emit Solid::Networking::notifier()->shouldConnect();
        }
        break;
//      default:
//        qDebug() <<  "Unrecognised status code!";
    }
    emit Solid::Networking::notifier()->statusChanged(netStatus);
}

void Solid::NetworkingPrivate::serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner)
{
    Q_UNUSED(name)
    Q_UNUSED(oldOwner)
    if (newOwner.isEmpty()) {
        // kded quit on us
        netStatus = Solid::Networking::Unknown;
        emit Solid::Networking::notifier()->statusChanged(netStatus);

    } else {
        // kded was replaced or started
        initialize();
        emit Solid::Networking::notifier()->statusChanged(netStatus);
        serviceStatusChanged(netStatus);
    }
}
