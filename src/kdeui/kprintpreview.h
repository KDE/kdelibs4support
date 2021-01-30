/*
 *  This file is part of the KDE libraries
 *  Copyright (c) 2007 Alex Merry <alex.merry@kdemail.net>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/

#ifndef KPRINTPREVIEW_H
#define KPRINTPREVIEW_H

#include <kdelibs4support_export.h>

#include <QDialog>
#include <QPrinter>

class KPrintPreviewPrivate;

/**
 * KPrintPreview provides a print preview dialog.
 *
 * Use it like this:
 *
 * @code
 * QPrinter printer;
 * KPrintPreview preview(&printer);
 * doPrint(printer); // draws to the QPrinter
 * preview.exec();
 * @endcode
 *
 * @deprecated since 5.0, use QPrintPreviewDialog instead
 */
class KDELIBS4SUPPORT_EXPORT KPrintPreview : public QDialog
{
    Q_OBJECT

public:
    /**
     * Create a KPrintPreview object.
     *
     * This will change the settings on the QPrinter, so you
     * should not re-use the QPrinter object for printing
     * normally.
     *
     * @param printer pointer to a QPrinter to configure for
     *                print preview
     * @param parent  pointer to the parent widget for the dialog
     */
    KDELIBS4SUPPORT_DEPRECATED explicit KPrintPreview(QPrinter *printer, QWidget *parent = nullptr);
    ~KPrintPreview() override;

    /**
     * Returns true if the print preview system is available
     * @since KDE 4.5
     */
    static bool isAvailable();

protected:
    void showEvent(QShowEvent *event) override;

private:
    KPrintPreviewPrivate *const d;
};

#endif // KPRINTPREVIEW_H

