
include(ECMMarkAsTest)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Concurrent)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../..")

macro(KDE4SUPPORT_EXECUTABLE_TESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} KF5::KDE4Support Qt5::Core Qt5::Widgets KF5::ConfigCore)
        ecm_mark_as_test(${_testname})
    endforeach()
endmacro()

macro(KDE4SUPPORT_UNIT_TESTS)
  foreach(_testname ${ARGN})
    add_executable(${_testname} ${_testname}.cpp)
    add_test(kde4support-${_testname} ${_testname})
    target_link_libraries(${_testname}
       KF5::KDE4Support
       Qt5::DBus
       Qt5::Widgets
       Qt5::Test
       Qt5::Network
       Qt5::Concurrent
       KF5::CoreAddons
       KF5::I18n
       KF5::Crash
       KF5::KIOCore
       KF5::KIOWidgets
    )
    ecm_mark_as_test(${_testname})
  endforeach(_testname)
endmacro(KDE4SUPPORT_UNIT_TESTS)

kde4support_unit_tests(
  globalcleanuptest
  kaccelgentest
  kcalendartest
  kdatetimetest
  klocaletimeformattest
  ktimezonestest
  k4timezonewidget_unittest
  kdebugtest
  kbuttongrouptest
  kdialog_unittest
  kfadewidgeteffecttest
  kconfigafterkglobaltest1
  kconfigafterkglobaltest2
  kdebug_unittest
  kdebug_qcoreapptest
  kmd5benchmark
  ktabwidget_unittest
  ktempdirtest
  ktemporaryfiletest
  kuniqueapptest
  kurlmimetest
  klibloadertest
  ksavefiletest
  kconfigcompattest
  kglobalstatictest
  kmimetypetest
  kmimetype_nomimetypes
  kstandarddirstest
  kasciitest
  kglobaltest
  klockfiletest
  kurltest
  kfilemetainfotest
  kglobalsettingstest
  ksharedptrtest
  naturalcomparetest
)

kde4support_executable_tests(
  klockfile_testlock         # Helper for klockfiletest
  kglobalsettingsclient      # Helper for kglobalsettingstest
)

########### module for klibloadertest ###############

if(NOT KDE_NO_DEPRECATED)
  # this exists to check that code using the kde 3 macros still build,
  # even though the module cannot be loaded
  set(klibloadertestmodule3_PART_SRCS klibloadertest3_module.cpp )

  add_library(klibloadertestmodule3 MODULE ${klibloadertestmodule3_PART_SRCS})
  set_target_properties(klibloadertestmodule3 PROPERTIES PREFIX "") # remove lib prefix - missing from cmake

  target_link_libraries(klibloadertestmodule3 KF5::KDE4Support Qt5::Test KF5::I18n KF5::Service KF5::CoreAddons)

  set_target_properties(klibloadertestmodule3 PROPERTIES SKIP_BUILD_RPATH FALSE BUILD_WITH_INSTALL_RPATH FALSE)
endif()

########### module for klibloadertest5 ###############

set(klibloadertestmodule5_PART_SRCS klibloadertest5_module.cpp )

add_library(klibloadertestmodule5 MODULE ${klibloadertestmodule5_PART_SRCS})
ecm_mark_as_test(klibloadertestmodule5)
set_target_properties(klibloadertestmodule5 PROPERTIES PREFIX "") # remove lib prefix - missing from cmake

target_link_libraries(klibloadertestmodule5 KF5::KDE4Support Qt5::Test KF5::I18n KF5::Service KF5::CoreAddons)

set_target_properties(klibloadertestmodule5 PROPERTIES SKIP_BUILD_RPATH FALSE BUILD_WITH_INSTALL_RPATH FALSE)

########### klocaletest ###############

# compile into the test since it's not exported
set(klocaletest_SRCS klocaletest.cpp ../src/kdecore/kdayperiod.cpp)
add_executable(klocaletest ${klocaletest_SRCS})
add_test(kde4support-klocaletest klocaletest)
ecm_mark_as_test(klocaletest)
target_link_libraries(klocaletest Qt5::Test Qt5::DBus KF5::I18n KF5::KDE4Support KF5::ConfigCore)

########### kdatetimeformattertest ###############

# compile KDateTimeFormatter and KDayPeriod into the test since it's not exported
set(kdatetimeformattertest_SRCS kdatetimeformattertest.cpp ../src/kdecore/kdatetimeformatter.cpp ../src/kdecore/kdayperiod.cpp)
add_executable(kdatetimeformattertest ${kdatetimeformattertest_SRCS})
add_test(kde4support-kdatetimeformattertest kdatetimeformattertest)
ecm_mark_as_test(kdatetimeformattertest)
target_link_libraries(kdatetimeformattertest Qt5::Test KF5::I18n KF5::KDE4Support KF5::ConfigCore)


