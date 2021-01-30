/* This file is part of the KDE libraries
    Copyright (c) 1999 Preston Brown <pbrown@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "kuniqueapplication.h"
#include "kuniqueapplication_p.h"
#include <kmainwindow.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <QFile>
#include <QList>
#include <QTimer>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusError>
#include <QDBusReply>

#include <kcmdlineargs.h>
#include <klocalizedstring.h>
#include <k4aboutdata.h>
#include <kconfiggroup.h>
#include <kwindowsystem.h>

#include <config-kdelibs4support.h>
#if HAVE_X11
#include <kstartupinfo.h>
#include <netwm.h>
#include <X11/Xlib.h>
#include <fixx11h.h>
#endif

/* I don't know why, but I end up with complaints about
   a forward-declaration of QWidget in the activeWidow()->show
   call below on Qt/Mac if I don't include this here... */
#include <QWidget>

#include <kconfig.h>
#include "kdebug.h"

#if defined(Q_OS_DARWIN) || defined (Q_OS_MAC)
#include <kkernel_mac.h>
#endif

bool KUniqueApplication::Private::s_startOwnInstance = false;
bool KUniqueApplication::Private::s_multipleInstances = false;
#ifdef Q_OS_WIN
/* private helpers from kapplication_win.cpp */
#ifndef _WIN32_WCE
void KApplication_activateWindowForProcess(const QString &executableName);
#endif
#endif

void
KUniqueApplication::addCmdLineOptions()
{
    KCmdLineOptions kunique_options;
    kunique_options.add("nofork", ki18n("Do not run in the background."));
#ifdef Q_OS_MAC
    kunique_options.add("psn", ki18n("Internally added if launched from Finder"));
#endif
    KCmdLineArgs::addCmdLineOptions(kunique_options, KLocalizedString(), "kuniqueapp", "kde");
}

static QDBusConnectionInterface *tryToInitDBusConnection()
{
    // Check the D-Bus connection health
    QDBusConnectionInterface *dbusService = nullptr;
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if (!sessionBus.isConnected() || !(dbusService = sessionBus.interface())) {
        kError() << "KUniqueApplication: Cannot find the D-Bus session server: " << sessionBus.lastError().message() << endl;
        ::exit(255);
    }
    return dbusService;
}

bool KUniqueApplication::start()
{
    return start(StartFlags());
}

bool
KUniqueApplication::start(StartFlags flags)
{
    extern bool s_kuniqueapplication_startCalled;
    if (s_kuniqueapplication_startCalled) {
        return true;
    }
    s_kuniqueapplication_startCalled = true;

    addCmdLineOptions(); // Make sure to add cmd line options
#if defined(Q_OS_WIN) || defined(Q_OS_MACX)
    Private::s_startOwnInstance = true;
#else
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs("kuniqueapp");
    Private::s_startOwnInstance = !args->isSet("fork");
#endif

    QString appName = KCmdLineArgs::aboutData()->appName();
    const QStringList parts = KCmdLineArgs::aboutData()->organizationDomain().split(QLatin1Char('.'), QString::SkipEmptyParts);
    if (parts.isEmpty()) {
        appName.prepend(QLatin1String("local."));
    } else
        foreach (const QString &s, parts) {
            appName.prepend(QLatin1Char('.'));
            appName.prepend(s);
        }

    bool forceNewProcess = Private::s_multipleInstances || flags & NonUniqueInstance;

    if (Private::s_startOwnInstance) {
#if defined(Q_OS_DARWIN) || defined (Q_OS_MAC)
        mac_initialize_dbus();
#endif

        QDBusConnectionInterface *dbusService = tryToInitDBusConnection();

        QString pid = QString::number(getpid());
        if (forceNewProcess) {
            appName = appName + '-' + pid;
        }

        // Check to make sure that we're actually able to register with the D-Bus session
        // server.
        bool registered = dbusService->registerService(appName) == QDBusConnectionInterface::ServiceRegistered;
        if (!registered) {
            kError() << "KUniqueApplication: Can't setup D-Bus service. Probably already running."
                     << endl;
#if defined(Q_OS_WIN) && !defined(_WIN32_WCE)
            KApplication_activateWindowForProcess(KCmdLineArgs::aboutData()->appName());
#endif
            ::exit(255);
        }

        // We'll call newInstance in the constructor. Do nothing here.
        return true;

#if defined(Q_OS_DARWIN) || defined (Q_OS_MAC)
    } else {
        mac_fork_and_reexec_self();
#endif

    }

#ifndef Q_OS_WIN
    int fd[2];
    signed char result;
    // We use result to talk between child and parent. It can be
    // 0: App already running please call newInstance on it
    // 1: App was not running, child will call newInstance on itself
    // -1: Error, Can't start DBus service
    if (0 > pipe(fd)) {
        kError() << "KUniqueApplication: pipe() failed!" << endl;
        ::exit(255);
    }
    int fork_result = fork();
    switch (fork_result) {
    case -1:
        kError() << "KUniqueApplication: fork() failed!" << endl;
        ::exit(255);
        break;
    case 0: {
        // Child

        QDBusConnectionInterface *dbusService = tryToInitDBusConnection();
        ::close(fd[0]);
        if (forceNewProcess) {
            appName.append("-").append(QString::number(getpid()));
        }

        QDBusReply<QDBusConnectionInterface::RegisterServiceReply> reply =
            dbusService->registerService(appName);
        if (!reply.isValid()) {
            kError() << "KUniqueApplication: Can't setup D-Bus service." << endl;
            result = -1;
            ::write(fd[1], &result, 1);
            ::exit(255);
        }
        if (reply == QDBusConnectionInterface::ServiceNotRegistered) {
            // Already running. Ok.
            result = 0;
            ::write(fd[1], &result, 1);
            ::close(fd[1]);
            return false;
        }

#if HAVE_X11
        KStartupInfoId id;
        if (kapp != nullptr) { // KApplication constructor unsets the env. variable
            id.initId(kapp->startupId());
        } else {
            id = KStartupInfo::currentStartupIdEnv();
        }
        if (!id.isNull()) {
            // notice about pid change
            int screen = 0;
            xcb_connection_t *connection = xcb_connect(nullptr, &screen);
            if (connection != nullptr) { // use extra X connection
                KStartupInfoData data;
                data.addPid(getpid());
                KStartupInfo::sendChangeXcb(connection, screen, id, data);
                xcb_disconnect(connection);
            }
        }
#endif
    }
    result = 1;
    ::write(fd[1], &result, 1);
    ::close(fd[1]);
    Private::s_startOwnInstance = true;
    return true; // Finished.
    default:
        // Parent

        if (forceNewProcess) {
            appName.append("-").append(QString::number(fork_result));
        }
        ::close(fd[1]);

        Q_FOREVER {
        int n = ::read(fd[0], &result, 1);
            if (n == 1)
            {
                break;
            }
            if (n == 0)
            {
                kError() << "KUniqueApplication: Pipe closed unexpectedly." << endl;
                ::exit(255);
            }
            if (errno != EINTR)
            {
                kError() << "KUniqueApplication: Error reading from pipe." << endl;
                ::exit(255);
            }
        }
        ::close(fd[0]);

        if (result != 0) {
            // Only -1 is actually an error
            ::exit(result == -1 ? -1 : 0);
        }

#endif
        QDBusConnectionInterface *dbusService = tryToInitDBusConnection();
        if (!dbusService->isServiceRegistered(appName)) {
            kError() << "KUniqueApplication: Registering failed!" << endl;
        }

        QByteArray saved_args;
        QDataStream ds(&saved_args, QIODevice::WriteOnly);
        KCmdLineArgs::saveAppArgs(ds);

        QByteArray new_asn_id;
#if HAVE_X11
        KStartupInfoId id;
        if (kapp != nullptr) { // KApplication constructor unsets the env. variable
            id.initId(kapp->startupId());
        } else {
            id = KStartupInfo::currentStartupIdEnv();
        }
        if (!id.isNull()) {
            new_asn_id = id.id();
        }
#endif

        QDBusMessage msg = QDBusMessage::createMethodCall(appName, "/MainApplication", "org.kde.KUniqueApplication",
                           "newInstance");
        msg << new_asn_id << saved_args;
        QDBusReply<int> reply = QDBusConnection::sessionBus().call(msg, QDBus::Block, INT_MAX);

        if (!reply.isValid()) {
            QDBusError err = reply.error();
            kError() << "Communication problem with " << KCmdLineArgs::aboutData()->appName() << ", it probably crashed." << endl
                     << "Error message was: " << err.name() << ": \"" << err.message() << "\"" << endl;
            ::exit(255);
        }
#ifndef Q_OS_WIN
        ::exit(reply);
        break;
    }
#endif
    return false; // make insure++ happy
}

KUniqueApplication::KUniqueApplication(bool GUIenabled, bool configUnique)
    : KApplication(GUIenabled, Private::initHack(configUnique)),
      d(new Private(this))
{
    d->processingRequest = false;
    d->firstInstance = true;

    // the sanity checking happened in initHack
    new KUniqueApplicationAdaptor(this);

    if (Private::s_startOwnInstance)
        // Can't call newInstance directly from the constructor since it's virtual...
    {
        QTimer::singleShot(0, this, SLOT(_k_newInstanceNoFork()));
    }
}

KUniqueApplication::~KUniqueApplication()
{
    delete d;
}

// this gets called before even entering QApplication::QApplication()
KComponentData KUniqueApplication::Private::initHack(bool configUnique)
{
    KComponentData cData(KCmdLineArgs::aboutData());
    if (configUnique) {
        KConfigGroup cg(cData.config(), "KDE");
        s_multipleInstances = cg.readEntry("MultipleInstances", false);
    }
    if (!KUniqueApplication::start())
        // Already running
    {
        ::exit(0);
    }
    return cData;
}

void KUniqueApplication::Private::_k_newInstanceNoFork()
{
    q->newInstance();
    firstInstance = false;
}

bool KUniqueApplication::restoringSession()
{
    return d->firstInstance && isSessionRestored();
}

int KUniqueApplication::newInstance()
{
    if (!d->firstInstance) {
        QList<KMainWindow *> allWindows = KMainWindow::memberList();
        if (!allWindows.isEmpty()) {
            // This method is documented to only work for applications
            // with only one mainwindow.
            KMainWindow *mainWindow = allWindows.first();
            if (mainWindow) {
                mainWindow->show();
#if HAVE_X11
                mainWindow->setAttribute(Qt::WA_NativeWindow, true);
                // This is the line that handles window activation if necessary,
                // and what's important, it does it properly. If you reimplement newInstance(),
                // and don't call the inherited one, use this (but NOT when newInstance()
                // is called for the first time, like here).
                KStartupInfo::setNewStartupId(mainWindow->windowHandle(), startupId());
#endif
#ifdef Q_OS_WIN
                KWindowSystem::forceActiveWindow(mainWindow->winId());
#endif

            }
        }
    }
    return 0; // do nothing in default implementation
}

#ifndef KDELIBS4SUPPORT_NO_DEPRECATED
void KUniqueApplication::setHandleAutoStarted()
{
}
#endif

////

int KUniqueApplicationAdaptor::newInstance(const QByteArray &asn_id, const QByteArray &args)
{
    if (!asn_id.isEmpty()) {
        parent()->setStartupId(asn_id);
    }

    const int index = parent()->metaObject()->indexOfMethod("loadCommandLineOptionsForNewInstance");
    if (index != -1) {
        // This hook allows the application to set up KCmdLineArgs using addCmdLineOptions
        // before we load the app args. Normally not necessary, but needed by kontact
        // since it switches to other sets of options when called as e.g. kmail or korganizer
        QMetaObject::invokeMethod(parent(), "loadCommandLineOptionsForNewInstance");
    }

    QDataStream ds(args);
    KCmdLineArgs::loadAppArgs(ds);

    int ret = parent()->newInstance();
    // Must be done out of the newInstance code, in case it is overloaded
    parent()->d->firstInstance = false;
    return ret;
}

#include "moc_kuniqueapplication.cpp"
#include "moc_kuniqueapplication_p.cpp"
