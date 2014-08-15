
include(ECMAddTests)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Concurrent)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../..")

macro(KDELIBS4SUPPORT_EXECUTABLE_TESTS)
    foreach(_testname ${ARGN})
        add_executable(${_testname} ${_testname}.cpp)
        target_link_libraries(${_testname} KF5::KDELibs4Support Qt5::Core Qt5::Widgets KF5::ConfigCore)
        ecm_mark_as_test(${_testname})
    endforeach()
endmacro()


ecm_add_tests( globalcleanuptest.cpp
        kaccelgentest.cpp
        kcalendartest.cpp
        kcmdlineargs_unittest.cpp
        kdatetimetest.cpp
        klocaletimeformattest.cpp
        ktimezonestest.cpp
        k4timezonewidget_unittest.cpp
        kdebugtest.cpp
        kbuttongrouptest.cpp
        kdialog_unittest.cpp
        kfadewidgeteffecttest.cpp
        kconfigafterkglobaltest1.cpp
        kconfigafterkglobaltest2.cpp
        kdebug_unittest.cpp
        kdebug_qcoreapptest.cpp
        kmd5benchmark.cpp
        ktabwidget_unittest.cpp
        ktempdirtest.cpp
        ktemporaryfiletest.cpp
        kuniqueapptest.cpp
        kurlmimetest.cpp
        klibloadertest.cpp
        ksavefiletest.cpp
        kconfigcompattest.cpp
        kglobalstatictest.cpp
        kmimetypetest.cpp
        kmimetype_nomimetypes.cpp
        kstandarddirstest.cpp
        kasciitest.cpp
        kglobaltest.cpp
        klockfiletest.cpp
        kurltest.cpp
        kfilemetainfotest.cpp
        kglobalsettingstest.cpp
        ksharedptrtest.cpp
        naturalcomparetest.cpp
    LINK_LIBRARIES
       KF5::KDELibs4Support
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


kdelibs4support_executable_tests(
  klockfile_testlock         # Helper for klockfiletest
  kglobalsettingsclient      # Helper for kglobalsettingstest
)

########### module for klibloadertest ###############

if(NOT KDELIBS4SUPPORT_NO_DEPRECATED)
  # this exists to check that code using the kde 3 macros still build,
  # even though the module cannot be loaded
  set(klibloadertestmodule3_PART_SRCS klibloadertest3_module.cpp )

  add_library(klibloadertestmodule3 MODULE ${klibloadertestmodule3_PART_SRCS})
  set_target_properties(klibloadertestmodule3 PROPERTIES PREFIX "") # remove lib prefix - missing from cmake

  target_link_libraries(klibloadertestmodule3 KF5::KDELibs4Support Qt5::Test KF5::I18n KF5::Service KF5::CoreAddons)

  set_target_properties(klibloadertestmodule3 PROPERTIES SKIP_BUILD_RPATH FALSE BUILD_WITH_INSTALL_RPATH FALSE)
endif()

########### module for klibloadertest5 ###############

set(klibloadertestmodule5_PART_SRCS klibloadertest5_module.cpp )

add_library(klibloadertestmodule5 MODULE ${klibloadertestmodule5_PART_SRCS})
ecm_mark_as_test(klibloadertestmodule5)
set_target_properties(klibloadertestmodule5 PROPERTIES PREFIX "") # remove lib prefix - missing from cmake

target_link_libraries(klibloadertestmodule5 KF5::KDELibs4Support Qt5::Test KF5::I18n KF5::Service KF5::CoreAddons)

set_target_properties(klibloadertestmodule5 PROPERTIES SKIP_BUILD_RPATH FALSE BUILD_WITH_INSTALL_RPATH FALSE)

########### klocaletest ###############

# compile into the test since it's not exported
ecm_add_test(klocaletest.cpp ../src/kdecore/kdayperiod.cpp
    TEST_NAME "kdelibs4support-klocaletest"
    LINK_LIBRARIES Qt5::Test Qt5::DBus KF5::I18n KF5::KDELibs4Support KF5::ConfigCore)

########### kdatetimeformattertest ###############

# compile KDateTimeFormatter and KDayPeriod into the test since it's not exported
ecm_add_test(kdatetimeformattertest.cpp ../src/kdecore/kdatetimeformatter.cpp ../src/kdecore/kdayperiod.cpp
    TEST_NAME "kdelibs4support-kdatetimeformattertest" LINK_LIBRARIES Qt5::Test KF5::I18n KF5::KDELibs4Support KF5::ConfigCore)


