/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen,
 * Department of Informatics, University of Oslo.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cim.h"
#include "config.h"
#include "simfile.h"

/******************************************************************************
                                         PROCEDURE LOCK(t,i,j)               */

/*ARGSUSED */
long 
__rdblock (p, t, i, j)
     __bs10FILE *p;
     double t;
     long i,
       j;
{
  long lock = -1;
  if (t > 0.0)
    {
      if (p->locked)
	(void) __rdbunlock (p);
    }
  return (lock);
}
