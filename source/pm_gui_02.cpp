/*
 * Copyright (C) 2001-2016 Dmitry A.Steklenev
 */

#include <stdlib.h>

// Methods of the PMGUI class are divided between two modules so that
// non-PM applications can safe use of objects of this class.

#include "pm_gui.h"
#include "pm_fileutils.h"

HAB PMGUI::m_hab[ PM_MAX_THREADS ];
HMQ PMGUI::m_hmq[ PM_MAX_THREADS ];

APIRET APIENTRY DosQueryModFromEIP( HMODULE *phMod, ULONG *pObjNum, ULONG BuffLen,
                                    PCHAR pBuff, ULONG *pOffset, ULONG Address );

/* Returns the current thread identifier.
 */

TID PMGUI::tid()
{
  PTIB ptib;
  DosGetInfoBlocks( &ptib, NULL );
  return ptib->tib_ptib2->tib2_ultid;
}

/* Returns the current process identifier.
 */

PID PMGUI::pid()
{
  PPIB ppib;
  DosGetInfoBlocks( NULL, &ppib );
  return ppib->pib_ulpid;
}

/* Returns the current process type code.
 */

ULONG PMGUI::ptype()
{
  PPIB ppib;
  DosGetInfoBlocks( NULL, &ppib );
  return ppib->pib_ultype;
}

/* Returns the anchor block handle of the current thread.
 */

HAB PMGUI::hab() {
  return m_hab[ tid()];
}

/* Returns the message queue handle of the current thread.
 */

HMQ PMGUI::hmq() {
  return m_hmq[ tid()];
}

/* Returns the current module handle.
 */

HMODULE PMGUI::module_handle()
{
  ULONG   obj_num = 0;
  ULONG   offset  = 0;
  HMODULE handle  = 0;
  char    name[_MAX_PATH];

  DosQueryModFromEIP( &handle, &obj_num, sizeof( name ), name,
                      &offset, (ULONG)(&PMGUI::module_handle));
  return  handle;
}

/* Returns the current module name.
 */

char* PMGUI::module_name( char* result, int size )
{
  if( result && size )
  {
    ULONG   obj_num = 0;
    ULONG   offset  = 0;
    HMODULE handle  = 0;

    DosQueryModFromEIP( &handle, &obj_num, size, result,
                        &offset, (ULONG)(&PMGUI::module_name));
    DosQueryModuleName( handle, size, result );
  }

  return result;
}

/* Returns the current program name.
 */

char* PMGUI::program_name( char* result, int size )
{
  if( result && size )
  {
    PPIB ppib;
    PTIB ptib;

    DosGetInfoBlocks( &ptib, &ppib );
    DosQueryModuleName( ppib->pib_hmte, size, result );
  }

  return result;
}

/* Returns the current program startup path.
 */

char* PMGUI::program_path( char* result, int size )
{
  char pathname[_MAX_PATH];
  program_name( pathname, sizeof( pathname ));
  return sdrivedir( result, pathname, size );
}
