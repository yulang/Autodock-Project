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
	int rst;
	pthread_mutex_lock(&(jp->get_job_lock));
	if (jp->cpu_ptr == jp->mic_ptr)
	{
		return NO_JOB;
	}
	if (t == CPU)
	{
		rst = (jp->cpu_ptr)++;
	} else {
		rst = (jp->mic_ptr)--;
	}
	pthread_mutex_unlock(&(jp->get_job_lock));
	return rst;
}

void* do_job(void* arg)
{
    //won't return until the vina job has been done
	char cmd[MAX_CMD_LEN];
	char *conf;
    struct para* job_para = (struct para*) arg;

    gen_filename(conf, job_para->job, CONF);
	if (job_para->t == CPU)
	{
		strcpy(cmd, "cd ");
		strcat(cmd, job_para->wk_path);
		strcat(cmd, "; ");
		strcat(cmd, "./autodock vina --config ");
        strcat(cmd, conf);
	} else {
		strcpy(cmd, "ssh mic0 \"");
		strcat(cmd, "cd ");
		strcat(cmd, job_para->wk_path);
		strcat(cmd, "; ");
		strcat(cmd, "./autodock vina --config ");
        strcat(cmd, conf);
		strcat(cmd, "\"");
	}
    system(cmd);
    free(conf);
    return NULL;
}

void job_para_init(int job, type t, const char* wk_path, struct para* p)
{
    p->job = job;
    p->t = t;
    strcpy(p->wk_path, wk_path);
}