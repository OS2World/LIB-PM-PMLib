/*
 * Copyright (C) 2010-2016 Dmitry A.Steklenev
 */

#ifndef PM_SMP_H
#define PM_SMP_H

#include "pm_os2.h"

extern  unsigned int xchg( unsigned int* p, unsigned int x );
#pragma aux xchg = "xchg [esi],eax" parm [ESI][EAX] value [EAX];
extern  unsigned int xadd( unsigned int* p, unsigned int x );
#pragma aux xadd = "lock xadd [esi],eax" parm [ESI][EAX] value [EAX];

/** Exchanges the contents of the destination and source operands. */
template <class T> T xchg( T& p, T x ) {
  return (T)xchg((unsigned int*)&p, (unsigned int)x );
}

#endif
