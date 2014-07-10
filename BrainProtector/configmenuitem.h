/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyrught 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/********************************************************
 *	configmenuitem.h				*
 *	Config editing menu code.			*
 *							*
 *	Changelog:					*
 *	- Date-version/build:				*
 *		(State) Change.				*
 *							*
 *	- 5.1.2011-0.0/3:				*
 *		(Done) MenuItem.			*
 *		(Done) ItemCreate().			*
 *		       ^^^^^^^^^^^^ configmenuitem.c	*
 *		(Done) MenuFree().			*
 *		       ^^^^^^^^^^ configmenuitem.c	*
 *		(Done) MenuAppend().			*
 *		       ^^^^^^^^^^^^ configmenuitem.c	*
 *		(Done) Menu().				*
 *		       ^^^^^^^^^^^^ configmenuitem.c	*
 ********************************************************/

#ifndef _CONFIGMENUITEM_H_
#define _CONFIGMENUITEM_H_

	#include <stdlib.h>
	#include <stdio.h>
	#include <wchar.h>

	typedef struct _MenuItem
	{
		wchar_t *item;
		void (*function)();
		struct _MenuItem *next;
	} MenuItem;

	MenuItem *ItemCreate(wchar_t *, void (*)());
	void MenuFree(MenuItem *);
	void MenuAppend(MenuItem *, MenuItem *);

#endif

