#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#define MAX_INDEX 5
#define MAX_FILENAME 20
#define MAX_PATH 200
#define MAX_CONF_SIZE 500
#define MAX_CMD_LEN 200

#define itoa(in, s, rad) sprintf(s, "%d", in)

typedef int type;
#define CPU 0
#define MIC 1

#define CONF 0
#define LIG 1

#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include "task_alloc.h"

struct conf {
	char lig[MAX_FILENAME];
	char rcp[MAX_FILENAME];
	char outfile[MAX_FILENAME];
	int cent[3];
	int size[3];
	int exht;
	int cpu;
};

int write_conf(const char* path, struct conf cf, int index) ;
int setup(const char* lig_lib, const char* rcp_loc, const char* work_path, int index, type t, struct conf cf);
void gen_filename(char* filename, int index, int file_type);

#endif