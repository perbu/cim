/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen and Terje Mjoes,
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
                                CHARACTER PROCEDURE GETCHAR (text attributt) */

char 
__rtgetchar (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    __rerror ("Getchar: Notext");
  else if (t->pos > t->length)
    __rerror ("Getchar: Pos outside limits");
  return (t->obj->string[(long) t->start + (long) (t->pos++) - 2]);
}
