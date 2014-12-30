#ifndef TASK_ALLOC_H
#define TASK_ALLOC_H

#include <file_mange.h>



#define MAX_JOBS 10

struct job_pool
{
	int lig_arr[MAX_JOBS];
	int fetch_ptr;
	int total_job;
};

#endif