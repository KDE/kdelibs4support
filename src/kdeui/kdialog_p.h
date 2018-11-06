/*  This file is part of the KDE project
    Copyright (C) 2007 Matthias Kretz <kretz@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

*/

#ifndef KDEUI_KDIALOG_P_H
#define KDEUI_KDIALOG_P_H

#include "kdialog.h"
#include <QPointer>
#include <QSignalMapper>
#include <QSize>
#include <QHash>

class QBoxLayout;
class KPushButton;
class KUrlLabel;
class KSeparator;
class QDialogButtonBox;

class KDialogPrivate
{
    Q_DECLARE_PUBLIC(KDialog)
protected:
    KDialogPrivate()
        : mDetailsVisible(false), mSettingDetails(false), mDeferredDelete(false),
          mDetailsWidget(nullptr),
          mTopLayout(nullptr), mMainWidget(nullptr), mUrlHelp(nullptr), mActionSeparator(nullptr),
          mButtonOrientation(Qt::Horizontal),
          mDefaultButton(KDialog::NoDefault),
          mButtonBox(nullptr)
    {
    }

    virtual ~KDialogPrivate() {}

    KDialog *q_ptr;

    void setupLayout();
    void appendButton(KDialog::ButtonCode code, const KGuiItem &item);

    bool mDetailsVisible;
    bool mSettingDetails;
    bool mDeferredDelete;
    QWidget *mDetailsWidget;
    QSize mIncSize;
    QSize mMinSize;
    QString mDetailsButtonText;

    QBoxLayout *mTopLayout;
    QPointer<QWidget> mMainWidget;
    KUrlLabel *mUrlHelp;
    KSeparator *mActionSeparator;

    QString mAnchor;
    QString mHelpApp;
    QString mHelpLinkText;

    Qt::Orientation mButtonOrientation;
    KDialog::ButtonCode mDefaultButton;
    KDialog::ButtonCode mEscapeButton;

    QDialogButtonBox *mButtonBox;
    QHash<int, KPushButton *> mButtonList;
    QSignalMapper mButtonSignalMapper;

protected Q_SLOTS:
    void queuedLayoutUpdate();
    void helpLinkClicked();

private:
    void init(KDialog *);
    bool dirty: 1;
};

#endif // KDEUI_KDIALOG_P_H
