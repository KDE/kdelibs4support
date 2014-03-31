#!/bin/bash
$XGETTEXT $(find -name "kcalendarsystem*.cpp") -o $podir/kdecalendarsystems5.pot
$XGETTEXT TIMEZONES -o $podir/timezones5.pot
