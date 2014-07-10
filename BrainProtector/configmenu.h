/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/********************************************************
 *	configmenu.h					*
 *	Config editor menu code.			*
 *							*
 *	Changelog:					*
 *	- Date-version/build:				*
 *		(State) Change.				*
 *							*
 *	- 6.1.2011-0.0/6:				*
 *		(Done) MenuStackItem			*
 *		       ^^^^^^^^^^^^^ configmenu.c	*
 *		(Done) MenuPush()			*
 *		       ^^^^^^^^^^ configmenu.c		*
 *		(Done) MenuPop()			*
 *		       ^^^^^^^^^ configmenu.c		*
 ********************************************************/

#ifndef _CONFIGMENU_H_
#define _CONFIGMENU_H_

	#include <configmenuitem.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <wchar.h>
	
	void MenuPush(MenuItem *, wchar_t *);
	void MenuPop();
	void Menu();

#endif

