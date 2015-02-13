/*
    Copyright 2010 John Layt <john@layt.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef KCALENDARSYSTEMCOPTICPRIVATE_H
#define KCALENDARSYSTEMCOPTICPRIVATE_H

#include "kcalendarsystemprivate_p.h"

class KCalendarSystemCopticPrivate : public KCalendarSystemPrivate
{
public:
    explicit KCalendarSystemCopticPrivate(KCalendarSystemCoptic *q);

    virtual ~KCalendarSystemCopticPrivate();

    // Virtual methods each calendar system must re-implement
    void loadDefaultEraList() Q_DECL_OVERRIDE;
    int monthsInYear(int year) const Q_DECL_OVERRIDE;
    int daysInMonth(int year, int month) const Q_DECL_OVERRIDE;
    int daysInYear(int year) const Q_DECL_OVERRIDE;
    bool isLeapYear(int year) const Q_DECL_OVERRIDE;
    bool hasLeapMonths() const Q_DECL_OVERRIDE;
    bool hasYearZero() const Q_DECL_OVERRIDE;
    int maxMonthsInYear() const Q_DECL_OVERRIDE;
    int earliestValidYear() const Q_DECL_OVERRIDE;
    int latestValidYear() const Q_DECL_OVERRIDE;
    QString monthName(int month, int year, KLocale::DateTimeComponentFormat format, bool possessive) const Q_DECL_OVERRIDE;
    QString weekDayName(int weekDay, KLocale::DateTimeComponentFormat format) const Q_DECL_OVERRIDE;
};

#endif // KCALENDARSYSTEMCOPTICPRIVATE_H
