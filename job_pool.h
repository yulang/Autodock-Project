#ifndef JOB_POOL_H
#define JOB_POOL_H

#include <pthread.h>
#include <file_manage.h>

struct job_pool
{
	pthread_mutex_t get_job_lock;
	int lig_arr[MAX_JOBS];
	int cpu_ptr;
	int mic_ptr;
	int job_num;
};

#define MAX_JOBS 100
#define CPU_WL 3
#define MIC_WL 11

#define NO_JOB 0
#define GET_JOB_ERROR -1
#define GOT_JOB 1

int get_job(struct job_pool *jp, type t);

#endif