remove_definitions(-DQT_NO_CAST_FROM_ASCII)

set(kprintutils_LIB_SRCS
    kprintpreview.cpp
)

set(kprintutils_LIB_SRCS
  ${kprintutils_LIB_SRCS}
  kdeprintdialog.cpp
)

add_library(KF5PrintUtils ${kprintutils_LIB_SRCS})
generate_export_header(KF5PrintUtils BASE_NAME KPrintUtils)
add_library(KF5::PrintUtils ALIAS KF5PrintUtils)

target_include_directories(KF5PrintUtils INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/KPrintUtils>")

target_link_libraries(KF5PrintUtils
  PUBLIC
    Qt5::Widgets
    Qt5::PrintSupport
  PRIVATE
    KF5::Parts
    KF5::I18n
)
set_target_properties(KF5PrintUtils PROPERTIES
                      VERSION ${KPRINTUTILS_VERSION_STRING}
                      SOVERSION ${KPRINTUTILS_SOVERSION}
                      EXPORT_NAME PrintUtils
)

ecm_generate_headers(KPrintUtils_HEADERS
  HEADER_NAMES
  KPrintPreview
  KDEPrintDialog

  REQUIRED_HEADERS KPrintUtils_HEADERS
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kprintutils_export.h
  kdeprintdialog.h
  ${KPrintUtils_HEADERS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/KPrintUtils COMPONENT Devel
)

install(TARGETS KF5PrintUtils EXPORT KF5PrintUtilsTargets ${INSTALL_TARGETS_DEFAULT_ARGS})



include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KPrintUtils LIB_NAME KF5PrintUtils DEPS "widgets printsupport" FILENAME_VAR PRI_FILENAME)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
