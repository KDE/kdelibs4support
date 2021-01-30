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

#ifndef KCALENDARSYSTEMTHAI_H
#define KCALENDARSYSTEMTHAI_H

#include "kcalendarsystemgregorian_p.h"

class KCalendarSystemThaiPrivate;

/**
 * @internal
 * This is the Thai calendar implementation which is the Gregorian calendar
 * but using a different Epoch
 *
 * @b license GNU-LGPL v.2 or later
 *
 * @see KLocale,KCalendarSystem
 *
 * @author John Layt <john@layt.net>
 */
class KCalendarSystemThai: public KCalendarSystemGregorian
{
public:
    KDELIBS4SUPPORT_DEPRECATED explicit KCalendarSystemThai(const KSharedConfig::Ptr config, const KLocale *locale);
    ~KCalendarSystemThai() override;

    QString calendarType() const override;
    KLocale::CalendarSystem calendarSystem() const override;

    QDate epoch() const override;
    QDate earliestValidDate() const override;
    QDate latestValidDate() const override;

    bool isLunar() const override;
    bool isLunisolar() const override;
    bool isSolar() const override;
    bool isProleptic() const override;

protected:
    bool julianDayToDate(qint64 jd, int &year, int &month, int &day) const override;
    bool dateToJulianDay(int year, int month, int day, qint64 &jd) const override;
    KCalendarSystemThai(KCalendarSystemThaiPrivate &dd, const KSharedConfig::Ptr config, const KLocale *locale);

private:
    Q_DECLARE_PRIVATE(KCalendarSystemThai)
};

#endif // KCALENDARSYSTEMTHAI_H
