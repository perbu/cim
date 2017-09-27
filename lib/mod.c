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

/******************************************************************************
                                             INTEGER PROCEDURE MOD(i,j)      */

long 
__rmod (i, j)
     long i,
       j;
{
  long res;
  if (j == 0)
    __rerror ("Mod: Second parameter is equal zero");
  res = i % j;
  return ((res == 0) ? 0 : ((res > 0) != (j > 0)) ? res + j : res);
}
