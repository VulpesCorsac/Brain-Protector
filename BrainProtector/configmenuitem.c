/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/****************************************
 *	configmenuitem.c		*
 *	Menu code for bpconfig.		*
 *					*
 *	Changelog:			*
 *	- Date-version/build:		*
 *		(State) Change.		*
 *					*
 *	- 5.1.11-0.0/2:			*
 *		(Done) ItemCreate()	*
 *		(Done) MenuFree()	*
 *		(Done) ItemAppend()	*
 *		(Done) Menu()		*
 ****************************************/

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <configmenuitem.h>

MenuItem *ItemCreate(wchar_t *itemtext, void (*itemfunction)())
{
	MenuItem *item = malloc(sizeof(MenuItem));

	item->item = itemtext;
	item->function = itemfunction;
	item->next = 0;

	return item;
}

void MenuFree(MenuItem *menu)
{
	MenuItem *item = menu->next;

	while(item)
	{
		free(menu);
		menu = item;
		item = item->next;
	}

	free(menu);
}

void MenuAppend(MenuItem *menu, MenuItem *item)
{
	while(menu->next)
	{
		menu = menu->next;
	}

	menu->next = item;
}

