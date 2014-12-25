#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#define MAX_INDEX 5
#define MAX_FILENAME 20
#define MAX_PATH 200
#define MAX_CONF_SIZE 500
#define MAX_CMD_LEN 200

typedef int type
#define CPU 0
#define MIC 1

#include <log.h>
#include <stdlib.h>

struct conf {
	char lig[MAX_FILENAME];
	char rcp[MAX_FILENAME];
	char outfile[MAX_FILENAME];
	int cent[3];
	int size[3];
	int exht;
};

int write_conf(char* path, struct conf cf, int index);

#endif