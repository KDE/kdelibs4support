// -*- c++ -*-
/* This file is part of the KDE libraries
    Copyright (C) 1997, 1998 Richard Moore <rich@kde.org>
                  1998 Stephan Kulow <coolo@kde.org>
                  1998 Daniel Grana <grana@ie.iwi.unibe.ch>
                  1999,2000,2001,2002,2003 Carsten Pfeiffer <pfeiffer@kde.org>
                  2003 Clarence Dang <dang@kde.org>
                  2008 Jarosław Staniek <staniek@kde.org>
                  2009 David Jarvie <djarvie@kde.org>

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

#include "kfiledialog.h"

#include <QCheckBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <qmimedatabase.h>

#include <kimageio.h>
#include <klocalizedstring.h>
#include <krecentdocument.h>
#include <QDebug>
#include <kwindowsystem.h>
#include "kfilewidget.h"
#include "krecentdirs.h"
#include "kservice.h"
#include <ksharedconfig.h>
#include <kwindowconfig.h>

//From defaults-kfile.h in KIOFileWidgets
#define ConfigGroup QLatin1String("KFileDialog Settings")

/** File dialogs are native by default on Windows. */
#if defined(Q_OS_WIN)
const bool NATIVE_FILEDIALOGS_BY_DEFAULT = true;
#else
const bool NATIVE_FILEDIALOGS_BY_DEFAULT = false;
#endif

static QStringList mime2KdeFilter(const QStringList &mimeTypes, QString *allExtensions = nullptr)
{
    QMimeDatabase db;
    QStringList kdeFilter;
    QStringList allExt;
    foreach (const QString &mimeType, mimeTypes) {
        QMimeType mime(db.mimeTypeForName(mimeType));
        if (mime.isValid()) {
            allExt += mime.globPatterns();
            kdeFilter.append(mime.globPatterns().join(QLatin1String(" ")) +
                             QLatin1Char('|') +
                             mime.comment());
        }
    }
    if (allExtensions) {
        allExt.sort();
        *allExtensions = allExt.join(QLatin1String(" "));
    }
    return kdeFilter;
}
/** @return File dialog filter in Qt format for @a filters
 *          or "All files (*)" for empty list.
 */
static QString qtFilter(const QStringList &_filters)
{
    QString converted;
    const QStringList filters = _filters;

    foreach (const QString &current, filters) {
        QString new_f; //filter part
        QString new_name; //filter name part
        int p = current.indexOf('|');
        if (p == -1) {
            new_f = current;
            new_name = current; // nothing better found
        } else {
            new_f = current.left(p);
            new_name = current.mid(p + 1);
        }
        //Qt filters assume anything in () is the file extension list
        new_name = new_name.replace('(', '[').replace(')', ']').trimmed();

        //convert everything to lower case and remove dupes (doesn't matter on win32)
        QStringList allfiltersUnique;
        const QStringList origList(new_f.split(' ', QString::SkipEmptyParts));
        foreach (const QString &origFilter, origList) {
            if (!allfiltersUnique.contains(origFilter, Qt::CaseInsensitive)) {
                allfiltersUnique += origFilter.toLower();
            }
        }

        if (!converted.isEmpty()) {
            converted += ";;";
        }

        converted += (new_name + " (" + allfiltersUnique.join(" ") + QLatin1Char(')'));
    }

    // Strip escape characters from escaped '/' characters.
    converted.replace("\\/", "/");

    return converted;
}

/** @return File dialog filter in Qt format for @a filter in KDE format
 *          or "All files (*)" for empty filter.
 */
static QString qtFilter(const QString &filter)
{
    // Qt format: "some text (*.first *.second)" or "All files (*)" separated by ;;
    // KDE format: "*.first *.second|Description" or "*|Description", separated by \n (Description is optional)
    QStringList filters;
    if (filter.isEmpty()) {
        filters += i18n("*|All files");
    } else {
        // check if it's a mimefilter
        int pos = filter.indexOf('/');
        if (pos > 0 && filter[pos - 1] != '\\') {
            filters = mime2KdeFilter(filter.split(QLatin1Char(' '), QString::SkipEmptyParts));
        } else {
            filters = filter.split('\n', QString::SkipEmptyParts);
        }
    }
    return qtFilter(filters);
}

class KFileDialogPrivate
{
public:
    /** Data used for native mode. */
    class Native
    {
    public:
        Native()
            : mode(KFile::File),
              operationMode(KFileWidget::Opening)
        {
        }
        /** @return previously set (global) start dir or the first url
         selected using setSelection() or setUrl() if the start dir is empty. */
        QUrl startDir() const
        {
            if (!s_startDir.isEmpty()) {
                return s_startDir;
            }
            if (!selectedUrls.isEmpty()) {
                return selectedUrls.first();
            }
            return QUrl();
        }
        /** @return previously set (global) start dir or @p defaultDir
         if the start dir is empty. */
        static QUrl staticStartDir(const QUrl &defaultDir)
        {
            if (s_startDir.isEmpty()) {
                return defaultDir;
            }
            return s_startDir;
        }
        static QUrl s_startDir;
        static bool s_allowNative;  // as fallback when we can't use native dialog
        QString filter;
        QString selectedFilter;
        QStringList mimeTypes;
        QList<QUrl> selectedUrls;
        KFile::Modes mode;
        KFileWidget::OperationMode operationMode;
    };

    KFileDialogPrivate()
        : native(nullptr),
          w(nullptr),
          cfgGroup(KSharedConfig::openConfig(), ConfigGroup)
    {
        if (cfgGroup.readEntry("Native", NATIVE_FILEDIALOGS_BY_DEFAULT) &&
                KFileDialogPrivate::Native::s_allowNative) {
            native = new Native;
        }
    }

    static bool isNative()
    {
        if (!KFileDialogPrivate::Native::s_allowNative) {
            return false;
        }
        KConfigGroup cfgGroup(KSharedConfig::openConfig(), ConfigGroup);
        return cfgGroup.readEntry("Native", NATIVE_FILEDIALOGS_BY_DEFAULT);
    }

    static QString getOpenFileName(const QUrl &startDir, const QString &filter,
                                   QWidget *parent, const QString &caption,
                                   QString *selectedFilter);
    static QUrl getOpenUrl(const QUrl &startDir, const QString &filter,
                           QWidget *parent, const QString &caption,
                           QString *selectedFilter);
    static QStringList getOpenFileNames(const QUrl &startDir, const QString &filter,
                                        QWidget *parent, const QString &caption,
                                        QString *selectedFilter);
    static QList<QUrl> getOpenUrls(const QUrl &startDir, const QString &filter,
                                   QWidget *parent, const QString &caption,
                                   QString *selectedFilter);
    static QString getSaveFileName(const QUrl &dir, const QString &filter,
                                   QWidget *parent, const QString &caption,
                                   KFileDialog::Options options, QString *selectedFilter);
    static QUrl getSaveUrl(const QUrl &dir, const QString &filter,
                           QWidget *parent, const QString &caption,
                           KFileDialog::Options options, QString *selectedFilter);

    ~KFileDialogPrivate()
    {
        delete native;
    }

    Native *native;
    KFileWidget *w;
    KConfigGroup cfgGroup;
};

QUrl KFileDialogPrivate::Native::s_startDir;
bool KFileDialogPrivate::Native::s_allowNative = true;

KFileDialog::KFileDialog(const QUrl &startDir, const QString &filter,
                         QWidget *parent, QWidget *customWidget)
#ifdef Q_OS_WIN
    : QDialog(parent, Qt::WindowMinMaxButtonsHint),
#else
    : QDialog(parent),
#endif
      d(new KFileDialogPrivate)

{
    if (d->native) {
        KFileDialogPrivate::Native::s_startDir = startDir;
        // check if it's a mimefilter
        int pos = filter.indexOf('/');
        if (pos > 0 && filter[pos - 1] != '\\') {
            setMimeFilter(filter.split(QLatin1Char(' '), QString::SkipEmptyParts));
        } else {
            setFilter(filter);
        }
        return;
    }

    d->w = new KFileWidget(startDir, this);
    KFileWidget *fileQWidget = d->w;

    KWindowConfig::restoreWindowSize(windowHandle(), d->cfgGroup); // call this before the fileQWidget is set as the main widget.
    // otherwise the sizes for the components are not obeyed (ereslibre)

    d->w->setFilter(filter);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fileQWidget);
    setLayout(layout);

    d->w->okButton()->show();
    connect(d->w->okButton(), SIGNAL(clicked()), SLOT(slotOk()));
    d->w->cancelButton()->show();
    connect(d->w->cancelButton(), SIGNAL(clicked()), SLOT(slotCancel()));

    // Publish signals
    connect(fileQWidget, SIGNAL(fileSelected(QUrl)),
            SIGNAL(fileSelected(QUrl)));
    connect(fileQWidget, SIGNAL(fileHighlighted(QUrl)),
            SIGNAL(fileHighlighted(QUrl)));
    connect(fileQWidget, SIGNAL(selectionChanged()),
            SIGNAL(selectionChanged()));
    connect(fileQWidget, SIGNAL(filterChanged(QString)),
            SIGNAL(filterChanged(QString)));

    connect(fileQWidget, SIGNAL(accepted()), SLOT(accept()));
    //connect(fileQWidget, SIGNAL(canceled()), SLOT(slotCancel()));

    if (customWidget) {
        d->w->setCustomWidget(QString(), customWidget);
    }
}

KFileDialog::~KFileDialog()
{
    delete d;
}

void KFileDialog::setLocationLabel(const QString &text)
{
    if (d->w) {
        d->w->setLocationLabel(text);
    }
}

void KFileDialog::setFilter(const QString &filter)
{
    if (d->native) {
        d->native->filter = filter;
        return;
    }
    d->w->setFilter(filter);
}

QString KFileDialog::currentFilter() const
{
    if (d->w) {
        return d->w->currentFilter();
    }
    return QString();    // not available
}

void KFileDialog::setMimeFilter(const QStringList &mimeTypes,
                                const QString &defaultType)
{
    if (d->native) {
        QString allExtensions;
        QStringList filters = mime2KdeFilter(mimeTypes, &allExtensions);
        if (defaultType.isEmpty() && (mimeTypes.count() > 1)) {
            filters.prepend(allExtensions + QLatin1Char('|') + i18n("All Supported Files"));
        }
        d->native->filter = filters.join(QLatin1String("\n"));
        return;
    }
    d->w->setMimeFilter(mimeTypes, defaultType);
}

void KFileDialog::clearFilter()
{
    if (d->native) {
        d->native->filter.clear();
        return;
    }
    d->w->clearFilter();
}

QString KFileDialog::currentMimeFilter() const
{
    if (d->native) {
        // adapted from qt2KdeFilter
        QString filter = d->native->selectedFilter.split(";;").replaceInStrings("/", "\\/")[0];
        filter = filter.mid(filter.indexOf('(') + 1, filter.indexOf(')') - filter.indexOf('(') - 1);
        QMimeDatabase db;
        QString mimetype = db.mimeTypeForFile("test" + filter.mid(1).split(' ')[0], QMimeDatabase::MatchExtension).name();
        return mimetype;
    }
    return d->w->currentMimeFilter();
}

QMimeType KFileDialog::currentFilterMimeType()
{
    QMimeDatabase db;
    return db.mimeTypeForName(currentMimeFilter());
}

void KFileDialog::setPreviewWidget(KPreviewWidgetBase *w)
{
    if (d->w) {
        d->w->setPreviewWidget(w);
    }
}

void KFileDialog::setInlinePreviewShown(bool show)
{
    if (d->w) {
        d->w->setInlinePreviewShown(show);
    }
}

// This is only used for the initial size when no configuration has been saved
QSize KFileDialog::sizeHint() const
{
    if (d->w) {
        return d->w->dialogSizeHint();
    }
    return QSize();
}

// This slot still exists mostly for compat purposes; for subclasses which reimplement slotOk
void KFileDialog::slotOk()
{
    if (d->w) {
        d->w->slotOk();
    }
}

// This slot still exists mostly for compat purposes; for subclasses which reimplement accept
void KFileDialog::accept()
{
    if (d->w) {
        setResult(QDialog::Accepted);   // keep old behavior; probably not needed though
        d->w->accept();
        KConfigGroup cfgGroup(KSharedConfig::openConfig(), ConfigGroup);
        QDialog::accept();
    }
}

// This slot still exists mostly for compat purposes; for subclasses which reimplement slotCancel
void KFileDialog::slotCancel()
{
    if (d->w) {
        d->w->slotCancel();
        reject();
    }
}

void KFileDialog::setUrl(const QUrl &url, bool clearforward)
{
    if (d->native) {
        d->native->selectedUrls.clear();
        d->native->selectedUrls.append(url);
        return;
    }
    d->w->setUrl(url, clearforward);
}

void KFileDialog::setSelection(const QString &name)
{
    if (d->native) {
        d->native->selectedUrls.clear();
        d->native->selectedUrls.append(QUrl(name)); // could be relative
        return;
    }
    d->w->setSelection(name);
}

QString KFileDialog::getOpenFileName(const QUrl &startDir,
                                     const QString &filter,
                                     QWidget *parent, const QString &caption)
{
    return KFileDialogPrivate::getOpenFileName(startDir, filter, parent, caption, nullptr);
}

QString KFileDialogPrivate::getOpenFileName(const QUrl &startDir,
        const QString &filter,
        QWidget *parent,
        const QString &caption,
        QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        return QFileDialog::getOpenFileName(
                   parent,
                   caption.isEmpty() ? i18n("Open") : caption,
                   KFileDialogPrivate::Native::staticStartDir(startDir).toLocalFile(),
                   qtFilter(filter),
                   selectedFilter);
// TODO use extra args?     QString * selectedFilter = 0, Options options = 0
    }
    KFileDialog dlg(startDir, filter, parent);

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::File | KFile::LocalOnly | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);

    dlg.exec();
    if (selectedFilter) {
        *selectedFilter = dlg.currentMimeFilter();
    }
    return dlg.selectedFile();
}

QString KFileDialog::getOpenFileNameWId(const QUrl &startDir,
                                        const QString &filter,
                                        WId parent_id, const QString &caption)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        return KFileDialog::getOpenFileName(startDir, filter, nullptr, caption);    // everything we can do...
    }
    QWidget *parent = QWidget::find(parent_id);
    KFileDialogPrivate::Native::s_allowNative = false;
    KFileDialog dlg(startDir, filter, parent);
    if (parent == nullptr && parent_id != 0) {
        dlg.setAttribute(Qt::WA_NativeWindow, true);
        KWindowSystem::setMainWindow(dlg.windowHandle(), parent_id);
    }

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::File | KFile::LocalOnly | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);

    dlg.exec();

    return dlg.selectedFile();
}

QStringList KFileDialog::getOpenFileNames(const QUrl &startDir,
        const QString &filter,
        QWidget *parent,
        const QString &caption)
{
    return KFileDialogPrivate::getOpenFileNames(startDir, filter, parent, caption, nullptr);
}

QStringList KFileDialogPrivate::getOpenFileNames(const QUrl &startDir,
        const QString &filter,
        QWidget *parent,
        const QString &caption,
        QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        return QFileDialog::getOpenFileNames(
                   parent,
                   caption.isEmpty() ? i18n("Open") : caption,
                   KFileDialogPrivate::Native::staticStartDir(startDir).toLocalFile(),
                   qtFilter(filter), selectedFilter);
// TODO use extra args?  QString * selectedFilter = 0, Options options = 0
    }
    KFileDialogPrivate::Native::s_allowNative = false;
    KFileDialog dlg(startDir, filter, parent);

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::Files | KFile::LocalOnly | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);

    dlg.exec();
    if (selectedFilter) {
        *selectedFilter = dlg.currentMimeFilter();
    }
    return dlg.selectedFiles();
}

QUrl KFileDialog::getOpenUrl(const QUrl &startDir, const QString &filter,
                             QWidget *parent, const QString &caption)
{
    return KFileDialogPrivate::getOpenUrl(startDir, filter, parent, caption, nullptr);
}
QUrl KFileDialogPrivate::getOpenUrl(const QUrl &startDir, const QString &filter,
                                    QWidget *parent, const QString &caption,
                                    QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        const QString fileName(KFileDialogPrivate::getOpenFileName(
                                   startDir, filter, parent, caption, selectedFilter));
        return fileName.isEmpty() ? QUrl() : QUrl::fromLocalFile(fileName);
    }
    KFileDialogPrivate::Native::s_allowNative = false;
    KFileDialog dlg(startDir, filter, parent);

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::File | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);

    dlg.exec();
    if (selectedFilter) {
        *selectedFilter = dlg.currentMimeFilter();
    }
    return dlg.selectedUrl();
}

QList<QUrl> KFileDialog::getOpenUrls(const QUrl &startDir,
                                     const QString &filter,
                                     QWidget *parent,
                                     const QString &caption)
{
    return KFileDialogPrivate::getOpenUrls(startDir, filter, parent, caption, nullptr);
}

QList<QUrl> KFileDialogPrivate::getOpenUrls(const QUrl &startDir,
        const QString &filter,
        QWidget *parent,
        const QString &caption,
        QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        const QStringList fileNames(KFileDialogPrivate::getOpenFileNames(
                                        startDir, filter, parent, caption, selectedFilter));
        QList<QUrl> urls;
        Q_FOREACH (const QString &file, fileNames) {
            urls.append(QUrl::fromLocalFile(file));
        }
        return urls;
    }
    KFileDialogPrivate::Native::s_allowNative = false;

    KFileDialog dlg(startDir, filter, parent);

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::Files | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);

    dlg.exec();
    if (selectedFilter) {
        *selectedFilter = dlg.currentMimeFilter();
    }
    return dlg.selectedUrls();
}

void KFileDialog::setConfirmOverwrite(bool enable)
{
    if (d->w && operationMode() == KFileDialog::Saving) {
        d->w->setConfirmOverwrite(enable);
    }
}

QUrl KFileDialog::getExistingDirectoryUrl(const QUrl &startDir,
        QWidget *parent,
        const QString &caption)
{
    return QFileDialog::getExistingDirectoryUrl(parent, caption, KFileDialogPrivate::Native::staticStartDir(startDir));
}

QString KFileDialog::getExistingDirectory(const QUrl &startDir,
        QWidget *parent,
        const QString &caption)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) {
        return QFileDialog::getExistingDirectory(parent, caption,
                KFileDialogPrivate::Native::staticStartDir(startDir).toLocalFile(),
                QFileDialog::ShowDirsOnly);
    }
    QUrl url = KFileDialog::getExistingDirectoryUrl(startDir, parent, caption);
    if (url.isValid()) {
        return url.toLocalFile();
    }
    return QString();
}

QUrl KFileDialog::getImageOpenUrl(const QUrl &startDir, QWidget *parent,
                                  const QString &caption)
{
    if (KFileDialogPrivate::isNative() && (!startDir.isValid() || startDir.isLocalFile())) { // everything we can do...
        const QStringList mimetypes(KImageIO::mimeTypes(KImageIO::Reading));
        return KFileDialog::getOpenUrl(startDir, mimetypes.join(" "), parent, caption);
    }
    const QStringList mimetypes = KImageIO::mimeTypes(KImageIO::Reading);
    KFileDialogPrivate::Native::s_allowNative = false;
    KFileDialog dlg(startDir, mimetypes.join(" "), parent);

    dlg.setOperationMode(KFileDialog::Opening);
    dlg.setMode(KFile::File | KFile::ExistingOnly);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Open") : caption);
    dlg.setInlinePreviewShown(true);

    dlg.exec();

    return dlg.selectedUrl();
}

QUrl KFileDialog::selectedUrl() const
{
    if (d->native) {
        return d->native->selectedUrls.isEmpty() ? QUrl() : d->native->selectedUrls.first();
    }
    return d->w->selectedUrl();
}

QList<QUrl> KFileDialog::selectedUrls() const
{
    if (d->native) {
        return d->native->selectedUrls;
    }
    return d->w->selectedUrls();
}

QString KFileDialog::selectedFile() const
{
    if (d->native) {
        return selectedUrl().toLocalFile();
    }
    return d->w->selectedFile();
}

QStringList KFileDialog::selectedFiles() const
{
    if (d->native) {
        QStringList lst;
        Q_FOREACH (const QUrl &u, selectedUrls()) {
            if (u.isLocalFile()) {
                lst.append(u.toLocalFile());
            }
        }
        return lst;
    }
    return d->w->selectedFiles();
}

QUrl KFileDialog::baseUrl() const
{
    if (d->native) {
        return selectedUrl().isEmpty() ? QUrl() : QUrl::fromLocalFile(selectedUrl().path());    /* shouldn't that be .directory()? */
    }
    return d->w->baseUrl();
}

QString KFileDialog::getSaveFileName(const QUrl &dir, const QString &filter,
                                     QWidget *parent,
                                     const QString &caption)
{
    //TODO KDE5: replace this method by the method below (with default parameter values in declaration)
    // Set no confirm-overwrite mode for backwards compatibility
    return KFileDialogPrivate::getSaveFileName(dir, filter, parent, caption, Options(), nullptr);
}

QString KFileDialog::getSaveFileName(const QUrl &dir, const QString &filter,
                                     QWidget *parent,
                                     const QString &caption, Options options)
{
    return KFileDialogPrivate::getSaveFileName(dir, filter, parent, caption, options, nullptr);
}

QString KFileDialogPrivate::getSaveFileName(const QUrl &dir, const QString &filter,
        QWidget *parent, const QString &caption,
        KFileDialog::Options options, QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative()) {
        bool defaultDir = dir.isEmpty();
        bool specialDir = !defaultDir && dir.scheme() == "kfiledialog";
        QUrl startDir;
        QString recentDirClass;
        if (specialDir) {
            startDir = KFileDialog::getStartUrl(dir, recentDirClass);
        } else if (!specialDir && !defaultDir) {
            if (!dir.isLocalFile()) {
                qWarning() << "non-local start dir " << dir;
            }
            startDir = dir;
        }

        QFileDialog::Options opts = (options & KFileDialog::ConfirmOverwrite) ? QFileDialog::Options() : QFileDialog::DontConfirmOverwrite;
        const QString result = QFileDialog::getSaveFileName(
                                   parent,
                                   caption.isEmpty() ? i18n("Save As") : caption,
                                   KFileDialogPrivate::Native::staticStartDir(startDir).toLocalFile(),
                                   qtFilter(filter),
// TODO use extra args?     QString * selectedFilter = 0, Options opts = 0
                                   selectedFilter, opts);
        if (!result.isEmpty()) {
            if (!recentDirClass.isEmpty()) {
                KRecentDirs::add(recentDirClass, QUrl::fromLocalFile(result).toString());
            }
            KRecentDocument::add(result);
        }
        return result;
    }

    KFileDialog dlg(dir, filter, parent);

    dlg.setOperationMode(KFileDialog::Saving);
    dlg.setMode(KFile::File | KFile::LocalOnly);
    dlg.setConfirmOverwrite(options & KFileDialog::ConfirmOverwrite);
    dlg.setInlinePreviewShown(options & KFileDialog::ShowInlinePreview);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Save As") : caption);

    dlg.exec();

    QString filename = dlg.selectedFile();
    if (!filename.isEmpty()) {
        KRecentDocument::add(filename);
    }

    return filename;
}

QString KFileDialog::getSaveFileNameWId(const QUrl &dir, const QString &filter,
                                        WId parent_id,
                                        const QString &caption)
{
    //TODO KDE5: replace this method by the method below (with default parameter values in declaration)
    // Set no confirm-overwrite mode for backwards compatibility
    return getSaveFileNameWId(dir, filter, parent_id, caption, Options());
}

QString KFileDialog::getSaveFileNameWId(const QUrl &dir, const QString &filter,
                                        WId parent_id,
                                        const QString &caption, Options options)
{
    if (KFileDialogPrivate::isNative()) {
        return KFileDialog::getSaveFileName(dir, filter, nullptr, caption, options); // everything we can do...
    }

    QWidget *parent = QWidget::find(parent_id);
    KFileDialog dlg(dir, filter, parent);
    if (parent == nullptr && parent_id != 0) {
        dlg.setAttribute(Qt::WA_NativeWindow, true);
        KWindowSystem::setMainWindow(dlg.windowHandle(), parent_id);
    }

    dlg.setOperationMode(KFileDialog::Saving);
    dlg.setMode(KFile::File | KFile::LocalOnly);
    dlg.setConfirmOverwrite(options & ConfirmOverwrite);
    dlg.setInlinePreviewShown(options & ShowInlinePreview);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Save As") : caption);

    dlg.exec();

    QString filename = dlg.selectedFile();
    if (!filename.isEmpty()) {
        KRecentDocument::add(filename);
    }

    return filename;
}

QUrl KFileDialog::getSaveUrl(const QUrl &dir, const QString &filter,
                             QWidget *parent, const QString &caption)
{
    //TODO KDE5: replace this method by the method below (with default parameter values in declaration)
    // Set no confirm-overwrite mode for backwards compatibility
    return KFileDialogPrivate::getSaveUrl(dir, filter, parent, caption, Options(), nullptr);
}

QUrl KFileDialog::getSaveUrl(const QUrl &dir, const QString &filter,
                             QWidget *parent, const QString &caption, Options options)
{
    return KFileDialogPrivate::getSaveUrl(dir, filter, parent, caption, options, nullptr);
}

QUrl KFileDialogPrivate::getSaveUrl(const QUrl &dir, const QString &filter,
                                    QWidget *parent, const QString &caption,
                                    KFileDialog::Options options, QString *selectedFilter)
{
    if (KFileDialogPrivate::isNative() && (!dir.isValid() || dir.isLocalFile())) {
        const QString fileName(KFileDialogPrivate::getSaveFileName(
                                   dir, filter, parent, caption, options, selectedFilter));
        return fileName.isEmpty() ? QUrl() : QUrl::fromLocalFile(fileName);
    }

    KFileDialogPrivate::Native::s_allowNative = false;

    KFileDialog dlg(dir, filter, parent);

    dlg.setOperationMode(KFileDialog::Saving);
    dlg.setMode(KFile::File);
    dlg.setConfirmOverwrite(options & KFileDialog::ConfirmOverwrite);
    dlg.setInlinePreviewShown(options & KFileDialog::ShowInlinePreview);
    dlg.setWindowTitle(caption.isEmpty() ? i18n("Save As") : caption);

    dlg.exec();
    if (selectedFilter) {
        *selectedFilter = dlg.currentMimeFilter();
    }
    QUrl url = dlg.selectedUrl();
    if (url.isValid()) {
        KRecentDocument::add(url);
    }

    return url;
}

void KFileDialog::setMode(KFile::Modes m)
{
    if (d->native) {
        d->native->mode = m;
    } else {
        d->w->setMode(m);
    }
}

KFile::Modes KFileDialog::mode() const
{
    if (d->native) {
        return d->native->mode;
    }
    return d->w->mode();
}

QPushButton *KFileDialog::okButton() const
{
    if (d->w) {
        return d->w->okButton();
    }
    return nullptr;
}

QPushButton *KFileDialog::cancelButton() const
{
    if (d->w) {
        return d->w->cancelButton();
    }
    return nullptr;
}

KUrlComboBox *KFileDialog::locationEdit() const
{
    if (d->w) {
        return d->w->locationEdit();
    }
    return nullptr;
}

KFileFilterCombo *KFileDialog::filterWidget() const
{
    if (d->w) {
        return d->w->filterWidget();
    }
    return nullptr;
}

KActionCollection *KFileDialog::actionCollection() const
{
    if (d->w) {
        return d->w->actionCollection();
    }
    return nullptr;
}

void KFileDialog::setKeepLocation(bool keep)
{
    if (d->w) {
        d->w->setKeepLocation(keep);
    }
}

bool KFileDialog::keepsLocation() const
{
    if (d->w) {
        return d->w->keepsLocation();
    }
    return false;
}

void KFileDialog::setOperationMode(OperationMode mode)
{
    if (d->native) {
        d->native->operationMode = static_cast<KFileWidget::OperationMode>(mode);
    } else {
        d->w->setOperationMode(static_cast<KFileWidget::OperationMode>(mode));
    }
}

KFileDialog::OperationMode KFileDialog::operationMode() const
{
    if (d->native) {
        return static_cast<KFileDialog::OperationMode>(d->native->operationMode);
    }
    return static_cast<KFileDialog::OperationMode>(d->w->operationMode());
}

void KFileDialog::keyPressEvent(QKeyEvent *e)
{
    if (d->w) {
        if (e->key() == Qt::Key_Escape) {
            e->accept();
            d->w->cancelButton()->animateClick();
        } else {
            QDialog::keyPressEvent(e);
        }
    }
}

void KFileDialog::hideEvent(QHideEvent *e)
{
    if (d->w) {
        KWindowConfig::saveWindowSize(windowHandle(), d->cfgGroup, KConfigBase::Persistent);

        QDialog::hideEvent(e);
    }
}

// static
QUrl KFileDialog::getStartUrl(const QUrl &startDir,
                              QString &recentDirClass)
{
    return KFileWidget::getStartUrl(startDir, recentDirClass);
}

void KFileDialog::setStartDir(const QUrl &directory)
{
    if (KFileDialogPrivate::isNative()) {
        KFileDialogPrivate::Native::s_startDir = directory;
    }
    KFileWidget::setStartDir(directory);
}

KToolBar *KFileDialog::toolBar() const
{
    if (d->w) {
        return d->w->toolBar();
    }
    return nullptr;
}

KFileWidget *KFileDialog::fileWidget()
{
    return d->w;
}

#ifdef Q_OS_WIN
int KFileDialog::exec()
{
    if (!d->native || !KFileDialogPrivate::Native::s_allowNative) {
        KFileDialogPrivate::Native::s_allowNative = true;
        return QDialog::exec();
    }

// not clear here to let KFileDialogPrivate::Native::startDir() return a useful value
// d->native->selectedUrls.clear();
    int res = QDialog::Rejected;
    switch (d->native->operationMode) {
    case KFileWidget::Opening:
    case KFileWidget::Other:
        if (d->native->mode & KFile::File) {
            QUrl url(KFileDialogPrivate::getOpenUrl(
                         d->native->startDir(), d->native->filter, parentWidget(), windowTitle(), &d->native->selectedFilter));
            if (url.isEmpty() || !url.isValid()) {
                res = QDialog::Rejected;
                break;
            }
            d->native->selectedUrls.clear();
            d->native->selectedUrls.append(url);
            res = QDialog::Accepted;
            break;
        } else if (d->native->mode & KFile::Files) {
            QList<QUrl> urls(KFileDialogPrivate::getOpenUrls(
                                 d->native->startDir(), d->native->filter, parentWidget(), windowTitle(), &d->native->selectedFilter));
            if (urls.isEmpty()) {
                res = QDialog::Rejected;
                break;
            }
            d->native->selectedUrls = urls;
            res = QDialog::Accepted;
            break;
        } else if (d->native->mode & KFile::Directory) {
            QUrl url(KFileDialog::getExistingDirectoryUrl(
                         d->native->startDir(), parentWidget(), windowTitle()));
            if (url.isEmpty() || !url.isValid()) {
                res = QDialog::Rejected;
                break;
            }
            d->native->selectedUrls.clear();
            d->native->selectedUrls.append(url);
            res = QDialog::Accepted;
            break;
        }
        break;
    case KFileWidget::Saving:
        if (d->native->mode & KFile::File) {
            QUrl url(KFileDialogPrivate::getSaveUrl(
                         d->native->startDir(), d->native->filter, parentWidget(), windowTitle(), Options(), &d->native->selectedFilter));
            if (url.isEmpty() || !url.isValid())  {
                res = QDialog::Rejected;
                break;
            }
            d->native->selectedUrls.clear();
            d->native->selectedUrls.append(url);
            res = QDialog::Accepted;
            break;
        } else if (d->native->mode & KFile::Directory) {
            QUrl url(KFileDialog::getExistingDirectoryUrl(
                         d->native->startDir(), parentWidget(), windowTitle()));
            if (url.isEmpty() || !url.isValid()) {
                res = QDialog::Rejected;
                break;
            }
            d->native->selectedUrls.clear();
            d->native->selectedUrls.append(url);
            res = QDialog::Accepted;
            break;
        }
        break;
    default:;
    }

    setResult(res);
    emit finished(res);

    if (res == QDialog::Accepted) {
        emit accepted();
    } else {
        emit rejected();
    }

    return res;
}
#endif // Q_OS_WIN

#include "moc_kfiledialog.cpp"
