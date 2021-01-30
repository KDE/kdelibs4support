/*
    Copyright (c) 2002-2003 Carlos Moro <cfmoro@correo.uniovi.es>
    Copyright (c) 2002-2003 Hans Petter Bieker <bieker@kde.org>
    Copyright 2007, 2010 John Layt <john@layt.net>
        Calendar conversion routines based on Hdate v6, by Amos
        Shapir 1978 (rev. 1985, 1992)

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

#ifndef KCALENDARSYSTEMHEBREW_H
#define KCALENDARSYSTEMHEBREW_H

#include "kcalendarsystem.h"

class KCalendarSystemHebrewPrivate;

/**
 * @internal
 * This is the Hebrew calendar implementation.
 *
 * The Hebrew calendar is the traditional calendar used by the Jewish faith
 *
 * @see KLocale,KCalendarSystem,KCalendarSystemFactory
 *
 * @author Hans Petter Bieker <bieker@kde.org>
 */
class KCalendarSystemHebrew : public KCalendarSystem
{
public:
    KDELIBS4SUPPORT_DEPRECATED explicit KCalendarSystemHebrew(const KSharedConfig::Ptr config, const KLocale *locale);
    ~KCalendarSystemHebrew() override;

    QString calendarType() const override;
    KLocale::CalendarSystem calendarSystem() const override;

    QDate epoch() const override;
    QDate earliestValidDate() const override;
    QDate latestValidDate() const override;

    QString monthName(int month, int year, MonthNameFormat format = LongName) const override;
    QString monthName(const QDate &date, MonthNameFormat format = LongName) const override;

    QString weekDayName(int weekDay, WeekDayNameFormat format = LongDayName) const override;
    QString weekDayName(const QDate &date, WeekDayNameFormat format = LongDayName) const override;

    int yearStringToInteger(const QString &sNum, int &iLength) const override;

    bool isLunar() const override;
    bool isLunisolar() const override;
    bool isSolar() const override;
    bool isProleptic() const override;

protected:
    bool julianDayToDate(qint64 js, int &year, int &month, int &day) const override;
    bool dateToJulianDay(int year, int month, int day, qint64 &jd) const override;
    KCalendarSystemHebrew(KCalendarSystemHebrewPrivate &dd, const KSharedConfig::Ptr config, const KLocale *locale);

private:
    Q_DECLARE_PRIVATE(KCalendarSystemHebrew)
};

#endif // KCALENDARSYSTEMHEBREW_H
