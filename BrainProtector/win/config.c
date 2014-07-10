/****************************************
 *					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George			*
 *	Derbenew Alexander		*
 *					*
 ****************************************/

/************************************************
 *	config.c				*
 *	Loading configuration data		*
 *						*
 *	Changelog:				*
 *	- Date-version/build:			*
 *		(State) Change.			*
 *						*
 *	- 2.1.2011-0.0/0:			*
 *		(Done) ConfigLoad()		*
 *						*
 *	- 3.1.2011-0.0/1:			*
 *		(Optimized) ConfigLoad()	*
 *		(Done) ConfigSave()		*
 ************************************************/

#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

ConfigNetwork * ConfigLoad()
{
	FILE * config_file;
	ConfigNetwork *config = 0;
	ConfigTrainFile *current = 0;

	if(!(config_file = fopen("bp.cfg", "r")))
	{
		fwprintf(stderr, L"Can't open config!\n");

		return 0;
	}

	config = malloc(sizeof(ConfigNetwork));

	if(fscanf(config_file, "%u %u %u %u", &(config->input), &(config->output), &(config->files), &(config->trains)) != 4)
	{
		fwprintf(stderr, L"Can't load config!\n");
		free(config);
		return 0;
	}

	if(config->files)
		current = config->first = malloc(sizeof(ConfigTrainFile));

	for(unsigned int loop = 0; loop < config->files; loop++)
	{
		current->next = 0;
		current->name = malloc(1024);

		if(fscanf(config_file, "%s %u %f", current->name, &(current->size), &(current->output)) != 3)
		{
			fwprintf(stderr, L"Invalid config!\n");

			current = config->first;

			while(current->next)
			{
				config->first = current->next;
				free(current->name);
				free(current);
				current = config->first;
			}

			free(current->name);
			free(current);
			free(config);
			return 0;
		}
		
		if(loop + 1 < config->files)
		{
			current->next = malloc(sizeof(ConfigTrainFile));
			current = current->next;
		}
	}

	fclose(config_file);

	return config;
}

int ConfigSave(ConfigNetwork *config)
{
	ConfigTrainFile *current = config->first;

	FILE *config_file = fopen("bp.cfg", "w");
	if(!config_file)
	{
		fwprintf(stderr, L"Can't open config!\n");

		return -1;
	}

	fprintf(config_file, "%u %u %u %u\n",
		config->input, config->output, config->files, config->trains);

	while(current)
	{
		fprintf(config_file, "%s %u %f\n",
			current->name, current->size, current->output);

		current = current->next;
	}

	fclose(config_file);

	return 0;
}

