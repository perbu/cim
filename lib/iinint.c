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
#include "simfile.h"

/******************************************************************************
                                           INTEGER PROCEDURE ININT           */

long 
__riinint (p)
     __bs2FILE *p;
{
  __txt t;
  long inint;
  if (__rilastitem (p))
    __rerror ("Inint: End of file");
  t.obj = p->IMAGE.obj;
  t.start = p->IMAGE.start + p->IMAGE.pos - 1;
  t.length = p->IMAGE.length - p->IMAGE.pos + 1;
  t.pos = 1;
  inint = __rtgetint (&t);
  p->IMAGE.pos += t.pos - 1;
  return (inint);
}
