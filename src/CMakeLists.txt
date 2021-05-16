add_subdirectory(includes)
add_subdirectory(kdebugdialog)
add_subdirectory(solid-networkstatus)
add_subdirectory(l10n)
if(NOT HAVE_NEPOMUK)
   set(KIO_NO_NEPOMUK TRUE)
endif()

if(NOT Strigi_FOUND)
    set(KIO_NO_STRIGI TRUE)
else()
    include_directories(${STRIGI_INCLUDE_DIR} ${STRIGI_INCLUDE_DIRS} ${LIBSTREAMANALYZER_INCLUDE_DIRS})
    add_subdirectory( kio/dummyanalyzers )
    set(KIO_EXTRA_LIBS ${KIO_EXTRA_LIBS} ${STRIGI_STREAMANALYZER_LIBRARY} ${STRIGI_STREAMS_LIBRARY})
endif()

if(NOT KIO_NO_NEPOMUK)
    include_directories(
       ${SOPRANO_INCLUDE_DIR}
    )
endif()


find_package(OpenSSL)
set_package_properties(OpenSSL PROPERTIES DESCRIPTION "Support for secure network communications (SSL and TLS)"
                       URL "http://openssl.org"
                       TYPE RECOMMENDED
                       PURPOSE "KDE uses OpenSSL for the bulk of secure communications, including secure web browsing via HTTPS"
                      )
if(OPENSSL_FOUND)
   set(KSSL_HAVE_SSL 1)
   # on Windows include directories specified by target KDEWin and used by KF5KDELibs4Support
   # needs to be added first because they are used to override system include header.
   # This does not work if openssl include dir pointing to the standard include path would
   # be added first.
   if(NOT WIN32 OR NOT "${OPENSSL_INCLUDE_DIR}" STREQUAL "${INCLUDE_INSTALL_DIR}")
       include_directories(${OPENSSL_INCLUDE_DIR})
   endif()
endif()

add_subdirectory( kioslave )
add_subdirectory( kssl/kssl )
add_subdirectory( kssl/kcm )
add_subdirectory( kio/imageformats )

if(WIN32)
   set(libkdelibs4support_SRCS solid/networking_win.cpp
                               kdeui/kapplication_win.cpp)
else()
   set(libkdelibs4support_SRCS solid/networking_unix.cpp)
endif()


set(libkdelibs4support_SRCS
    ${libkdelibs4support_SRCS}
    kdecore/k4aboutdata.cpp
    kdecore/kcomponentdata.cpp
    kdecore/kcurrencycode.cpp
    kdecore/kdebug.cpp
    kdecore/kdebugdbusiface.cpp
    kdecore/kdeversion.cpp
    kdecore/kkernel_mac.cpp
    kdecore/kkernel_win.cpp
    kdecore/klibloader.cpp
    kdecore/klibrary.cpp
    kdecore/ktemporaryfile.cpp
    kdecore/ktempdir.cpp
    kdecore/kmd5.cpp
    kdecore/kmimetype.cpp
    kdecore/kmimetyperepository.cpp
    kdecore/ksavefile.cpp
    kdecore/ksocketfactory.cpp
    kdecore/k3socketdevice.cpp
    kdecore/k3bufferedsocket.cpp
    kdecore/k3clientsocketbase.cpp
    kdecore/k3resolver.cpp
    kdecore/k3resolvermanager.cpp
    kdecore/k3resolverworkerbase.cpp
    kdecore/k3resolverstandardworkers.cpp
    kdecore/k3reverseresolver.cpp
    kdecore/k3serversocket.cpp
    kdecore/k3socketaddress.cpp
    kdecore/k3socketbase.cpp
    kdecore/k3streamsocket.cpp
    kdecore/k3socketbuffer.cpp
    kdecore/netsupp.cpp
    kdecore/kascii.cpp
    kdecore/qtest_kde.cpp
    kdecore/kstandarddirs.cpp
    kdecore/kglobal.cpp
    kdecore/kcmdlineargs.cpp
    kdecore/kurl.cpp
    kdecore/kstringhandler_deprecated.cpp
    kdecore/kcalendarera.cpp
    kdecore/kcalendarsystem.cpp
    kdecore/kcalendarsystemcoptic.cpp
    kdecore/kcalendarsystemethiopian.cpp
    kdecore/kcalendarsystemgregorian.cpp
    kdecore/kcalendarsystemislamiccivil.cpp
    kdecore/kcalendarsystemhebrew.cpp
    kdecore/kcalendarsystemindiannational.cpp
    kdecore/kcalendarsystemjalali.cpp
    kdecore/kcalendarsystemjapanese.cpp
    kdecore/kcalendarsystemjulian.cpp
    kdecore/kcalendarsystemminguo.cpp
    kdecore/kcalendarsystemthai.cpp
    kdecore/kdatetime.cpp
    kdecore/kdatetimeformatter.cpp
    kdecore/kdatetimeparser.cpp
    kdecore/kdayperiod.cpp
    kdecore/klocalizeddate.cpp
    kdecore/ktimezone.cpp
    kdecore/ksystemtimezone.cpp
    kdecore/ktzfiletimezone.cpp
    kdecore/klocale.cpp
    kdecore/klocale_kde.cpp

    kdeui/k3icon.cpp
    kdeui/k4style.cpp
    kdeui/kaction.cpp
    kdeui/kapplication.cpp
    kdeui/kbuttongroup.cpp
    kdeui/kcolorchoosermode.cpp
    kdeui/kcolordialog.cpp
    kdeui/kcolorhelpers.cpp
    kdeui/kcolorvalueselector.cpp
    kdeui/kdeuiwidgetsproxystyle.cpp
    kdeui/kdialog.cpp
    kdeui/kdialogqueue.cpp
    kdeui/kfontcombobox.cpp
    kdeui/kfontdialog.cpp
    kdeui/khuesaturationselect.cpp
    kdeui/kpassivepopupmessagehandler.cpp
    kdeui/ksessionmanager.cpp
    kdeui/kfadewidgeteffect.cpp
    kdeui/kicon.cpp
    kdeui/kinputdialog.cpp
    kdeui/knumvalidator.cpp
    kdeui/karrowbutton.cpp
    kdeui/ksplashscreen.cpp
    kdeui/kstringvalidator.cpp
    kdeui/kdialogbuttonbox.cpp
    kdeui/kpixmapcache.cpp
    kdeui/kprogressdialog.cpp
    kdeui/kmenu.cpp
    kdeui/kmenubar.cpp
    kdeui/kshortcut.cpp
    kdeui/kstatusbar.cpp
    kdeui/ksystemtrayicon.cpp
    kdeui/ktabbar.cpp
    kdeui/ktabwidget.cpp
    kdeui/kuniqueapplication.cpp
    kdeui/khbox.cpp
    kdeui/kvbox.cpp
    kdeui/kmessagebox_queued.cpp
    kdeui/kmessageboxmessagehandler.cpp
    kdeui/kpushbutton.cpp
    kdeui/kundostack.cpp
    kdeui/krestrictedline.cpp
    kdeui/klistwidget.cpp
    kdeui/ktextbrowser.cpp
    kdeui/keditlistbox.cpp
    kdeui/knuminput.cpp
    kdeui/kglobalsettings.cpp
    kdeui/kdatetimewidget.cpp
    kdeui/kdatewidget.cpp
    kdeui/k4timezonewidget.cpp
    kdeui/kdeprintdialog.cpp
    kdeui/kprintpreview.cpp

    kio/kfilemetainfowidget.cpp
    kio/kdiskfreespace.cpp
    kio/metainfojob.cpp
    kio/kdevicelistmodel.cpp
    kio/kdevicelistitem.cpp
    kio/kfiledialog.cpp
    kssl/ksslx509v3.cpp
    kssl/kssl.cpp
    kssl/ksslutils.cpp
    kssl/ksslcertchain.cpp
    kssl/ksslcertificate.cpp
    kssl/kopenssl.cpp
    kio/kfileshare.cpp
    kio/kfileitemactionplugin.cpp
    kio/passworddialog.cpp
    kio/kdatatool.cpp
    kio/kfilemetadataconfigurationwidget.cpp
    kio/kfilemetadatawidget.cpp
    kio/kfilemetainfowidget.cpp
    kio/kmetaprops.cpp
    kio/knfotranslator.cpp
    kio/kfilemetainfo.cpp
    kio/kfilemetainfoitem.cpp
    kio/kcommentwidget.cpp
    kio/predicateproperties.cpp
    kio/kfilewriteplugin.cpp
#     kio/kpreviewprops.cpp
    kio/kscan.cpp
    kio/netaccess.cpp
    kio/kdirselectdialog.cpp
    kio/kimageio.cpp
    kio/kfiletreeview.cpp

    kparts/factory.cpp
    solid/powermanagement.cpp
    solid/networking.cpp
)

set_source_files_properties( solid/org.freedesktop.PowerManagement.xml
                             solid/org.freedesktop.PowerManagement.Inhibit.xml
                             solid/org.kde.Solid.PowerManagement.PolicyAgent.xml
                             PROPERTIES NO_NAMESPACE TRUE)

qt_add_dbus_interface(libkdelibs4support_SRCS solid/org.kde.Solid.Networking.Client.xml
                       org_kde_solid_networking_client)

qt_add_dbus_interfaces(libkdelibs4support_SRCS solid/org.freedesktop.PowerManagement.xml
                        solid/org.freedesktop.PowerManagement.Inhibit.xml
                        solid/org.kde.Solid.PowerManagement.PolicyAgent.xml)

if (NOT KIO_NO_NEPOMUK)
    set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS}
        kio/kfilemetadatareader.cpp
        kio/kfilemetadatareaderprocess.cpp
        kio/kfilemetadataprovider.cpp
    )
endif ()

if (X11_FOUND)
   set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS} kdeui/kxerrorhandler.cpp )
   list(APPEND libkdelibs4support_OPTIONAL_LIBS Qt5::X11Extras ${X11_LIBRARIES})
endif ()

if (WIN32)
    set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS}
                            kdecore/klockfile_win.cpp
                            kdecore/kde_file_win.cpp
                            kdecore/ktimezone_win.cpp
                            kdecore/klocale_win.cpp
                            )
else()
    set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS} kdecore/klockfile_unix.cpp)
endif ()

if (APPLE)
    set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS}
                            kdecore/klocale_mac.cpp
                            )
    set(libkdelibs4support_OPTIONAL_LIBS ${CARBON_LIBRARY})
endif()

if (UNIX)
    set(libkdelibs4support_SRCS ${libkdelibs4support_SRCS}
                            kdecore/klocale_unix.cpp
                            )
endif()

# Configure checks for io/
include(kdecore/ConfigureChecks.cmake)
configure_file(kdecore/config-io.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-io.h)

# Configure checks for date/
configure_file(kdecore/config-date.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-date.h)

# Configure checks for localization
configure_file(kdecore/config-localization.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-localization.h)


# Configure checks for network/ but also for netsupp.*
include(ConfigureChecks.cmake)
configure_file(config-network.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-network.h )

check_function_exists(mmap HAVE_MMAP)
check_symbol_exists(posix_madvise "sys/mman.h" HAVE_MADVISE)
configure_file(config-kdelibs4support.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kdelibs4support.h )
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/time.h HAVE_SYS_TIME_H)
configure_file(kdecore/config-klockfile.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-klockfile.h)
# we use this for the kstandarddirs unit tests
get_target_property(KCONFIG_COMPILER_LOCATION KF5::kconfig_compiler LOCATION)
# Generate config-kstandarddirs.h
configure_file(config-kstandarddirs.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kstandarddirs.h )

check_cxx_compiler_flag(-fvisibility=hidden __KDE_HAVE_GCC_VISIBILITY)
if (__KDE_HAVE_GCC_VISIBILITY)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif()
configure_file(kdemacros.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kdemacros.h )

add_library(KF5KDELibs4Support ${libkdelibs4support_SRCS})
generate_export_header(KF5KDELibs4Support BASE_NAME KDELibs4Support EXPORT_FILE_NAME kdelibs4support_export_internal.h)
add_library(KF5::KDELibs4Support ALIAS KF5KDELibs4Support)

target_include_directories(KF5KDELibs4Support PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../../..")

set(kdelibs4support_INCLUDES
  "${CMAKE_CURRENT_SOURCE_DIR}/kdecore"
  "${CMAKE_CURRENT_SOURCE_DIR}/kdeui"
  "${CMAKE_CURRENT_SOURCE_DIR}/kio"
  "${CMAKE_CURRENT_SOURCE_DIR}/kssl"
  "${CMAKE_CURRENT_BINARY_DIR}/kssl" # ksslconfig.h
  "${CMAKE_CURRENT_SOURCE_DIR}/kparts"
  "${CMAKE_CURRENT_SOURCE_DIR}/solid"
  "${CMAKE_CURRENT_BINARY_DIR}/.." # kdelibs4support_version.h
  "${CMAKE_SOURCE_DIR}"
)
target_include_directories(KF5KDELibs4Support PUBLIC "$<BUILD_INTERFACE:${kdelibs4support_INCLUDES}>")

target_include_directories(KF5KDELibs4Support INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support>" "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support/KDE>")

set(platformLinkLibraries)
if (APPLE)
    set(platformLinkLibraries "-framework CoreFoundation -framework Carbon -lresolv")
endif()
if (X11_FOUND)
    set(platformLinkLibraries ${XCB_XCB_LIBRARY})
endif()

target_link_libraries(KF5KDELibs4Support
PUBLIC
   Qt5::Widgets
   Qt5::DBus
   Qt5::PrintSupport
   KF5::CoreAddons
   KF5::Crash
   KF5::WidgetsAddons
   KF5::ConfigCore # KSharedConfig in kcomponentdata.h
   KF5::ConfigWidgets
   KF5::KIOCore
   KF5::KIOFileWidgets
   KF5::I18n
   KF5::Completion
   KF5::Notifications
   KF5::IconThemes
   KF5::WindowSystem
   KF5::GuiAddons
   KF5::UnitConversion
   KF5::TextWidgets
   KF5::Parts
PRIVATE
   KF5::UnitConversion
   KF5::GlobalAccel
   Qt5::Svg
   Qt5::Test # for qtest_kde.cpp
   ${KIO_EXTRA_LIBS}
   ${libkdelibs4support_OPTIONAL_LIBS}
   ${platformLinkLibraries}
)

if(WIN32)
    target_link_libraries(KF5KDELibs4Support PUBLIC kdewin)
    # required by moc to generate correct slot access code from solid/networking_q.h
    set_target_properties(KF5KDELibs4Support PROPERTIES COMPILE_DEFINITIONS "Q_OS_WIN")
endif()


if(NOT KIO_NO_NEPOMUK)
    target_link_libraries(KF5KDELibs4Support PRIVATE ${KDE4_NEPOMUK_LIBRARY} nepomukquery nepomukutils)
endif()

if(NOT KIO_NO_NEPOMUK)
    add_executable(kfilemetadatareader kio/kfilemetadatareaderprocess.cpp)
    target_link_libraries(kfilemetadatareader ${KDE4_NEPOMUK_LIBRARY} nepomukutils KF5::CoreAddons KF5::I18n KF5::KDELibs4Support)
    install(TARGETS kfilemetadatareader ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
endif()

set_target_properties(KF5KDELibs4Support PROPERTIES
    VERSION ${KDELIBS4SUPPORT_VERSION}
    SOVERSION ${KDELIBS4SUPPORT_SOVERSION}
    EXPORT_NAME KDELibs4Support
)


########## Designer plugin

kf5designerplugin_add_plugin(kf5deprecatedwidgets
    kdedeprecated.widgets
    kdedeprecated.qrc
    classpreviews.cpp
)

target_link_libraries(kf5deprecatedwidgets KF5::KDELibs4Support
                                    Qt5::Widgets
                                    Qt5::Designer
                                    KF5::ConfigCore
                                    KF5::Completion
                                    KF5::WidgetsAddons
                                    KF5::Auth
                                    )

install(TARGETS kf5deprecatedwidgets DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer)

##########

add_subdirectory(pics)

install(TARGETS KF5KDELibs4Support
        EXPORT KF5KDELibs4SupportTargets
        ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    kdecore/k4aboutdata.h
    kdecore/kcomponentdata.h
    kdecore/kcurrencycode.h
    kdecore/kdebug.h
    kdecore/kde_file.h
    kdecore/klibloader.h
    kdecore/klibrary.h
    kdecore/ktemporaryfile.h
    kdecore/ktempdir.h
    kdecore/kmd5.h
    kdecore/ksavefile.h
    kdecore/ksocketfactory.h
    kdecore/k3bufferedsocket.h
    kdecore/k3clientsocketbase.h
    kdecore/k3resolver.h
    kdecore/k3reverseresolver.h
    kdecore/k3serversocket.h
    kdecore/k3socketaddress.h
    kdecore/k3socketbase.h
    kdecore/k3socketdevice.h
    kdecore/k3streamsocket.h
    kdecore/kconfiggroup_kurl.h
    kdecore/kmimetype.h
    kdecore/kgenericfactory.h
    kdecore/kgenericfactory.tcc
    kdecore/ktypelist.h
    kdecore/kascii.h
    kdecore/qtest_kde.h
    kdecore/kstandarddirs.h
    kdecore/kglobal.h
    kdecore/kcmdlineargs.h
    kdecore/klockfile.h
    kdecore/kurl.h
    kdecore/ksharedptr.h
    kdecore/kstringhandler_deprecated.h
    kdecore/kcalendarsystem.h
    kdecore/kdatetime.h
    kdecore/klocalizeddate.h
    kdecore/ksystemtimezone.h
    kdecore/ktimezone.h
    kdecore/ktzfiletimezone.h
    kdecore/klocale.h
    kdecore/kdefakes.h

    kio/kdiskfreespace.h
    kio/kdatatool.h
    kio/kfilemetadataconfigurationwidget.h
    kio/kfilemetadatawidget.h
    kio/kfilemetainfoitem.h
    kio/kfilemetainfowidget.h
    kio/kfilemetainfo.h
    kio/predicateproperties.h
    kio/kfilewriteplugin.h
    kio/kfileshare.h
    kio/kfileitemactionplugin.h
    kio/kscan.h
    kio/kdirselectdialog.h
    kio/kfiledialog.h
    kio/kpropsdialog.h
    kio/kdevicelistmodel.h
    kio/kimageio.h
    kssl/ksslcertificate.h
    kssl/ksslx509v3.h
    kssl/kssl.h
    kssl/ksslutils.h
    kssl/ksslcertchain.h

    kdeui/k4style.h
    kdeui/kaccelgen.h
    kdeui/kaction.h
    kdeui/kapplication.h
    kdeui/kbuttongroup.h
    kdeui/kcolorchoosermode.h
    kdeui/kcolordialog.h
    kdeui/kcolorvalueselector.h
    kdeui/kdialog.h
    kdeui/khuesaturationselect.h
    kdeui/kpassivepopupmessagehandler.h
    kdeui/ksessionmanager.h
    kdeui/kfadewidgeteffect.h
    kdeui/kfontcombobox.h
    kdeui/kfontdialog.h
    kdeui/kicon.h
    kdeui/kinputdialog.h
    kdeui/knumvalidator.h
    kdeui/karrowbutton.h
    kdeui/ksplashscreen.h
    kdeui/ktabbar.h
    kdeui/ktabwidget.h
    kdeui/kdialogbuttonbox.h
    kdeui/kpixmapcache.h
    kdeui/kprogressdialog.h
    kdeui/kmenu.h
    kdeui/kmenubar.h
    kdeui/kshortcut.h
    kdeui/kstatusbar.h
    kdeui/kstringvalidator.h
    kdeui/ksystemtrayicon.h
    kdeui/kuniqueapplication.h
    kdeui/khbox.h
    kdeui/kvbox.h
    kdeui/kmessagebox_queued.h
    kdeui/kmessageboxmessagehandler.h
    kdeui/kpushbutton.h
    kdeui/kundostack.h
    kdeui/krestrictedline.h
    kdeui/klistwidget.h
    kdeui/ktextbrowser.h
    kdeui/keditlistbox.h
    kdeui/knuminput.h
    kdeui/kviewstatesaver.h
    kdeui/kxerrorhandler.h
    kdeui/kglobalsettings.h
    kdeui/kdatetimewidget.h
    kdeui/kdatewidget.h
    kdeui/k4timezonewidget.h
    kdeui/kdeprintdialog.h
    kdeui/kprintpreview.h
    kdecore/kdelibs4support_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/kdelibs4support_export_internal.h
    ${CMAKE_CURRENT_BINARY_DIR}/kdemacros.h
    kdecore/kdeversion.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support COMPONENT Devel
)

install(FILES
    solid/networking.h
    solid/powermanagement.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support/solid COMPONENT Devel
)

configure_file(kssl/ksslconfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/kssl/ksslconfig.h )
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/kssl/ksslconfig.h )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kssl/ksslconfig.h 
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support COMPONENT Devel)

install( FILES kdebug.areas kdebugrc DESTINATION ${KDE_INSTALL_CONFDIR} )

configure_file(kdecore/all_languages.desktop ${CMAKE_CURRENT_BINARY_DIR}/all_languages @ONLY)
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/all_languages DESTINATION ${KDE_INSTALL_LOCALEDIR} RENAME kf5_all_languages )


install( FILES kdeui/40.colors kdeui/Oxygen.colors kdeui/Web.colors kdeui/Royal.colors kdeui/Rainbow.colors
         DESTINATION ${KDE_INSTALL_CONFDIR}/colors)

if (NOT X11_FOUND)
    # install our copy of the rgb.txt named colors list on systems without X11
    install(FILES kdeui/rgb.txt DESTINATION ${KDE_INSTALL_DATADIR_KF5}/kdeui)
endif ()

# kio/*.h
install(FILES
    kio/metainfojob.h
    kio/passworddialog.h
    kio/netaccess.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support/kio COMPONENT Devel
)

# kparts/*.h
install(FILES
    kparts/genericfactory.h
    kparts/factory.h
    kparts/listingextension.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support/kparts COMPONENT Devel
)

# knewstuff3/*.h
install( FILES
    knewstuff3/downloaddialog.h
    knewstuff3/downloadmanager.h
    knewstuff3/downloadwidget.h
    knewstuff3/entry.h
    knewstuff3/knewstuffaction.h
    knewstuff3/knewstuffbutton.h
    knewstuff3/uploaddialog.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDELibs4Support/knewstuff3 COMPONENT Devel
)

install( FILES
    KDECoreMacros.cmake
    KDEUIMacros.cmake
    kde4uic.cmake
    DESTINATION ${CMAKECONFIG_INSTALL_DIR} COMPONENT Devel )

install( FILES
    kio/kscan.desktop
    kio/kdatatool.desktop
    kio/kfilewrite.desktop
    DESTINATION  ${KDE_INSTALL_KSERVICETYPES5DIR} )

# install the fileshareset and filesharelist scripts, they are not required on windows
if (UNIX)
   install(PROGRAMS kio/fileshareset DESTINATION ${KDE_INSTALL_LIBEXECDIR_KF5} )

   install(CODE "
     set(FILESHARESET_PATH \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_LIBEXECDIR_KF5}/fileshareset\")
        EXECUTE_PROCESS(COMMAND sh -c \"chown 0 '\${FILESHARESET_PATH}' && chmod u+s '\${FILESHARESET_PATH}'\")
   ")

   # write a cmake script file which creates the symlink
   file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/make_fileshare_symlink.cmake "exec_program(${CMAKE_COMMAND} ARGS -E create_symlink fileshareset \"\$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_LIBEXECDIR_KF5}/filesharelist\")\n")
   # and add it as post-install script to any of the installed targets, so it will be executed during "make install"
   set_target_properties(KF5KDELibs4Support PROPERTIES POST_INSTALL_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/make_fileshare_symlink.cmake)
endif ()

#solid stuff
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/solid/org.kde.Solid.Networking.Client.xml
        DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR}
        RENAME kf5_org.kde.Solid.Networking.Client.xml)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/solid/org.kde.Solid.PowerManagement.PolicyAgent.xml
        DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR}
        RENAME kf5_org.kde.Solid.PowerManagement.PolicyAgent.xml)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/solid/org.freedesktop.PowerManagement.xml
        DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR}
        RENAME kf5_org.freedesktop.PowerManagement.xml)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/solid/org.freedesktop.PowerManagement.Inhibit.xml
        DESTINATION ${KDE_INSTALL_DBUSINTERFACEDIR}
        RENAME kf5_org.freedesktop.PowerManagement.Inhibit.xml)

########## DocBookXML 4.2 compatibility

set (_legacy_docbookxml "4.2")
find_package(DocBookXML4 ${_legacy_docbookxml})
if (DocBookXML4_FOUND)
    set(DOCBOOKXML_FOUND "TRUE")
    set(DOCBOOKXML_CURRENTDTD_DIR ${DocBookXML4_DTD_DIR})
    set(DOCBOOKXML_CURRENTDTD_VERSION ${_legacy_docbookxml})
endif ()

mark_as_advanced (DocBookXML4_DTD_DIR DocBookXML4_DTD_VERSION)

include(uriencode)
set (_custom_dtd_kdex "customization/dtd/kdex.dtd")
# The path to KDocTools resources is set as absolute, allowing the
# installation of KDocTools and KDELibs4Support in different prefixes.
set (_KDocTools_customization_dir "${KDOCTOOLS_CUSTOMIZATION_DIR}")
kdelibs4support_encode_uri(DocBookXML4_DTD_DIR)
kdelibs4support_encode_uri(_KDocTools_customization_dir)
configure_file(${_custom_dtd_kdex}.cmake ${CMAKE_CURRENT_BINARY_DIR}/${_custom_dtd_kdex} )
# WARNING: this is due to severe limitations on the windows platform to keep the dtd relocateable
# see the .xsl.cmake files for a more verbose explanation below
if(NOT WIN32)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_custom_dtd_kdex}
        DESTINATION ${KDE_INSTALL_DATADIR_KF5}/kdoctools/customization/dtd )
else()
    get_filename_component(NORMALIZED_DTD_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/${KDE_INSTALL_DATADIR_KF5}/kdoctools/customization/dtd" ABSOLUTE)
    # file(RELATIVE_PATH ...) returns wrong values for paths containing /../ (it doesn't normalize the path)
    # To work around this we make sure the variable passed has all /../ elements
    # removed using get_filename_component(... ABSOLUTE)
    install(CODE
        "file(RELATIVE_PATH DocBookXML4_DTD_DIR \"${NORMALIZED_DTD_INSTALL_PATH}\" \"${DocBookXML4_DTD_DIR}\")
        file(RELATIVE_PATH _KDocTools_customization_dir \"${NORMALIZED_DTD_INSTALL_PATH}\" \"${_KDocTools_customization_dir}\")
        include(\"${CMAKE_SOURCE_DIR}/cmake/uriencode.cmake\")
        set(CMAKE_MODULE_PATH \"${CMAKE_MODULE_PATH}\")
        kdelibs4support_encode_uri(DocBookXML4_DTD_DIR)
        kdelibs4support_encode_uri(_KDocTools_customization_dir)
        configure_file(\"${CMAKE_CURRENT_SOURCE_DIR}/${_custom_dtd_kdex}.cmake\" \"${CMAKE_CURRENT_BINARY_DIR}/${_custom_dtd_kdex}.install\")"
    )
    get_filename_component(_custom_dtd_kdex_name ${_custom_dtd_kdex} NAME)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_custom_dtd_kdex}.install DESTINATION ${KDE_INSTALL_DATADIR_KF5}/kdoctools/customization/dtd RENAME ${_custom_dtd_kdex_name})
endif()

install(FILES "customization/catalog4.xml"
    DESTINATION ${KDE_INSTALL_DATADIR_KF5}/kdoctools/customization )

