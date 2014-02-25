# Try to find DocBook XML 4.x DTD.
# By default it will find version 4.2. A different version can be specified
# as parameter for find_package().
# Once done, it will define:
#
#  DocBookXML4_FOUND - system has the requested DocBook4 XML DTDs
#  DocBookXML4_DTD_VERSION - the version of requested DocBook4
#     XML DTD
#  DocBookXML4_DTD_DIR - the directory containing the definition of
#     the DocBook4 XML

# Copyright (c) 2010, 2014 Luigi Toscano, <luigi.toscano@tiscali.it>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.


if (NOT DocBookXML_FIND_VERSION)
     set(DocBookXML_FIND_VERSION "4.2")
endif ()

set (DocBookXML4_DTD_VERSION ${DocBookXML_FIND_VERSION}
     CACHE INTERNAL "Required version of DocBook4 XML DTDs")

function (locate_version version found_dir)

    set (DTD_PATH_LIST
        share/xml/docbook/schema/dtd/${version}
        share/xml/docbook/xml-dtd-${version}
        share/sgml/docbook/xml-dtd-${version}
        share/xml/docbook/${version}
    )

    find_path (searched_dir docbookx.dtd
        PATHS ${CMAKE_SYSTEM_PREFIX_PATH}
        PATH_SUFFIXES ${DTD_PATH_LIST}
    )

    if (NOT searched_dir)
        # hacks for systems that use the package version in the DTD dirs,
        # e.g. Fedora, OpenSolaris
        set (DTD_PATH_LIST)
        foreach (DTD_PREFIX_ITER ${CMAKE_SYSTEM_PREFIX_PATH})
            file(GLOB DTD_SUFFIX_ITER RELATIVE ${DTD_PREFIX_ITER}
                ${DTD_PREFIX_ITER}/share/sgml/docbook/xml-dtd-${version}-*
            )
            if (DTD_SUFFIX_ITER)
                list (APPEND DTD_PATH_LIST ${DTD_SUFFIX_ITER})
            endif ()
        endforeach ()

        find_path (searched_dir docbookx.dtd
            PATHS ${CMAKE_SYSTEM_PREFIX_PATH}
            PATH_SUFFIXES ${DTD_PATH_LIST}
        )
    endif ()
    set (${found_dir} ${searched_dir} PARENT_SCOPE)
endfunction()


locate_version (${DocBookXML4_DTD_VERSION} DocBookXML4_DTD_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args (DocBookXML4
    REQUIRED_VARS DocBookXML4_DTD_DIR DocBookXML4_DTD_VERSION
    FOUND_VAR DocBookXML4_FOUND)

#maintain backwards compatibility
# legacy version
locate_version ("4.2" DOCBOOKXML_CURRENTDTD_DIR)
if (DOCBOOKXML_CURRENTDTD_DIR)
    set(DOCBOOKXML_FOUND "TRUE")
    set(DOCBOOKXML_CURRENTDTD_VERSION "4.2")
endif ()

mark_as_advanced (DocBookXML4_DTD_DIR DocBookXML4_DTD_VERSION)
