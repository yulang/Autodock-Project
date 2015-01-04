#include "job_pool.h"

/*
void get_wkload(int rank, int node_num, int total_lig, int* offset, int* job_num)
{
    //rank begins from zero
    int tmp;
    tmp = total_lig / node_num;
    *offset = tmp * rank;
    if ((rank + 1) * tmp >= total_lig) {
        *job_num = total_lig - rank * tmp;
    } else {
        *job_num = tmp;
    }
}*/

//void job_pool_init(struct job_pool *jp, int job_num, int offset)
void job_pool_init(int rank, int node_num, int total_lig, struct job_pool *jp)
{
    //rank begins from zero
	int tmp, offset, job_num;
    
    tmp = total_lig / node_num;
    offset = tmp * rank;
    if ((rank + 1) * tmp >= total_lig) {
        job_num = total_lig - rank * tmp;
    } else {
        job_num = tmp;
    }
    
	pthread_mutex_init(&(jp->get_job_lock), NULL);
	jp->job_num = job_num;
	jp->cpu_ptr = offset;
	jp->mic_ptr = job_num + offset - 1;
	//for(i = 0; i < job_num; i++)
	//	jp->lig_arr[i] = arr[i];
}

int get_job(struct job_pool *jp, type t)
{
    //TODO
	int rst;
	pthread_mutex_lock(&(jp->get_job_lock));
	if (jp->cpu_ptr > jp->mic_ptr)
	{
		return NO_JOB;
	}
    
    if (jp->mic_ptr - jp->cpu_ptr < CMPT_RATIO * cpu_tnum) { //the number of jobs left is the minus of two pointer plus 1
        if (t == MIC) {
            return NO_JOB;
        } else {
            rst = (jp->cpu_ptr)++;
        }
    } else {
        if (t == CPU)
        {
            rst = (jp->cpu_ptr)++;
        } else {
            rst = (jp->mic_ptr)--;
        }
    }
    pthread_mutex_unlock(&(jp->get_job_lock));
	return rst;
}

void do_job(int job, type t, const char* work_path)
{
    //won't return until the vina job has been done
	char cmd[MAX_CMD_LEN];
	char *conf;

    gen_filename(conf, job, CONF);
	if (t == CPU)
	{
		strcpy(cmd, "cd ");
		strcat(cmd, work_path);
		strcat(cmd, "; ");
		strcat(cmd, "./autodock vina --config ");
        strcat(cmd, conf);
	} else {
		strcpy(cmd, "ssh mic0 \"");
		strcat(cmd, "cd ");
		strcat(cmd, work_path);
		strcat(cmd, "; ");
		strcat(cmd, "./autodock vina --config ");
        strcat(cmd, conf);
		strcat(cmd, "\"");
	}
    system(cmd);
    free(conf);
}

void job_para_init(struct job_pool* jp, type t, const char* home_path, const char* lig_lib, const char* rcp, struct para* p)
{
    p->jp = jp;
    p->t = t;
    strcpy(p->home_path, home_path);
    strcpy(p->lig_lib, lig_lib);
    strcpy(p->rcp, rcp);
}

void* vina_worker(void* arg)
{
    struct para* work_para = (struct para*) malloc(sizeof(struct para));
    memcpy(work_para, (struct para*)arg, sizeof(struct para));
    char work_path[MAX_PATH];
    int my_job;
    my_job = get_job(work_para->jp, work_para->t);
    while (my_job != NO_JOB) {
        get_workpath(work_para->home_path, my_job, work_path);
        setup(work_para->lig_lib, work_para->rcp, work_para->home_path, my_job, work_para->t);
        do_job(my_job, work_para->t, work_path);
    }
    free(work_para);
    return NULL;
}