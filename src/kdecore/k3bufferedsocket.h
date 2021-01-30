/*  -*- C++ -*-
 *  Copyright (C) 2003,2005 Thiago Macieira <thiago@kde.org>
 *
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef KBUFFEREDSOCKET_H
#define KBUFFEREDSOCKET_H

#include <kdelibs4support_export.h>
#include "k3streamsocket.h"

#include <QObject>
#include <QByteArray>
#include <QList>

namespace KNetwork
{

class KBufferedSocketPrivate;
/** @class KBufferedSocket k3bufferedsocket.h k3bufferedsocket.h
 *  @brief Buffered stream sockets.
 *
 * This class allows the user to create and operate buffered stream sockets
 * such as those used in most Internet connections. This class is
 * also the one that resembles the most to the old QSocket
 * implementation.
 *
 * Objects of this type operate only in non-blocking mode. A call to
 * setBlocking(true) will result in an error.
 *
 * @note Buffered sockets only make sense if you're using them from
 *       the main (event-loop) thread. This is actually a restriction
 *       imposed by Qt's QSocketNotifier. If you want to use a socket
 *       in an auxiliary thread, please use KStreamSocket.
 *
 * @see KNetwork::KStreamSocket, KNetwork::KServerSocket
 * @author Thiago Macieira <thiago@kde.org>
 * @deprecated Use KSocketFactory or KLocalSocket instead
 */
class KDELIBS4SUPPORT_DEPRECATED_EXPORT KBufferedSocket: public KStreamSocket
{
    Q_OBJECT
public:
    /**
     * Default constructor.
     *
     * @param node    destination host
     * @param service destination service to connect to
     * @param parent      the parent object for this object
     */
    KDELIBS4SUPPORT_DEPRECATED explicit KBufferedSocket(const QString &node = QString(), const QString &service = QString(),
                             QObject *parent = nullptr);

    /**
     * Destructor.
     */
    ~KBufferedSocket() override;

    /**
     * Be sure to catch new devices.
     */
    void setSocketDevice(KSocketDevice *device) override;

protected:
    /**
     * Buffered sockets can only operate in non-blocking mode.
     */
    bool setSocketOptions(int opts) override;

public:
    /**
     * Closes the socket for new data, but allow data that had been buffered
     * for output with writeData() to be still be written.
     *
     * @sa closeNow
     */
    void close() override;

    /**
     * Make use of the buffers.
     */
    qint64 bytesAvailable() const override;

    /**
     * Make use of buffers.
     */
    qint64 waitForMore(int msecs, bool *timeout = nullptr) override;

    /**
     * Catch changes.
     */
    void enableRead(bool enable) override;

    /**
     * Catch changes.
     */
    void enableWrite(bool enable) override;

    /**
     * Sets the use of input buffering.
     */
    void setInputBuffering(bool enable);

    /**
     * Sets the use of output buffering.
     */
    void setOutputBuffering(bool enable);

    /**
     * Returns the length of the output buffer.
     */
    qint64 bytesToWrite() const override;

    /**
     * Closes the socket and discards any output data that had been buffered
     * with writeData() but that had not yet been written.
     *
     * @sa close
     */
    virtual void closeNow();

    /**
     * Returns true if a line can be read with readLine()
     */
    bool canReadLine() const override;

    // KDE4: make virtual, add timeout to match the Qt4 signature
    //       and move to another class up the hierarchy
    /**
     * Blocks until the connection is either established, or completely
     * failed.
     */
    void waitForConnect();

protected:
    /**
     * Reads data from a socket.
     *
     * The @p from parameter is always set to peerAddress()
     */
    qint64 readData(char *data, qint64 maxlen, KSocketAddress *from) override;

    /**
     * Peeks data from the socket.
     *
     * The @p from parameter is always set to peerAddress()
     */
    qint64 peekData(char *data, qint64 maxlen, KSocketAddress *from) override;

    /**
     * Writes data to the socket.
     *
     * The @p to parameter is discarded.
     */
    qint64 writeData(const char *data, qint64 len, const KSocketAddress *to) override;

    /**
     * Improve the readLine performance
     */
    qint64 readLineData(char *data, qint64 maxSize) override;

    /**
     * Catch connection to clear the buffers
     */
    void stateChanging(SocketState newState) override;

protected Q_SLOTS:
    /**
     * Slot called when there's read activity.
     */
    void slotReadActivity() override;

    /**
     * Slot called when there's write activity.
     */
    void slotWriteActivity() override;

#if 0
    // Already present in QIODevice
Q_SIGNALS:
    /**
     * This signal is emitted whenever data is written.
     */
    void bytesWritten(int bytes);
#endif

private:
    KBufferedSocket(const KBufferedSocket &);
    KBufferedSocket &operator=(const KBufferedSocket &);

    KBufferedSocketPrivate *const d;
};

}               // namespace KNetwork

#endif
