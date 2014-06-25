#!/bin/sh

# Invoke the extractrc script on all .ui, .rc, and .kcfg files in the sources.
# The results are stored in a pseudo .cpp file to be picked up by xgettext.
lst=`find . -name \*.rc -o -name \*.ui -o -name \*.kcfg`
if [ -n "$lst" ] ; then
    $EXTRACTRC $lst >> rc.cpp
fi

# The extracted strings will be used by KColorTable::readNamedColor(),
# see src/kdeui/kcolordialog.cpp. Here we extract from rgb.txt
# the same subset of color names as in KColorTable::readNamedColor(),
# i.e. we throw away
#  - color names containing spaces,
#  - color names containing "gray" or "grey" as substrings.
#
# This filtering was introduced long before KDE Frameworks 5.0,
# probably for early versions of KDE SC 4.x or earlier. The rationale
# for this filtering and the reason why we keep it is the following:
#   1. Most colors names containing spaces in rgb.txt have also their
#        versions written without spaces but with exactly same RGB
#        components.
#   2. There are too many "gray/grey" colors, keeping them in the list
#        would add only mess.
awk '$1 ~ "[0-9]+" && $4 !~ "gr[ae]y" && NF == 4 { print $4 }' kdeui/rgb.txt | LC_ALL=C sort -u | while read i ; do
    echo "i18nc(\"color\", \"$i\");" | sed -e "s#!# #g" >> rc.cpp
done

# Run xgettext to extract strings from all source files.
$XGETTEXT `find . -name \*.cpp -o -name \*.h -o -name \*.qml` kdecore/TIMEZONES -o $podir/kdelibs4support.pot
