/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/****************************************
 *	configedit.c			*
 *	Editing Config code		*
 *					*
 *	Changelog:			*
 *	- Date-version/build:		*
 *		(State) Change.		*
 *					*
 *	- 5.1.2011-0.0/3:		*
 *		(Start) main()		*
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>
#include <errno.h>

#include <config.h>
#include <configmenuitem.h>
#include <configmenu.h>

ConfigNetwork *config = 0;

typedef struct _DirQueue
{
	char *path;
	struct _DirQueue *next;
} DirQueue;

DirQueue *queue = 0;

void Enqueue(char *path)
{
	DirQueue *current = queue;

	if(queue)
	{
		while(current->next)
			current = current->next;

		current->next = malloc(sizeof(DirQueue));
		current = current->next;

		current->path = path;
		current->next = 0;
	}
	else
	{
		queue = malloc(sizeof(DirQueue));
		
		queue->path = path;
		queue->next = 0;
	}
}

char * Dequeue()
{
	DirQueue *current = queue;
	char *path = 0;

	if(!queue)
		return 0;
	
	queue = queue->next;

	path = current->path;
	free(current);
	
	return path;
}

WIN32_FIND_DATA *SearchFile(char *path)
{
	static HANDLE search = INVALID_HANDLE_VALUE;
	char *mask = 0;
	static WIN32_FIND_DATA find_data;

	if(search == INVALID_HANDLE_VALUE)
	{
		mask = realloc(mask, strlen(path) + 3);
		memcpy(mask, path, strlen(path));
		mask[strlen(path)] = '\\';
		mask[strlen(path) + 1] = '*';
		mask[strlen(path) + 2] = 0;

		if(search)
			FindClose(search);

		search = FindFirstFile(mask, &find_data);

		if(search == INVALID_HANDLE_VALUE)
		{
			free(mask);
			return 0;
		}


		free(mask);
	}
	else if(!FindNextFile(search, &find_data))
	{
		search = INVALID_HANDLE_VALUE;
		return 0;
	}

	return &find_data;
}

void AppendFiles(char *dir, float output)
{
	Enqueue(dir);
	struct dirent *entry = 0;
	ConfigTrainFile *current = 0;

	if(config->first)
	{
		current = config->first;

		while(current->next)
			current = current->next;
	}
	
	char *path = 0;
	
	WIN32_FIND_DATA * find_data = 0;
	
	while((path = Dequeue()))
	{
		while(find_data = SearchFile(path))
		{
			if(!(find_data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char *newpath = malloc(strlen(path) + strlen(find_data->cFileName) + 2);
				memcpy(newpath, path, strlen(path));
				newpath[strlen(path)] = '\\';
				memcpy(newpath + strlen(path) + 1, find_data->cFileName, strlen(find_data->cFileName) + 1);
				printf("Adding file '%s'...\n", newpath);
				if(config->first)
				{
					current->next = malloc(sizeof(ConfigTrainFile));
					current = current->next;
				}
				else
				{
					config->first = current = malloc(sizeof(ConfigTrainFile));
				}

				current->next = 0;
				current->name = newpath;
				current->output = output;
				config->files++;
				
				FILE *file = fopen(newpath, "r");
				
				fseek(file, 0, SEEK_END);
				current->size = ftell(file);
				fclose(file);

				config->trains += (current->size / 512) + ((current->size % 512)?1:0);
			}
			else if(find_data->cFileName[0] != '.')
			{
					
				char *newpath = malloc(strlen(path) + strlen(find_data->cFileName) + 2);
				memcpy(newpath, path, strlen(path));
				newpath[strlen(path)] = '/';
				memcpy(newpath + strlen(path) + 1, find_data->cFileName, strlen(find_data->cFileName) + 1);
				Enqueue(newpath);
			}
			else
				printf("%s\n", find_data->cFileName);
		}

		free(path);
	}
}

void EditConfigNetworkTrainGood()
{
	char *dir = malloc(256);

	wprintf(L"Set dirrectory with good train files (Up-to 255 symbols): ");

	while(!scanf("%s", dir));
	
	AppendFiles(dir, -1.0f);
}

void EditConfigNetworkTrainBad()
{
	char *dir = malloc(256);
	wprintf(L"Set dirrectory width bad train files (Up-to 255 symbols: ");

	while(!scanf("%s", dir));

	AppendFiles(dir, 1.0f);
}

void EditConfigNetworkSetInput()
{
	wprintf(L"Set input neurons number (Current: %u): ", config->input);
	while(!scanf("%u", &(config->input)));
}

void EditConfigNetworkSetOutput()
{
	wprintf(L"Set output neurons number (Current: %u): ", config->output);
	while(!scanf("%u", &(config->output)));
}

void EditConfigNetworkTrainShuffle()
{
	ConfigTrainFile *first = config->first;
	ConfigTrainFile *last = first;
	ConfigTrainFile *new = 0;

	wprintf(L"Shuffle train files (Run more times to good effect)...\n");

	while(first)
	{
		if(first->next && rand()%2)
		{
			last = first;

			while(last->next && last->next->next)
				last = last->next;

			if(!new)
				config->first = new = last->next;
			else
				new = new->next = last->next;

			last->next = 0;
		}
		else
		{
			if(!new)
				config->first = new = first;
			else
				new = new->next = first;

			first = first->next;

			new->next = 0;
		}
	}
}

void EditConfigNetwork()
{
	MenuItem *menu_network = ItemCreate(L"Set input neurons number", EditConfigNetworkSetInput);
	MenuAppend(menu_network, ItemCreate(L"Set output neurons number", EditConfigNetworkSetOutput));
	MenuAppend(menu_network, ItemCreate(L"Append bad train files", EditConfigNetworkTrainBad));
	MenuAppend(menu_network, ItemCreate(L"Append good train files", EditConfigNetworkTrainGood));
	MenuAppend(menu_network, ItemCreate(L"Shuffle train files", EditConfigNetworkTrainShuffle));
	MenuAppend(menu_network, ItemCreate(L"Back", MenuPop));

	MenuPush(menu_network, L"Network Config");
}

void ConfigWrite()
{
	wprintf(L"Saving config... ");

	ConfigSave(config);

	wprintf(L"[ OK ]\n");

	MenuPop();
}

int main(int argc, char *argv[])
{
	ConfigNetwork *config_old = 0;

	wprintf(L" * Loading config file... ");

	config_old = ConfigLoad();

	if(config_old)
	{
		wprintf(L"[ OK ]\n");
	}
	else
	{
		wprintf(L"[ Fail! ]\n * Creating new config... [ OK ]\n");

		config = malloc(sizeof(ConfigNetwork));

		config->input = config->output = config->files = config->trains = 0;
		config->first = 0;
	}

	MenuItem * menu_main = ItemCreate(L"Network", EditConfigNetwork);
	MenuAppend(menu_main, ItemCreate(L"Write and exit", ConfigWrite));
	MenuAppend(menu_main, ItemCreate(L"Exit", MenuPop));

	MenuPush(menu_main, L"Main config");

	Menu();
	
	return 0;
}

