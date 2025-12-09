/*
 * Copyright (C) 2016 Dmitry A.Steklenev
 */

#include <string.h>
#include <types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <nerrno.h>

#ifndef  TCPV40HDRS
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "pm_socket.h"

#define  CONNECT_TIMEOUT 15

/* Constructs the socket object.
 */

PMSocket::PMSocket()
{
  m_errno = 0;
  m_so = -1;
}

/* Destroys the socket object.
 */

PMSocket::~PMSocket()
{
  close();
}

/* Converts a string into an internet address.
 *
 * Converts a string containing a valid internet address using
 * dotted-decimal notation or host name into an internet address
 * number typed as an unsigned long value.
 */

u_long PMSocket::address( const char* hostname )
{
  u_long address;
  struct hostent* entry;

  if( !hostname ) {
    m_errno = HBASEERR + HOST_NOT_FOUND;
    return -1;
  }

  if(( address = inet_addr( hostname )) == -1 ) {
    if(( entry = gethostbyname( hostname )) != NULL ) {
      memcpy( &address, entry->h_addr, sizeof( address ));
    } else {
      #ifdef NETDB_INTERNAL
      if( h_errno == NETDB_INTERNAL && sock_errno()) {
        m_errno = sock_errno();
      } else {
      #endif
        m_errno = HBASEERR + h_errno;
      #ifdef NETDB_INTERNAL
      }
      if( !m_errno ) {
        m_errno = HBASEERR + HOST_NOT_FOUND;
      }
      #endif
    }
  }

  return address;
}

/* Requests a connection to a remote host.
 *
 * Creates an endpoint for communication and requests a
 * connection to a remote host.
 */

BOOL PMSocket::connect( u_long address, int port )
{
  struct sockaddr_in server = {0};

  if( address == -1 ) {
    return FALSE;
  }

  close();

  if(( m_so = socket( PF_INET, SOCK_STREAM, 0 )) != -1 )
  {
    #ifdef CONNECT_TIMEOUT
    int dontblock;
    #endif

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = address;
    server.sin_port = htons((u_short)port );

    #ifdef CONNECT_TIMEOUT

      dontblock = 1;
      ioctl( m_so, FIONBIO, (char*)&dontblock, sizeof( dontblock ));

      if( ::connect( m_so, (struct sockaddr*)&server, sizeof( server )) != -1 ||
          sock_errno() == SOCEINPROGRESS )
      {
        struct timeval timeout = {0};
        fd_set waitlist;

        timeout.tv_sec = CONNECT_TIMEOUT;

        FD_ZERO( &waitlist    );
        FD_SET ( m_so, &waitlist );

        if( select( m_so + 1, NULL, &waitlist, NULL, &timeout ) <= 0 ) {
          if( sock_errno() == SOCEINPROGRESS ) {
            m_errno = SOCETIMEDOUT;
          } else {
            m_errno = sock_errno();
          }
          soclose( m_so );
          m_so = -1;
        } else {
          dontblock = 0;
          ioctl( m_so, FIONBIO, (char*)&dontblock, sizeof( dontblock ));
        }
      } else {
        m_errno = sock_errno();
        soclose( m_so );
        m_so = -1;
      }
    #else
      if( ::connect( m_so, (struct sockaddr*)&server, sizeof( server )) == -1 ) {
        m_errno = sock_errno();
        soclose( m_so );
        m_so = -1;
      }
    #endif
  } else {
    m_errno = sock_errno();
  }

  return ( m_so != -1 );
}

/* Shuts down a socket and frees resources allocated to the socket.
 */

BOOL PMSocket::close()
{
  if( m_so != -1 ) {
    m_errno = soclose( m_so );
    m_so = -1;
    return !m_errno;
  } else {
    return TRUE;
  }
}

/* Sends data on a connected socket. When successful, returns TRUE.
 * The return value FALSE indicates an error was detected on the sending side
 * of the connection.
 */

BOOL PMSocket::write( const char* buffer, int size )
{
  while( size )
  {
    int done = send( m_so, (char*)buffer, size, 0 );

    if( done <= 0 ) {
      m_errno = sock_errno();
      return FALSE;
    }

    buffer += done;
    size   -= done;
  }

  return TRUE;
}

/* Receives data on a socket and stores it in the buffer. When successful,
 * the number of bytes of data received into the buffer is returned. The
 * value 0 indicates that the connection is closed. The value -1 indicates
 * an error.
 */

int PMSocket::read( char* buffer, int size )
{
  int read = 0;
  int done;

  while( read < size )
  {
    done = recv( m_so, buffer + read, size - read, 0 );

    if( done < 0 ) {
      m_errno = sock_errno();
      break;
    } else if( done == 0 ) {
      break;
    } else {
      read += done;
    }
  }

  return read;
}

/* Receives string on a socket and stores it in the buffer.
 *
 * Receives bytes on a socket up to the first new-line character (\n)
 * or until the number of bytes received is equal to n-1, whichever
 * comes first. Stores the result in string and adds a null
 * character (\0) to the end of the string. If n is equal to 1, the
 * string is empty. Returns a pointer to the string buffer if successful.
 * A NULL return value indicates an error or that the connection is closed.
 */

char* PMSocket::readline( char* buffer, int size )
{
  int done = 0;
  char* p  = buffer;

  while( done < size - 1 ) {
    if( recv( m_so, p, 1, 0 ) == 1 ) {
      if( *p == '\r' ) {
        continue;
      } else if( *p == '\n' ) {
        break;
      } else {
        ++p;
        ++done;
      }
    } else {
      if( !done ) {
        return NULL;
      } else {
        break;
      }
    }
  }

  *p = 0;
  return buffer;
}

/* Maps the error number in <i>errnum</i> to an error message string.
 */

const char* PMSocket::strerror( int errnum )
{
  if( errnum >= HBASEERR ) {
    switch( errnum - HBASEERR )
    {
      case HOST_NOT_FOUND : return "Host not found.";
      case TRY_AGAIN      : return "Server failure.";
      case NO_RECOVERY    : return "Non-recoverable error.";
      case NO_DATA        : return "Valid name, no data record of requested type.";

      default:
        return "";
    }
  } else if( errnum >= SOCBASEERR ) {
    return sock_strerror( errnum );
  } else {
    return strerror( errnum );
  }
}

