/*
 * Copyright (C) 2013-2016 Dmitry A.Steklenev
 */

#ifndef PM_SCOPEDPTR_H
#define PM_SCOPEDPTR_H

#include "pm_os2.h"
#include "pm_memory.h"

/**
 * Scoped pointer to object.
 *
 * The PMScopedPtr class template stores a pointer to a dynamically
 * allocated object, typically with a C++ new-expression. The object
 * pointed to is guaranteed to be cleaned when the PMScopedPtr
 * pointing to it is destroyed.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMScopedPtr
{
  public:

    /**
     * Constructs the scoped pointer.
     * @exception none
     */

    explicit PMScopedPtr( T* p ) {
      m_p = p;
    }

    /**
     * Destructs the scoped pointer.
     *
     * This destructor destroys the PMScopedPtr object and cleans the
     * holded object.
     */

   ~PMScopedPtr() {
      delete m_p;
    }

    /**
     * Returns a reference to the pointed object.
     * @exception none
     */

    T& operator* () {
      return *m_p;
    }

    /**
     * Returns a reference to the pointed object.
     * @exception none
     */

    const T& operator* () const {
      return *m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    T* operator->() {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    const T* operator->() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    operator const T*() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    operator T*() {
      return m_p;
    }

    /**
     * Assignment operator.
     * @exception none
     */

    PMScopedPtr& operator=( T* p ) {
      m_p = p;
      return *this;
    }

  private:
    T* m_p;
};

/**
 * Scoped pointer to array of objects.
 *
 * The PMScopedArrayPtr class template stores a pointer to a dynamically
 * allocated array of objects, typically with a C++ new[]-expression. The
 * array pointed to is guaranteed to be cleaned when the PMScopedArrayPtr
 * pointing to it is destroyed.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMScopedArrayPtr
{
  public:

    /**
     * Constructs the scoped pointer.
     * @exception none
     */

    explicit PMScopedArrayPtr( T* p ) {
      m_p = p;
    }

    /**
     * Destructs the scoped pointer.
     *
     * This destructor destroys the PMScopedArrayPtr object and cleans the
     * holded array.
     */

   ~PMScopedArrayPtr() {
      delete[] m_p;
    }

    /**
     * Returns a reference to the pointed array.
     * @exception none
     */

    T& operator* () {
      return *m_p;
    }

    /**
     * Returns a reference to the pointed array.
     * @exception none
     */

    const T& operator* () const {
      return *m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    T* operator->() {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    const T* operator->() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    operator const T*() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    operator T*() {
      return m_p;
    }

    /**
     * Assignment operator.
     * @exception none
     */

    PMScopedArrayPtr& operator=( T* p ) {
      m_p = p;
      return *this;
    }

  private:
    T* m_p;
};

/**
 * Scoped pointer to memory.
 *
 * The PMScopedMemPtr class template stores a pointer to a dynamically
 * allocated memory, typically with a C++ malloc() function. The memory
 * pointed to is guaranteed to be freed when the PMScopedMemPtr
 * pointing to it is destroyed.
 *
 * You can construct and destruct objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMScopedMemPtr
{
  public:

    /**
     * Constructs the scoped pointer.
     * @exception none
     */

    explicit PMScopedMemPtr( T* p = NULL ) {
      m_p = p;
    }

    /**
     * Destructs the scoped pointer.
     *
     * This destructor destroys the PMScopedMemPtr object and frees the
     * holded memory area.
     */

   ~PMScopedMemPtr() {
      xfree( m_p );
    }

    /**
     * Returns a reference to the pointed memory.
     * @exception none
     */

    T& operator* () {
      return *m_p;
    }

    /**
     * Returns a reference to the pointed memory.
     * @exception none
     */

    const T& operator* () const {
      return *m_holder;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    T* operator->() {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    const T* operator->() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    operator const T*() const {
      return m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    operator T*() {
      return m_p;
    }

    /**
     * Assignment operator.
     * @exception none
     */

    PMScopedMemPtr& operator=( T* p ) {
      m_p = p;
      return *this;
    }

  private:
    T* m_p;
};

#endif
