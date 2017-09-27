/* $Id: cextspec.c,v 1.17 1997/01/26 14:30:17 cim Exp $ */

/* Copyright (C) 1994, 1998  Sverre Hvammen Johansen,
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */


#include "config.h"
#include "const.h"
#include "newstr.h"
#include "filelist.h"

#include <stdio.h>
#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack osFilelist;

typedef struct _filelist filelist_t;
typedef struct _fileelem fileelem_t;

struct _fileelem
{
  fileelem_t *next;
  char *name;
};

struct _filelist
{
  fileelem_t *first;
  fileelem_t *last;
};

static filelist_t dirlist, archlist, linklist;



/******************************************************************************
                                                                INITFILELIST */

void initFilelist ()
{
  obstack_init (&osFilelist);
}


/******************************************************************************
                                                                  CLEAR_LIST */

clear_list (listp) filelist_t *listp;
{
  listp->first=listp->last=0;
}
/******************************************************************************
                                                                     IS_NAME */

char is_name (listp, name) filelist_t *listp; char *name;
{
  fileelem_t *elem;
  for (elem= listp->first; elem!=NULL; elem= elem->next)
    if (!strcmp (name, elem->name)) return (1);
  return (0);
}

/******************************************************************************
                                                                   GET_NAMES */

char *get_names (listp) filelist_t *listp;
{
  fileelem_t *elem;
  for (elem= listp->first; elem!=NULL; elem= elem->next)
    {
      newstrGrow2 (elem->name, " ");
    }
  return newstrFinish ();
}

/******************************************************************************
                                                       GET_NAMES_IN_LINKLIST */

char * get_names_in_linklist ()
{
  return get_names(&linklist);
}

/******************************************************************************
                                                                 INSERT_NAME */

char insert_name (listp, name, first) filelist_t *listp; char *name, first;
{
  fileelem_t *new;
  new= (fileelem_t *) obstack_alloc (&osFilelist, sizeof (fileelem_t));
  new->next= NULL;
  new->name= name;

  if (is_name(listp, name)) return FALSE;

  if (listp->first == NULL)
    {
      listp->first= listp->last= new;
    }
  else
    {
      if (first)
	{
	  new->next= listp->first;
	  listp->first= new;
	} 
      else
	{
	  listp->last= listp->last->next= new;
	}
    }
  return TRUE;
}

/******************************************************************************
                                                      INSERT_NAME_IN_DIRLIST */

char insert_name_in_dirlist (name) char *name;
{
  if (name==NULL)
    {
      clear_list (&dirlist);
    }
  else
    {
      insert_name (&linklist, newstrcat2 ("-L", name), FALSE);
      return insert_name (&dirlist, name, FALSE);
    }
}

/******************************************************************************
                                                     INSERT_NAME_IN_ARCHLIST */

char insert_name_in_archlist (name) char *name;
{
  return insert_name (&archlist, name, FALSE);
}

/******************************************************************************
                                                     INSERT_NAME_IN_LINKLIST */

char insert_name_in_linklist (name, first) char *name, first;
{
  return insert_name (&linklist, name, first);
}

/******************************************************************************
                                                              TRANSFORM_NAME */

char * transform_name(name, fromsuffix, tosuffix) 
  char *name, *fromsuffix, *tosuffix;
{
  newstrGrown (name, strlen(name) - strlen(fromsuffix));
  newstrGrow1 (tosuffix);
  return newstrFinish ();
}

/******************************************************************************
                                                                   OPEN_NAME */

FILE *open_name (dirlist, linklist, name, link) 
  filelist_t *dirlist, *linklist; char *name; char link;
{
  FILE *f;
  fileelem_t *elem;
  char *str;
  for (elem= dirlist->first; elem!=NULL; elem= elem->next)
    {
      obstack_grow (&osFilelist, elem->name, strlen (elem->name));
      obstack_1grow (&osFilelist, '/');
      obstack_grow0 (&osFilelist, name, strlen (name));
      str= obstack_finish (&osFilelist);
#if OPEN_FILE_IN_BINARY_MODE
      if ((f = fopen (str, "rb"))!= NULL)
#else
      if ((f = fopen (str, "r"))!= NULL)
#endif
	{
	  if (link) 
	    insert_name (linklist, transform_name (str, ".atr", ".o"), TRUE);
	  return (f);
	}
      obstack_free (&osFilelist, str);
    }
  return (NULL);
}

/******************************************************************************
                                                             SHORT_FILE_NAME */

char * short_file_name (f)
     FILE *f;
{
  int i,
    c;
  static char s[17];
  for (i = 0; i < 16; i++)
    s[i] = getc (f);
  for (i = 15; i >= 0; i--)
    if (s[i] != ' ')
      break;
  if (i >= 0 && s[i] != '/')
    i++;
  s[i] = '\0';
  if ((c = getc (f)) != 0)
    ungetc (c, f);
  if (c == EOF)
    return (NULL);
  else
    return (s);
}

/******************************************************************************
                                                 OPEN_AND_POSITION_ARCH_NAME */

FILE *open_and_position_arch_name (archname, name) char *archname, *name;
{
  FILE *f;
  char *string_table=NULL;
  int i,
    sl;
  long l;
  char s1[17], *s2, c;

#if OPEN_FILE_IN_BINARY_MODE
  f = fopen (archname, "rb");
#else
  f = fopen (archname, "r");
#endif
  if (f == NULL)
    merror (6, archname);
  fscanf (f, "%7s", s1);
  getc (f);
  if (strcmp (s1, "!<arch>"))
    merror (7, archname);
  while ((s2 = short_file_name (f)) != NULL)
    {
      fscanf (f, "%12ld", &l);
      fscanf (f, "%6ld", &l);
      fscanf (f, "%6ld", &l);
      fscanf (f, "%8ld", &l);
      fscanf (f, "%10ld", &l);
      while ((c = getc (f)) != '`' && c != EOF);
      if (c != '`' || getc (f) != '\n')
	merror (8, archname);
      if (!strcmp (s2, name)) goto found;

      if (s2[0] == '/' && isdigit(s2[1]))
	{
	  long pos;
	  sscanf(&s2[1],"%ld",&pos);
	  if (!strcmp(&string_table[pos], name)) goto found;
	}
      if (l & 1)
	l++;
      if(!strcmp(s2,"/"))
	{
	  while (l-- >0)
	    {
	      char c;
	      c = getc (f);
	      if (c == '/' || c == '\n')
		obstack_1grow (&osFilelist, 0);
	      else obstack_1grow (&osFilelist, c);
	    }
	  string_table= (char *) obstack_finish (&osFilelist);
	} else
	  {
#if NO_SEEK_IN_AR
	    while (l-- > 0)
	      getc (f);
#else
	    fseek (f, l, 1);
#endif
	  }
    }
  fclose (f);
  f= NULL;
 found:
  if (string_table) obstack_free (&osFilelist, string_table);
  return f;
}

/******************************************************************************
                                             SEARC_AND_OPEN_NAME_IN_ARCHLIST */

FILE *searc_and_open_name_in_archlist (name, link)
char *name; char link;
{
  FILE *f;
  fileelem_t *elem;
#if OPEN_FILE_IN_BINARY_MODE
  if ((f = fopen (name, "rb")) != NULL)
#else
  if ((f = fopen (name, "r")) != NULL)
#endif
    {
      if (link) insert_name (&linklist, transform_name(name,".atr",".o"), TRUE);
      return (f);
    }
  
  f=open_name (&dirlist, &linklist, name, link);

  for (elem= archlist.first; elem!=NULL; elem= elem->next)
    if ((f= open_and_position_arch_name (elem->name, name)) != NULL) 
      return(f);

  return (NULL);
}

/******************************************************************************
                                                       SEARC_AND_INSERT_NAME */

char searc_and_insert_name (dirlistp, listp, name) 
  filelist_t *dirlistp, *listp; char *name;
{
  FILE *f;
  fileelem_t *elem;
  char *str;
  for (elem= dirlistp->first; elem!=NULL; elem= elem->next)
    {
      obstack_grow (&osFilelist, elem->name, strlen (elem->name));
      obstack_1grow (&osFilelist, '/');
      obstack_grow0 (&osFilelist, name, strlen (name));
      str= obstack_finish (&osFilelist);
      if ((f = fopen (str, "r")) != NULL)
	{
	  insert_name (listp, str, FALSE);
	  fclose(f);
	  return TRUE;
	}
      obstack_free (&osFilelist, str);
    }
  return FALSE;
}

/******************************************************************************
                                                                     NEW_LIB */

#define LIBPREFIX "lib"
#define LIBSUFFIX ".a"
#define LIBARCHSUFFIX "-atr.a"

void new_lib (name) 
  char *name;
{
  searc_and_insert_name (&dirlist, &archlist, 
			 transform_name (newstrcat3 (LIBPREFIX, name, 
						     LIBSUFFIX), 
					 LIBSUFFIX, LIBARCHSUFFIX));
					 
  insert_name (&linklist, newstrcat2 ("-l", name), FALSE);
}




