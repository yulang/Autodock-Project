#ifndef JOB_POOL_H
#define JOB_POOL_H

#include <pthread.h>
#include "file_manage.h"


//#define CPU_WL 3
//#define MIC_WL 11



#define CPU_CORE 24
#define MIC_CORE 240
#define CMPT_RATIO 2

#define NO_JOB -1
#define GET_JOB_ERROR -2

struct job_pool
{
    pthread_mutex_t get_job_lock;
    //int lig_arr[MAX_JOBS];
    int cpu_ptr;
    int mic_ptr;
    int job_num;
};

//int job, type t, const char* wk_path
struct para {
    struct job_pool* jp;
    type t;
    char* home_path;
    char lig_lib[MAX_PATH];
    char rcp[MAX_PATH];
};

int get_job(struct job_pool *jp, type t);
void do_job(int job, type t, const char* work_path);
void job_pool_init(int rank, int node_num, int total_lig, struct job_pool *jp);
void job_para_init(struct job_pool* jp, type t, const char* home_path, const char* lig_lib, const char* rcp, struct para* p);
void* vina_worker(void* arg);
extern int cpu_tnum, mic_tnum;

#endif