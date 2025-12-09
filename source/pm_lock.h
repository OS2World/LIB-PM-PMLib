/*
 * Copyright (C) 2016 Dmitry A.Steklenev
 */

#ifndef PM_LOCK_H
#define PM_LOCK_H

#include <pm_noncopyable.h>

/**
 * Locks a resource for a specified period of time.
 *
 * A special class PMLock protect resources (such as files,
 * data in memory, and peripheral devices) from simultaneous access by
 * several processes. Class also enable threads to serialize their access
 * to resources.
 *
 * The protection is based on use of a class providing methods <i>request</i>
 * and <i>release</i> and can be used in procedures, where probably may
 * be exception occurence between requesting and clearing of a resource.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMLock : public PMNonCopyable
{
  public:

    /**
     * Constructs the lock and request access to resource.
     *
     * Request access to resource. Blocks the calling thread
     * indefinitely.
     */

    PMLock( T& res ) : m_res( res ) {
      m_res.request();
    }

    /**
     * Destroys the lock and relinquishes ownership of a resource.
     *
     * Relinquishes ownership of a resource that was requested by
     * constructor.
     */

   ~PMLock() {
      m_res.release();
    }

  private:
    T& m_res;
};

#endif
