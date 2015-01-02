#ifndef JOB_POOL_H
#define JOB_POOL_H

#include <pthread.h>
#include "file_manage.h"

#define MAX_JOBS 100
//#define CPU_WL 3
//#define MIC_WL 11

#define CPU_CORE 24
#define MIC_CORE 240

#define NO_JOB 0
#define GET_JOB_ERROR -1
#define GOT_JOB 1

struct job_pool
{
    pthread_mutex_t get_job_lock;
    int lig_arr[MAX_JOBS];
    int cpu_ptr;
    int mic_ptr;
    int job_num;
};

//int job, type t, const char* wk_path
struct para {
    int job;
    type t;
    char* wk_path;
};

int get_job(struct job_pool *jp, type t);
void* do_job(void* arg);
void job_pool_init(int rank, int node_num, int total_lig, struct job_pool *jp);
void job_para_init(int job, type t, const char* wk_path, struct para* p);

#endif