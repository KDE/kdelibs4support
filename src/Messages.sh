#!/bin/sh

# Invoke the extractrc script on all .ui, .rc, and .kcfg files in the sources.
# The results are stored in a pseudo .cpp file to be picked up by xgettext.
lst=`find . -name \*.rc -o -name \*.ui -o -name \*.kcfg`
if [ -n "$lst" ] ; then
    $EXTRACTRC $lst >> rc.cpp
fi

awk '$1 ~ "[0-9]+" && $4 !~ "gr[ae]y" { print $4 }' kdeui/rgb.txt | LC_ALL=C sort -u | while read i ; do
    echo "i18nc(\"color\", \"$i\");" | sed -e "s#!# #g" >> rc.cpp
done

# Run xgettext to extract strings from all source files.
$XGETTEXT `find . -name \*.cpp -o -name \*.h -o -name \*.qml` kdecore/TIMEZONES -o $podir/kdelibs4support.pot
