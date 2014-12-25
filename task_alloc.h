#ifndef TASK_ALLOC_H
#define TASK_ALLOC_H

#include <file_mange.h>

typedef int type
#define CPU 0
#define MIC 1

#define MAX_JOBS 10

struct job_pool
{
	int lig_arr[MAX_JOBS];
	int wk_ptr;
	int job_num;
};

#endif