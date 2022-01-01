cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.90.0") # handled by release scripts
set(KF_DEP_VERSION "5.90.0") # handled by release scripts
project(KDELibs4Support VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.90.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

include(GenerateExportHeader)
include(CMakeFindFrameworks)

set(REQUIRED_QT_VERSION 5.15.2)
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Network Widgets DBus Test Svg PrintSupport Designer)

find_package(KF5Completion ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Config ${KF_DEP_VERSION} REQUIRED)
find_package(KF5ConfigWidgets ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Crash ${KF_DEP_VERSION} REQUIRED)
find_package(KF5DesignerPlugin ${KF_DEP_VERSION} REQUIRED)
find_package(KF5GlobalAccel ${KF_DEP_VERSION} REQUIRED)
find_package(KF5DocTools ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Emoticons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5GuiAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5I18n ${KF_DEP_VERSION} REQUIRED)
find_package(KF5IconThemes ${KF_DEP_VERSION} REQUIRED)
find_package(KF5KIO ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Notifications ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Parts ${KF_DEP_VERSION} REQUIRED)
find_package(KF5Service ${KF_DEP_VERSION} REQUIRED)
find_package(KF5TextWidgets ${KF_DEP_VERSION} REQUIRED)
find_package(KF5UnitConversion ${KF_DEP_VERSION} REQUIRED)
find_package(KF5WidgetsAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KF5WindowSystem ${KF_DEP_VERSION} REQUIRED)
find_package(KF5XmlGui ${KF_DEP_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF_DEP_VERSION} REQUIRED)
find_package(KDED ${KF_DEP_VERSION} REQUIRED)

if(WIN32)
    find_package(KDEWin REQUIRED)
endif()

if (NOT APPLE AND NOT WIN32)
    find_package(X11)
endif()
set(HAVE_X11 ${X11_FOUND})
if (HAVE_X11)
  add_feature_info("X11 Session Management (libSM)" X11_SM_FOUND "Support for session management in KApplication")
  find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED X11Extras)
  find_package(XCB REQUIRED COMPONENTS XCB)
endif ()
if (APPLE)
    cmake_find_frameworks(CoreFoundation)
endif ()

include(ECMSetupVersion)
include(ECMMarkNonGuiExecutable)

ecm_setup_version(PROJECT VARIABLE_PREFIX KDELIBS4SUPPORT
                                 VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kdelibs4support_version.h"
                                 PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KDELibs4SupportConfigVersion.cmake"
                                 SOVERSION 5)

# Remove default warning version from KDEFrameworkCompilerSettings, too much noise
# instead avoid any deprecation warnings (to what Qt respects its own flag)
# Setting QT_DISABLE_DEPRECATED_BEFORE & KF_DISABLE_DEPRECATED_BEFORE_AND_AT as below
# defaults those DEPRECATED_WARNINGS_SINCE to the same version -> no warnings
remove_definitions(
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)
# remove definitions set by KDEFrameworkCompilerSettings which we fail to meet
remove_definitions(-DQT_NO_KEYWORDS)
remove_definitions(-DQT_NO_FOREACH)
remove_definitions(-DQT_NO_CAST_FROM_ASCII)
remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
remove_definitions(-DQT_USE_QSTRINGBUILDER)
# TODO: use QT_USE_QSTRINGBUILDER instead of deprecated & undocumented QT_USE_FAST_OPERATOR_PLUS
add_definitions(-DQT_USE_FAST_OPERATOR_PLUS)
# ideally we would support QT_NO_KEYWORDS, but at least we can do this
add_definitions(-DQT_NO_SIGNALS_SLOTS_KEYWORDS)

# QUrl
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0)

add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0)

# This one won't be needed when CMake 2.8.13 is depended on.
add_definitions(
    -DKDELIBS4SUPPORT_DEPRECATED= # if KDELIBS4SUPPORT_DEPRECATED is already defined the other deprecated macros won't be defined
    -DKDELIBS4SUPPORT_DEPRECATED_NO_EXPORT=KDELIBS4SUPPORT_NO_EXPORT
    -DKDELIBS4SUPPORT_DEPRECATED_EXPORT=KDELIBS4SUPPORT_EXPORT
)

add_definitions(-DTRANSLATION_DOMAIN=\"kdelibs4support\")

add_executable(kf5-config kde-config.cpp )
ecm_mark_nongui_executable(kf5-config)

target_link_libraries(kf5-config KF5::CoreAddons KF5::I18n KF5::ConfigCore KF5::KDELibs4Support)
target_include_directories(kf5-config PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/../..")

install(TARGETS kf5-config ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5KDELibs4Support")

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KDELibs4SupportConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KDELibs4SupportConfig.cmake"
    PATH_VARS KDE_INSTALL_DBUSINTERFACEDIR
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES  "${CMAKE_CURRENT_BINARY_DIR}/KF5KDELibs4SupportConfig.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/KF5KDELibs4SupportConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KF5KDELibs4SupportTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5KDELibs4SupportTargets.cmake
        NAMESPACE KF5::)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdelibs4support_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
        COMPONENT Devel)



#
# Compatibility file
#
set(CMAKECONFIG_COMPAT_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5KDE4Support")
if(BUILD_SHARED_LIBS)
    set(KDELibs4Support_LIB_TYPE SHARED)
else()
    set(KDELibs4Support_LIB_TYPE STATIC)
endif()
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KDE4SupportConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KDE4SupportConfig.cmake"
    INSTALL_DESTINATION "${CMAKECONFIG_COMPAT_INSTALL_DIR}"
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KDE4SupportConfigVersion.cmake"
    VERSION ${KF_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KF5KDE4SupportConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/KF5KDE4SupportConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_COMPAT_INSTALL_DIR}"
    COMPONENT Devel
)



ki18n_install(po)
kdoctools_install(po)
add_subdirectory(cmake)
add_subdirectory(data)
add_subdirectory(docs)
add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(autotests)
    add_subdirectory(tests)
endif ()


set (KDE_VERSION_MAJOR 4)
set (KDE_VERSION_MINOR 90)
set (KDE_VERSION_RELEASE 00)
set (KDE_VERSION "${KDE_VERSION_MAJOR}.${KDE_VERSION_MINOR}.${KDE_VERSION_RELEASE}" )
# For git master, use this:
set (KDE_VERSION_STRING "${KDE_VERSION} (KDE Frameworks >= 20121130)")
# For a stable release, use this:
#set (KDE_VERSION_STRING "${KDE_VERSION}")

include(CreateKDELibsDependenciesFile.cmake)
set(CMAKECONFIG_INSTALL_DIR ${KDE_INSTALL_LIBDIR}/cmake/KDELibs4)

# Used in configure_file() and install(EXPORT)
set(KDE4_TARGET_PREFIX KDE4__)

################# install files #################

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/KDELibsDependencies.cmake
               ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/kde4_cmake_uninstall.cmake.in
               ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/Win32.Manifest.in
         DESTINATION ${CMAKECONFIG_INSTALL_DIR} COMPONENT Devel)

# run a script before installing the exports files which deletes previously installed
# configuration specific export files KDELibs4(Library|Tools)Targets-<config>.cmake
# if the main exports file KDELibs4(Library|Tools)Targets.cmake has changed. This makes sure
# that this main file doesn't include older and different configuration specific exports files,
# which might have a different set of targets or targets with different names.
# The code for installing the exports files will soon go into a macro. Alex
install(CODE "set(EXPORT_FILES KDELibs4LibraryTargets.cmake)"
        CODE "set(EXPORT_INSTALL_DIR \"${CMAKECONFIG_INSTALL_DIR}\")"
        SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/check_installed_exports_file.cmake" )
install(EXPORT KF5KDELibs4SupportTargets
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        NAMESPACE ${KDE4_TARGET_PREFIX}
        FILE KF5KDELibs4LibraryTargets.cmake
        COMPONENT Devel)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)

if (HAVE_X11 AND NOT X11_SM_FOUND)
  message(FATAL_ERROR "The X11 Session Management library (libSM) is required when building on X11.")
endif()
