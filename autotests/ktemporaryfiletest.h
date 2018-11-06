/*
This file is part of the KDE libraries
This file has been placed in the Public Domain.
*/

#ifndef ktemporaryfiletest_h
#define ktemporaryfiletest_h

#include <QObject>
#include <QString>

class KTemporaryFileTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testKTemporaryFile();

private:
    QString kdeTempDir;
    QString componentName;
};

#endif
