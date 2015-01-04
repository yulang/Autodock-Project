#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H


#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "job_pool.h"

#define MAX_INDEX 10
#define MAX_FILENAME 20
#define MAX_PATH 200
#define MAX_CONF_SIZE 500
#define MAX_CMD_LEN 200
#define MAX_JOBS 100

typedef int type;
#define CPU 0
#define MIC 1

#define itoa(in, s, rad) sprintf(s, "%d", in)

#define CONF 0
#define LIG 1

struct conf {
	//char lig[MAX_FILENAME];
	char rcp[MAX_FILENAME];
	char outfile[MAX_FILENAME];
	double cent[3];
	double size[3];
	int exht;
	int cpu;
};

extern char *lig_dic[MAX_JOBS];
int write_conf(const char* path, struct conf* cf, int index);
int setup(const char* lig_lib, const char* rcp_loc, const char* home_path, int index, type t);
void gen_filename(char* filename, int index, int file_type);
void conf_parser(struct conf* cf, const char* in_conf, const char* rcp);
void traverse(const char* lig_lib);
void get_workpath(const char* home_path, int index, char* work_path);

#endif