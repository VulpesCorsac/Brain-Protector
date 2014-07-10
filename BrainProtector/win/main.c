#include <fann.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define OFFSET	4

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		wprintf(L"No files to scan!\n");

		return 0;
	}

	wprintf(L"Loading neural network...");

	struct fann *network = fann_create_from_file("network.net");

	if(!network)
	{
		wprintf(L"[ Fail ]\n");

		return -1;
	}

	wprintf(L"[ OK ]\n");

	for(unsigned int loop = 1; loop < argc; loop++)
	{
		FILE *file = 0;
		unsigned int size = 0;

		wprintf(L"File: %s\n", argv[loop]);
		
		if(!(file = fopen(argv[loop], "rb")))
		{
			fwprintf(stderr, L"Can't open file '%s'!\n", argv[loop]);

			continue;
		}

		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		wprintf(L" - Size: %u\n", size);

		char *buffer = malloc(size);

		if(fread(buffer, 1, size, file) != size)
		{
			fwprintf(stderr, L"Can't read from file!\n");

			free(buffer);

			fclose(file);

			continue;
		}

		fclose(file);

		fann_type *scan_data = malloc(sizeof(fann_type) * 4096);

		float detection = 0.0f;

		for(unsigned int offset = 0; offset < size; offset += OFFSET)
		{
			for(unsigned int counter = 0; counter < 512; counter++)
			{
				if(offset + counter < size)
				{
					scan_data[counter * 8] = (buffer[offset + counter] & 0x80)?1.0f:-1.0f;
					scan_data[counter * 8 + 1] = (buffer[offset + counter] & 0x40)?1.0f:-1.0f;
					scan_data[counter * 8 + 2] = (buffer[offset + counter] & 0x20)?1.0f:-1.0f;
					scan_data[counter * 8 + 3] = (buffer[offset + counter] & 0x10)?1.0f:-1.0f;
					scan_data[counter * 8 + 4] = (buffer[offset + counter] & 0x08)?1.0f:-1.0f;
					scan_data[counter * 8 + 5] = (buffer[offset + counter] & 0x04)?1.0f:-1.0f;
					scan_data[counter * 8 + 6] = (buffer[offset + counter] & 0x02)?1.0f:-1.0f;
					scan_data[counter * 8 + 7] = (buffer[offset + counter] & 0x01)?1.0f:-1.0f;
				}
				else
				{
					scan_data[counter * 8] = 0.0f;
					scan_data[counter * 8 + 1] = 0.0f;
					scan_data[counter * 8 + 2] = 0.0f;
					scan_data[counter * 8 + 3] = 0.0f;
					scan_data[counter * 8 + 4] = 0.0f;
					scan_data[counter * 8 + 5] = 0.0f;
					scan_data[counter * 8 + 6] = 0.0f;
					scan_data[counter * 8 + 7] = 0.0f;
				}
			}

			fann_type *output = fann_run(network, scan_data);

			if(*output > 0.0f)
			{
				detection += *output;
			}
		}

		free(buffer);

		free(scan_data);

		if(detection)
			wprintf(L" ^****DETECTION: %.2f%%\n", detection / (size/4) * 100);
		else
			wprintf(L" ^****Clean!\n");
	}
	
	return 0;
}

