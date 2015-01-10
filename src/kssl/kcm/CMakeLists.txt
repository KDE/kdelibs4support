########### next target ###############

set(kcmssl_SRCS kcmssl.cpp cacertificatespage.cpp displaycertdialog.cpp)

#todo: port to include klocalizedstring.h
ki18n_wrap_ui(kcmssl_SRCS cacertificates.ui displaycert.ui)

add_library(kcm_ssl MODULE ${kcmssl_SRCS})
target_link_libraries(kcm_ssl
   Qt5::Network
   KF5::KIOCore
   KF5::KIOWidgets
   KF5::CoreAddons
   KF5::ConfigWidgets   # KCModule
   KF5::I18n
)
install(TARGETS kcm_ssl DESTINATION ${KDE_INSTALL_PLUGINDIR})

########### install files ###############
install(FILES kcm_ssl.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
