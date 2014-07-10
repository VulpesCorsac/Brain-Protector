/****************************************
 * 					*
 *	Brain Protector Project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/************************************************
 *	train.c					*
 *	Neural Network Training code		*
 *						*
 *	Changelog:				*
 *	- Date-version/build:			*
 *		(State) Change.			*
 *						*
 *	- 3.1.2011-0.0/1:			*
 *		(Work) Train()			*
 *		(Done) TrainDataCallback()	*
 ************************************************/

#include <config.h>
#include <stdlib.h>
#include <fann.h>

#include <stdio.h>
#include <wchar.h>

static ConfigTrainFile *current = 0;
static char *buffer = 0;
unsigned int sector = 0;

static void TrainDataCallback(unsigned int, unsigned int, unsigned int, fann_type *, fann_type *);

int main(int argc, char *argv[])
{
	ConfigNetwork *config = 0;

	wprintf(L" * Loading config... ");

	if(!(config = ConfigLoad()))
	{
		wprintf(L"[ Fail ]\n");

		return -1;
	}

	wprintf(L"[ OK ]\n");

	struct fann_train_data *train_data = 0;

	current = config->first;

	struct fann *network = fann_create_shortcut(2, 4096, 1);

	fann_set_train_stop_function(network, FANN_STOPFUNC_MSE);

	while(config->trains)
	{
		train_data = fann_create_train_from_callback((config->trains > 128)?128:config->trains, config->input, config->output, TrainDataCallback);

		config->trains = (config->trains > 128)?config->trains-128:0;

		wprintf(L"Training...\n");

		fann_cascadetrain_on_data(network, train_data, 8192, 128, fann_get_MSE(network));

		fann_destroy_train(train_data);
	}

	free(buffer);
	
	fann_save(network, "network.net");

	return 0;
}

static void TrainDataCallback(unsigned int iteration, unsigned int input_number, unsigned int output_number, fann_type *input, fann_type *output)
{
	FILE *file = 0;

	if(!sector || sector > (current->size / 512) + (current->size % 512)?1:0)
	{
		sector = 0;
		current = current->next;
		buffer = realloc(buffer, current->size);

		wprintf(L"File: %s\n", current->name);

		if(!(file = fopen(current->name, "r")))
		{
			fwprintf(stderr, L"Can't open '%s'!\n", current->name);
			return;
		}

		if(fread(buffer, 1, current->size, file) != current->size)
		{
			fwprintf(stderr, L"Can't read '%s'!\n", current->name);
			return;
		}

		fclose(file);
	}

	for(unsigned int loop = 0; loop < 512 && 512 * sector + loop < current->size; loop++)
	{
		input[loop*8] = (buffer[512 * sector + loop] & 0x80)?1.0f:-1.0f;
		input[loop*8 + 1] = (buffer[512 * sector + loop] & 0x40)?1.0f:-1.0f;
		input[loop*8 + 2] = (buffer[512 * sector + loop] & 0x20)?1.0f:-1.0f;
		input[loop*8 + 3] = (buffer[512 * sector + loop] & 0x10)?1.0f:-1.0f;
		input[loop*8 + 4] = (buffer[512 * sector + loop] & 0x08)?1.0f:-1.0f;
		input[loop*8 + 5] = (buffer[512 * sector + loop] & 0x04)?1.0f:-1.0f;
		input[loop*8 + 6] = (buffer[512 * sector + loop] & 0x02)?1.0f:-1.0f;
		input[loop*8 + 7] = (buffer[512 * sector + loop] & 0x01)?1.0f:-1.0f;
	}

	*output = current->output;

	sector++;
}
