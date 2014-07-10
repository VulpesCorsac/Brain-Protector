/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/****************************************
 *	configmenu.c			*
 *	Config editor menu code		*
 *					*
 *	Changelog:			*
 *	- Date-version/build:		*
 *		(State) Change.		*
 *					*
 *	- 5.1.2011-0.0/6:		*
 *		(Done) MenuMenuPush()	*
 *		(Done) MenuMenuPop()	*
 ****************************************/

#include <configmenu.h>

wchar_t *title;

typedef struct _MenuStackItem
{
	MenuItem *item;
	wchar_t *title;
	struct _MenuStackItem *previous;
	struct _MenuStackItem *next;
} MenuStackItem;

MenuStackItem *stack = 0;
MenuItem *menu = 0;
wchar_t *title = 0;

void MenuPush(MenuItem *item, wchar_t *newtitle)
{
	if(menu)
	{
		if(stack)
		{
			stack->next = malloc(sizeof(MenuStackItem));
			stack->next->previous = stack;
			stack = stack->next;
		}
		else
		{
			stack = malloc(sizeof(MenuStackItem));
			stack->item = menu;
			stack->previous = 0;
		}

		stack->item = menu;
		stack->next = 0;
		stack->title = title;
	}
	
	menu = item;
	title = newtitle;
}

void MenuPop()
{
	if(menu)
		MenuFree(menu);

	if(stack)
	{
		menu = stack->item;
		title = stack->title;
		if(stack->previous)
		{
			stack = stack->previous;
			free(stack->next);
			stack->next = 0;
		}
		else
		{
			free(stack);
			stack = 0;
		}
	}
	else
		menu = 0;
}

void Menu()
{
	while(menu)
	{
		MenuItem *item = menu;

		unsigned int select = 0;
	
		unsigned int loop = 0;

		wprintf(L"### %ls ###\n\n", title);

		for(loop = 0; item; loop++)
		{
			wprintf(L"#%u. %ls\n", loop + 1, item->item);
			item = item->next;
		}

		while(!select || select > loop)
		{
			wprintf(L"\nSelect item: ");
			if(scanf("%u", &select));
		}

		select--;

		item = menu;

		for(loop = 0; loop < select; loop++)
			item = item->next;
	
		item->function();
	}
}

