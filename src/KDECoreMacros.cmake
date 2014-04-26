
# Copyright (c) 2006-2009 Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2006, 2007, Laurent Montel, <montel@kde.org>
# Copyright (c) 2007 Matthias Kretz <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


macro (KDE4_ADD_KCFG_FILES _sources )
   kconfig_add_kcfg_files( ${_sources} ${ARGN})
endmacro (KDE4_ADD_KCFG_FILES)


# TODO: This belongs whereever the rest of the kde translation system belongs
# Or in ECM, as it appears to be KDE independent, although it's odd to have such magic for TS files.
macro(KDE4_INSTALL_TS_FILES _lang _sdir)
   ki18n_install_ts_files(${_lang} ${_sdir})
endmacro(KDE4_INSTALL_TS_FILES)

