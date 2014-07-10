/****************************************
 * 					*
 *	Brain Protector project		*
 *	(C) Copyright 2010-2011		*
 *	Aloyan George,			*
 *	Derbenew Alexander.		*
 *					*
 ****************************************/

/********************************************************
 *	config.h					*
 *	Structures of configuration file,		*
 *	Loading configuration.				*
 *							*
 *	Changelog:					*
 *	- Date-version/build:				*
 *		(State) Change.				*
 *							*
 *	- 2.1.2011-0.0/0:				*
 *		(Done) ConfigMain			*
 *		(Done) ConfigNetwork			*
 *		(Done) ConfigLoad()			*
 *		       ^^^^^^^^^^^^ config.c		*
 *							*
 *	- 3.1.3011-0.0/1:				*
 *		(Done) ConfigTrainFile			*
 *		(Optimized) ConfigLoad()		*
 *			    ^^^^^^^^^^^^ config.c	*
 *		(Done) ConfigSave()			*
 *		       ^^^^^^^^^^^^ config.c		*
 ********************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

	#include <stdlib.h>

	typedef struct _ConfigNetwork
	{
		unsigned int input;
		unsigned int output;
		unsigned int files;
		unsigned int trains;
		struct _ConfigTrainFile *first;
	} ConfigNetwork;

	typedef struct _ConfigTrainFile
	{
		size_t size;
		char *name;
		unsigned int first;
		unsigned int last;
		float output;
		struct _ConfigTrainFile *next;
	} ConfigTrainFile;

	ConfigNetwork *ConfigLoad();
	int ConfigSave(ConfigNetwork *);

#endif

