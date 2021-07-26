
project(kioslave-metainfo)

########### next target ###############

set(kio_metainfo_PART_SRCS metainfo.cpp )


add_library(kio_metainfo MODULE ${kio_metainfo_PART_SRCS})


target_link_libraries(kio_metainfo
   KF5::KDELibs4Support
   KF5::CoreAddons
   KF5::I18n
   KF5::KIOCore
)

set_target_properties(kio_metainfo PROPERTIES OUTPUT_NAME "metainfo")
install(TARGETS kio_metainfo  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/kio)
