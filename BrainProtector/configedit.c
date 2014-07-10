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
#include <dirent.h>
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
	
	while((path = Dequeue()))
	{
		DIR *dir = opendir(path);

		while((entry = readdir(dir)))
		{
			if(entry->d_type == DT_REG)
			{
				char *newpath = malloc(strlen(path) + strlen(entry->d_name) + 2);
				memcpy(newpath, path, strlen(path));
				newpath[strlen(path)] = '/';
				memcpy(newpath + strlen(path) + 1, entry->d_name, strlen(entry->d_name) + 1);
				wprintf(L"Adding file '%s'...\n", newpath);
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
			else if(entry->d_type == DT_DIR && !((strlen(entry->d_name) == 1 || (strlen(entry->d_name) == 2 && entry->d_name[1] == '.')) && entry->d_name[0] == '.'))
			{
					
				char *newpath = malloc(strlen(path) + strlen(entry->d_name) + 2);
				memcpy(newpath, path, strlen(path));
				newpath[strlen(path)] = '/';
				memcpy(newpath + strlen(path) + 1, entry->d_name, strlen(entry->d_name) + 1);
				Enqueue(newpath);
			}
		}

		free(path);

		closedir(dir);
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

	wprintf(L"Shuffle train files (run more times to good effect)...\n");

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

