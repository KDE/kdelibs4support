/*
  Copyright (C) 2001 Michael Jarrett <michaelj@corel.com>
  Copyright (C) 2001 Carsten Pfeiffer <pfeiffer@kde.org>
  Copyright (C) 2009 Shaun Reich <shaun.reich@kdemail.net>

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

#ifndef KDIRSELECTDIALOG_H
#define KDIRSELECTDIALOG_H

#include <kdelibs4support_export.h>

#include <QDialog>
#include <QUrl>

class QAbstractItemView;

/**
 * A pretty dialog for a KDirSelect control for selecting directories.
 * @author Michael Jarrett <michaelj@corel.com>
 * @deprecated since 5.0, use QFileDialog::getExistingDirectoryUrl instead.
 */
class KDELIBS4SUPPORT_DEPRECATED_EXPORT KDirSelectDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Creates a new directory selection dialog.
     * @internal use the static selectDirectory function
     * @param startDir the directory, initially shown
     * @param localOnly unused. You can only select paths below the startDir
     * @param parent the parent for the dialog, usually 0L
     */
    KDELIBS4SUPPORT_DEPRECATED explicit KDirSelectDialog(const QUrl &startDir = QUrl(),
                              bool localOnly = false,
                              QWidget *parent = nullptr);

    /**
     * Destroys the directory selection dialog.
     */
    ~KDirSelectDialog() override;

    /**
     * Returns the currently selected URL, or an empty one if no item is selected.
     *
     * If the URL entered in the combobox is valid and exists, it is returned.
     * Otherwise, the URL selected in the treeview is returned instead.
     */
    QUrl url() const;

    /**
     * Returns a pointer to the view which is used for displaying the directories.
     */
    QAbstractItemView *view() const;

    /**
     * Returns whether only local directories can be selected.
     */
    bool localOnly() const;

    /**
     * Creates a KDirSelectDialog, and returns the result.
     * @param startDir the directory, initially shown
     * The tree will display this directory and subdirectories of it.
     * @param localOnly unused. You can only select paths below the startDir
     * @param parent the parent widget to use for the dialog, or NULL to create a parent-less dialog
     * @param caption the caption to use for the dialog, or QString() for the default caption
     * @return The URL selected, or an empty URL if the user canceled
     * or no URL was selected.
     *
     * @deprecated since 5.0, use QFileDialog::getExistingDirectory (if localOnly was true)
     * or QFileDialog::getExistingDirectoryUrl (if localOnly was false) instead.
     */
    static KDELIBS4SUPPORT_DEPRECATED QUrl selectDirectory(const QUrl &startDir = QUrl(),
            bool localOnly = false, QWidget *parent = nullptr,
            const QString &caption = QString());

    /**
     * @return The path for the root node
     */
    QUrl startDir() const;

public Q_SLOTS:
    /**
     * Sets the current @p url in the dialog.
     */
    void setCurrentUrl(const QUrl &url);

protected:
    void accept() override;

    /**
     * Reimplemented for saving the dialog geometry.
     */
    void hideEvent(QHideEvent *event) override;

private:
    class Private;
    Private *const d;

    Q_PRIVATE_SLOT(d, void slotCurrentChanged())
    Q_PRIVATE_SLOT(d, void slotExpand(const QModelIndex &))
    Q_PRIVATE_SLOT(d, void slotUrlActivated(const QString &))
    Q_PRIVATE_SLOT(d, void slotComboTextChanged(const QString &))
    Q_PRIVATE_SLOT(d, void slotContextMenuRequested(const QPoint &))
    Q_PRIVATE_SLOT(d, void slotNewFolder())
    Q_PRIVATE_SLOT(d, void slotMoveToTrash())
    Q_PRIVATE_SLOT(d, void slotDelete())
    Q_PRIVATE_SLOT(d, void slotProperties())
};

#endif
