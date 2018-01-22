# Windows notes

On Windows networkingservice and networkingclient test applications
require a running kded5 and <builddir>/bin/networkstatus.dll
installed in the directory where kded5 searches for kded modules
e.g <kded-install-root-lib-dir>/plugins/kf5/kded/.

Since the Windows implementation of solid uses QNetworkConfigurationManager
for determining the network connection state, the network status can not
be changed by the networkingservice test application. To see changes
to the network status in networkingclient, it is necessary to disable the
machine's network adapter.

