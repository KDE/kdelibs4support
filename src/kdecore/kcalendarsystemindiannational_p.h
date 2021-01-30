/*
    Copyright 2009, 2010 John Layt <john@layt.net>

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

#ifndef KCALENDARSYSTEMINDIANNATIONAL_H
#define KCALENDARSYSTEMINDIANNATIONAL_H

#include "kcalendarsystem.h"

class KCalendarSystemIndianNationalPrivate;

/**
 * @internal
 * This is the Indian National / Civil / Saka calendar implementation.
 *
 * Note: This is is the purely civil arithmetic calendar, the religious
 * versions of the calendar will be implemented separately once
 * astronomical calendars are implemented.
 *
 * @b license GNU-LGPL v.2 or later
 *
 * @see KLocale,KCalendarSystem
 *
 * @author John Layt <john@layt.net>
 */
class KCalendarSystemIndianNational: public KCalendarSystem
{
public:
    KDELIBS4SUPPORT_DEPRECATED explicit KCalendarSystemIndianNational(const KSharedConfig::Ptr config, const KLocale *locale);
    ~KCalendarSystemIndianNational() override;

    QString calendarType() const override;
    KLocale::CalendarSystem calendarSystem() const override;

    QDate epoch() const override;
    QDate earliestValidDate() const override;
    QDate latestValidDate() const override;

    QString monthName(int month, int year, MonthNameFormat format = LongName) const override;
    QString monthName(const QDate &date, MonthNameFormat format = LongName) const override;

    QString weekDayName(int weekDay, WeekDayNameFormat format = LongDayName) const override;
    QString weekDayName(const QDate &date, WeekDayNameFormat format = LongDayName) const override;

    bool isLunar() const override;
    bool isLunisolar() const override;
    bool isSolar() const override;
    bool isProleptic() const override;

protected:
    bool julianDayToDate(qint64 jd, int &year, int &month, int &day) const override;
    bool dateToJulianDay(int year, int month, int day, qint64 &jd) const override;
    KCalendarSystemIndianNational(KCalendarSystemIndianNationalPrivate &dd, const KSharedConfig::Ptr config, const KLocale *locale);

private:
    Q_DECLARE_PRIVATE(KCalendarSystemIndianNational)
};

#endif // KCALENDARSYSTEMINDIANNATIONAL_H
