
include(ECMMarkAsTest)

macro(KDELIBS4SUPPORT_EXAMPLES)
  foreach(_testname ${ARGN})
    add_executable(${_testname} ${_testname}.cpp)
    target_link_libraries(${_testname} KF5::KDELibs4Support Qt5::Test Qt5::Network Qt5::Widgets KF5::WidgetsAddons KF5::CoreAddons KF5::I18n KF5::XmlGui KF5::Completion KF5::Notifications KF5::IconThemes KF5::KIOFileWidgets)
    ecm_mark_as_test(${_testname})
  endforeach()
endmacro()

kdelibs4support_examples(
  kcolordlgtest
  kdebugtest_gui
  kdirselectdialogtest
  kdialogtest
  kmdcodectest
  kmessagetest
  kcmdlineargstest
  kdialogbuttonboxtest
  kmessageboxqueuedtest
  kprogressdialogtest
  khboxtest
  kshortcuttest
  kstatusbartest
  ksystemtraytest
  ktabwidgettest
  kfontdialogtest
  ktextbrowsertest
  keditlistboxtest
  knuminputtest
  kinputdialogtest
  kiopassdlgtest
  kmfitest
  kscantest
  netaccesstest
  kfstest
  kfiledialogtest
  kdatetimewidgettest
  kdatewidgettest
  kfiletreeviewtest
  kprintpreview_test
)

set(networkingclient_SRCS networkingclient.cpp )

qt5_wrap_ui(networkingclient_SRCS networkingclientview.ui)
qt5_add_dbus_interface(networkingclient_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../src/solid/org.kde.Solid.Networking.Client.xml clientinterface)
add_executable(networkingclient ${networkingclient_SRCS})

target_link_libraries(networkingclient KF5::KDELibs4Support Qt5::Widgets)
if(WIN32)
    target_link_libraries(networkingclient Qt5::Network)
endif()

if (X11_FOUND)
  find_package(Qt5X11Extras 5.2.0 REQUIRED NO_MODULE)
  kdelibs4support_examples( kxerrorhandlertest )

  target_link_libraries(kxerrorhandlertest ${X11_X11_LIB} Qt5::X11Extras)
endif ()

