/*
 * Copyright (C) 2016 Dmitry A.Steklenev
 */

#ifndef PM_SOCKET_H
#define PM_SOCKET_H

#include "pm_os2.h"
#include "pm_noncopyable.h"
#include <types.h>

#ifndef __ccdoc__
#define  HBASEERR 20000
#endif

/**
 * Base TCP/IP socket class.
 *
 * A socket is a communication channel abstraction that enables unrelated
 * processes to exchange data locally and across networks. A single socket
 * is one endpoint of a full-duplex (two-way) communication channel. This
 * means that data can be transmitted and received simultaneously.
 *
 * @author  Dmitry A.Steklenev
 * @version 1.0
 */

class PMSocket : public PMNonCopyable
{
  public:

    /** Constructs the socket object. */
    PMSocket();
    /** Destroys the socket object. */
   ~PMSocket();

    /**
     * Converts a string containing a valid internet address into an internet address.
     *
     * Converts a string containing a valid internet address using
     * dotted-decimal notation or host name into an internet address
     * number typed as an unsigned long value.  A -1 value
     * indicates an error.
     */

    u_long address( const char* hostname );

    /**
     * Requests a connection to a remote host.
     *
     * Creates an endpoint for communication and requests a
     * connection to a remote host.
     *
     * @return The return value FALSE indicates an error.
     */

    BOOL connect( u_long address, int port );

    /**
     * Requests a connection to a remote host.
     *
     * Creates an endpoint for communication and requests a
     * connection to a remote host.
     *
     * @return The return value FALSE indicates an error.
     */

    BOOL connect( const char* hostname, int port );

    /**
     * Shuts down a socket and frees resources allocated to the socket.
     *
     * @return Returns value TRUE indicates success; the value FALSE indicates an error.
     */

    BOOL close();

    /**
     * Sends data on a connected socket.
     *
     * @return When successful, returns TRUE. The return value FALSE indicates an
     *         error was detected on the sending side of the connection.
     */

    BOOL write( const char* buffer, int size );

    /**
     * Receives data on a socket and stores it in the buffer.
     *
     * @return When successful, the number of bytes of data received
     *         into the buffer is returned. The value 0 indicates that the
     *         connection is closed. The value -1 indicates an error.
     */

    int read( char* buffer, int size );

    /**
     * Receives string on a socket and stores it in the buffer.
     *
     * Receives bytes on a socket up to the first new-line character (\n)
     * or until the number of bytes received is equal to <i>n-1</i>, whichever
     * comes first. Stores the result in string and adds a null
     * character (\0) to the end of the string. If <i>n</i> is equal to 1, the
     * string is empty.
     *
     * @return Returns a pointer to the string buffer if successful.
     *         A NULL return value indicates an error or that the
     *         connection is closed.
     */

    char* readline( char* buffer, int size );

    /** Returns error code set by a socket method. */
    int errnum() const;
    /** Maps the error number in <i>errnum</i> to an error message string. */
    static const char* strerror( int errnum );
    /** Returns the socket descriptor. */
    int handle() const;

  private:

    int m_errno;
    int m_so;
};

/* Returns error code set by a socket method. */
inline int PMSocket::errnum() const {
  return m_errno;
}

/* Requests a connection to a remote host. */
inline BOOL PMSocket::connect( const char* hostname, int port ) {
  return connect( address( hostname ), port );
}

/* Returns the socket descriptor. */
inline int PMSocket::handle() const {
  return m_so;
}

#endif
