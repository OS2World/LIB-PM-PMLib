/*
 * Copyright (C) 2010-2020 Dmitry A.Steklenev
 */

#include "pm_exception.h"

ULONG EXPENTRY
PMExceptionHandler( PEXCEPTIONREPORTRECORD       pRep,
                    PEXCEPTIONREGISTRATIONRECORD pReg,
                    PCONTEXTRECORD,
                    PVOID  )
{
  if( pRep->ExceptionNum == XCPT_ACCESS_VIOLATION ) {
    DosUnsetExceptionHandler( pReg );
    longjmp(((PMEXCEPTION*)pReg)->env, 1 );
  }

  return XCPT_CONTINUE_SEARCH;
}

