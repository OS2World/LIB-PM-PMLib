/*
 * Copyright (C) 2006-2015 Dmitry A.Steklenev
 */

#ifndef PM_SHAREDPTR_H
#define PM_SHAREDPTR_H

#include "pm_os2.h"
#include "pm_memory.h"
#include "pm_smp.h"

/**
 * Shared pointer to object.
 *
 * The PMSharedPtr class template stores a pointer to a dynamically
 * allocated object, typically with a C++ new-expression. The object
 * pointed to is guaranteed to be cleaned when the last PMSharedPtr
 * pointing to it is destroyed.
 *
 * You can construct, destruct, copy, and assign objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMSharedPtr
{
  private:
    /** Pointer holder class. */
    class ptr_holder
    {
      public:
        ptr_holder( T* p )
        : m_p( p ),
          m_referenced( 1 )
        {}

        T* m_p;
        unsigned int m_referenced;
    };

  public:

    /**
     * Constructs the shared pointer.
     *
     * Use these constructor to create new objects with reference
     * counters set to 1.
     *
     * @exception bad_alloc If the implementation cannot allocate memory storage.
     */

    explicit PMSharedPtr( T* p ) {
      m_holder = new ptr_holder( p );
    }

    /** Constructs the "NULL" shared pointer. */
    PMSharedPtr() {
      m_holder = new ptr_holder( 0 );
    }

    /**
     * Copy constructor.
     *
     * Use this constructor to create a copy of the given object.
     * The constructor increments the reference count of the object holded
     * to by the given PMSharedPtr object.
     */

    PMSharedPtr( const PMSharedPtr<T>& reference )
    {
      reference.inc_reference();
      m_holder = reference.m_holder;
    }

    /**
     * Destructs the shared pointer.
     *
     * This destructor destroys the PMSharedPtr object and decrements the
     * reference count of the holded object. When the count is zero, the
     * holded object is cleaned.
     */

   ~PMSharedPtr() {
      dec_reference();
    }

    /**
     * Assignment operator.
     *
     * Use this operator to modify the PMSharedPtr object so that it
     * refers to the same object as another. If the object the pointer
     * previously referred to was a valid object, that object's reference
     * count is decremented. When the count is zero, that object is
     * cleaned.
     */

    PMSharedPtr<T>& operator=( const PMSharedPtr<T>& reference )
    {
      if( this != &reference )
      {
        reference.inc_reference();
        dec_reference();
        m_holder = reference.m_holder;
      }

      return *this;
    }

    /**
     * Returns a reference to the pointed object.
     * @exception none
     */

    T& operator* () {
      return *(m_holder->m_p);
    }

    /**
     * Returns a reference to the pointed object.
     * @exception none
     */

    const T& operator* () const {
      return *(m_holder->m_p);
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    T* operator->() {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    const T* operator->() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    operator const T*() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed object.
     * @exception none
     */

    operator T*() {
      return m_holder->m_p;
    }

    /**
     * Returns whether the stored pointer is NULL.
     * @exception none
     */

    bool is_null() const {
      return !m_holder->m_p;
    }

  private:
    ptr_holder* m_holder;

    /** Increments the reference count. */
    void inc_reference() const {
      xadd( &m_holder->m_referenced, 1 );
    }

    /** Decrements the reference count. */
    void dec_reference() const
    {
      if( xadd( &m_holder->m_referenced, -1 ) == 1 ) {
        delete m_holder->m_p;
        delete m_holder;
      }
    }
};

/**
 * Shared pointer to array of objects.
 *
 * The PMSharedArrayPtr class template stores a pointer to a dynamically
 * allocated array of objects, typically with a C++ new[]-expression. The
 * array pointed to is guaranteed to be cleaned when the last PMSharedArrayPtr
 * pointing to it is destroyed.
 *
 * You can construct, destruct, copy, and assign objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMSharedArrayPtr
{
  private:
    /** Pointer holder class. */
    class ptr_holder
    {
      public:
        ptr_holder( T* p )
        : m_p( p ),
          m_referenced( 1 )
        {}

        T* m_p;
        unsigned int m_referenced;
    };

  public:

    /**
     * Constructs the shared pointer.
     *
     * Use these constructor to create new objects with reference
     * counters set to 1.
     *
     * @exception bad_alloc If the implementation cannot allocate memory storage.
     */

    explicit PMSharedArrayPtr( T* p ) {
      m_holder = new ptr_holder( p );
    }

    /** Constructs the "NULL" shared pointer. */
    PMSharedArrayPtr() {
      m_holder = new ptr_holder( 0 );
    }

    /**
     * Copy constructor.
     *
     * Use this constructor to create a copy of the given object.
     * The constructor increments the reference count of the array holded
     * to by the given PMSharedArrayPtr object.
     */

    PMSharedArrayPtr( const PMSharedArrayPtr<T>& reference )
    {
      reference.inc_reference();
      m_holder = reference.m_holder;
    }

    /**
     * Destructs the shared pointer.
     *
     * This destructor destroys the PMSharedArrayPtr object and decrements the
     * reference count of the holded array. When the count is zero, the
     * holded array is cleaned.
     */

   ~PMSharedArrayPtr() {
      dec_reference();
    }

    /**
     * Assignment operator.
     *
     * Use this operator to modify the PMSharedArrayPtr object so that it
     * refers to the same array as another. If the object the pointer
     * previously referred to was a valid array, that array's reference
     * count is decremented. When the count is zero, that array
     * is cleaned.
     */

    PMSharedArrayPtr<T>& operator=( const PMSharedArrayPtr<T>& reference )
    {
      if( this != &reference )
      {
        reference.inc_reference();
        dec_reference();
        m_holder = reference.m_holder;
      }

      return *this;
    }

    /**
     * Returns a reference to the pointed array.
     * @exception none
     */

    T& operator* () {
      return *(m_holder->m_p);
    }

    /**
     * Returns a reference to the pointed array.
     * @exception none
     */

    const T& operator* () const {
      return *(m_holder->m_p);
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    T* operator->() {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    const T* operator->() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    operator const T*() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed array.
     * @exception none
     */

    operator T*() {
      return m_holder->m_p;
    }

    /**
     * Returns whether the stored pointer is NULL.
     * @exception none
     */

    bool is_null() const {
      return !m_holder->m_p;
    }

  private:
    ptr_holder* m_holder;

    /** Increments the reference count. */
    void inc_reference() const {
      xadd( &m_holder->m_referenced, 1 );
    }

    /** Decrements the reference count. */
    void dec_reference() const
    {
      if( xadd( &m_holder->m_referenced, -1 ) == 1 ) {
        delete[] m_holder->m_p;
        delete m_holder;
      }
    }
};

/**
 * Shared pointer to memory.
 *
 * The PMSharedMemPtr class template stores a pointer to a dynamically
 * allocated memory, typically with a C++ malloc() function. The memory
 * pointed to is guaranteed to be freed when the last PMSharedMemPtr
 * pointing to it is destroyed.
 *
 * You can construct, destruct, copy, and assign objects of this class.
 *
 * @author  Dmitry A Steklenev
 * @version 1.0
 */

template <class T> class PMSharedMemPtr
{
  private:
    /** Pointer holder class. */
    class ptr_holder
    {
      public:
        ptr_holder( T* p )
        : m_p( p ),
          m_referenced( 1 )
        {}

        T* m_p;
        unsigned int m_referenced;
    };

  public:

    /**
     * Constructs the shared pointer.
     *
     * Use these constructor to create new objects with reference
     * counters set to 1.
     *
     * @exception bad_alloc If the implementation cannot allocate memory storage.
     */

    explicit PMSharedMemPtr( T* p ) {
      m_holder = new ptr_holder( p );
    }

    /** Constructs the "NULL" shared pointer. */
    PMSharedMemPtr() {
      m_holder = new ptr_holder( 0 );
    }

    /**
     * Copy constructor.
     *
     * Use this constructor to create a copy of the given object.
     * The constructor increments the reference count of the memory holded
     * to by the given PMSharedMemPtr object.
     */

    PMSharedMemPtr( const PMSharedMemPtr<T>& reference )
    {
      reference.inc_reference();
      m_holder = reference.m_holder;
    }

    /**
     * Destructs the shared pointer.
     *
     * This destructor destroys the PMSharedMemPtr object and decrements the
     * reference count of the holded memory. When the count is zero, the
     * holded memory is freed.
     */

   ~PMSharedMemPtr() {
      dec_reference();
    }

    /**
     * Assignment operator.
     *
     * Use this operator to modify the PMSharedMemPtr object so that it
     * refers to the same memory area as another. If the object the pointer
     * previously referred to was a valid object, that memory's reference
     * count is decremented. When the count is zero, that memory is freed.
     */

    PMSharedMemPtr<T>& operator=( const PMSharedMemPtr<T>& reference )
    {
      if( this != &reference )
      {
        reference.inc_reference();
        dec_reference();
        m_holder = reference.m_holder;
      }

      return *this;
    }

    /**
     * Returns a reference to the pointed memory.
     * @exception none
     */

    T& operator* () {
      return *(m_holder->m_p);
    }

    /**
     * Returns a reference to the pointed memory.
     * @exception none
     */

    const T& operator* () const {
      return *(m_holder->m_p);
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    T* operator->() {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    const T* operator->() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    operator const T*() const {
      return m_holder->m_p;
    }

    /**
     * Returns a pointer to the pointed memory.
     * @exception none
     */

    operator T*() {
      return m_holder->m_p;
    }

    /**
     * Returns whether the stored pointer is NULL.
     * @exception none
     */

    bool is_null() const {
      return !m_holder->m_p;
    }

  private:
    ptr_holder* m_holder;

    /** Increments the reference count. */
    void inc_reference() const {
      xadd( &m_holder->m_referenced, 1 );
    }

    /** Decrements the reference count. */
    void dec_reference() const
    {
      if( xadd( &m_holder->m_referenced, -1 ) == 1 ) {
        xfree( m_holder->m_p );
        delete m_holder;
      }
    }
};

#endif
