/****************************************************************************
**
** Copyright (C) 2003 Carsten Pfeiffer <pfeiffer@kde.org>
**
****************************************************************************/

#ifndef KFDTEST_H
#define KFDTEST_H

#include <QObject>
#include <QUrl>

class KFDTest : public QObject
{
    Q_OBJECT

public:
    KFDTest(const QUrl &startDir, QObject *parent = nullptr, const char *name = nullptr);

public Q_SLOTS:
    void doit();

private:
    QUrl m_startDir;
};

#endif // KFDTEST_H
