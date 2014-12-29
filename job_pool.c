#include <job_pool.h>


void job_pool_init(struct job_pool *jp, int job_num, int arr[])
{
	int i;
	pthread_mutex_init(&(jp->get_job_lock), NULL);
	jp->job_num = job_num;
	jp->cpu_ptr = 0;
	jp->mic_ptr = job_num - 1;
	for(i = 0; i < job_num; i++)
		jp->lig_arr[i] = arr[i];
}

int get_job(struct job_pool *jp, type t)
{
	int rst;
	pthread_mutex_lock(&(jp->get_job_lock));
	if (jp->cpu_ptr == mic_ptr)
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

void do_job(int job, type t, const char* wk_path)
{
	char cmd[MAX_CMD_LEN]; 
	if (t == CPU)
	{
		strcpy(cmd, "cd ");
		strcat(cmd, wk_path);
		strcat(cmd, "; ");
	} else {
		strcpy(cmd, "ssh mic0 \"");
		strcat(cmd, "cd ");
		strcat(cmd, wk_path);
		strcat(cmd, "; ");

		strcat(cmd, "\"");
	}
	"./autodock vina --config"
}