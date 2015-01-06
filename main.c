#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "job_pool.h"
#include "file_manage.h"

//#include "mpi.h"

//#define MPI_ON 0
#define ARG_NUM 7
#define MIC_NUM_THREADS 240
#define HOME_PATH "/tmp/vina/"

int cal_tnum(int core_num, int t_perjob);
void env_setup();

int cpu_tnum, mic_tnum;

int main(int argc, char const *argv[])
{
    /*
     * Argument: 1.vina 2.lig_path 3. lig_num 4.rcp_path 5.conf_path 6.out_path
     */
	int my_rank, p; //p is the total number of MPI process
	// MPI_Status status;
	char *vina_exe = argv[1], *lig_path = argv[2], *rcp_path = argv[4], *conf = argv[5], *out = argv[6];
    int lig_num = atoi(argv[3]);
    
    //!!!Job pool is shared by all the threads in one node
    struct job_pool* jp = (struct job_pool*) malloc(sizeof(struct job_pool));
    if (argc == ARG_NUM) {
        if (lig_path == NULL || rcp_path == NULL || conf == NULL || out == NULL) {
            print("%s\n", "Illegal argument!");
            exit(1);
        }
    } else {
        print("%s\n", "Illegal argument number!");
        exit(1);
    }
    
    int i;
    //!!!Conf is shared by all the threads in one node
    struct conf* cf = (struct conf*) malloc(sizeof(struct conf));   //conf structure in the program

    conf_parser(cf, conf, lig_path, rcp_path, vina_exe);
    job_pool_init(0, 1, lig_num, jp);
    traverse(lig_path);
    env_setup();
    printf("1");
    cpu_tnum = cal_tnum(CPU_CORE, cf->cpu);
    mic_tnum = cal_tnum(MIC_CORE, cf->cpu);
    //test("%s\n",lig_dic[3]);
    
    pthread_t c_tid[cpu_tnum], m_tid[mic_tnum];
    struct para* job_para = (struct para*) malloc(sizeof(struct para));
    
    job_para_init(jp, cf, CPU, HOME_PATH, job_para);
    vina_worker((void*)job_para);
    /*for(i = 0; i < cpu_tnum; i++) {
        // autodock vina tasks
        job_para_init(jp, cf, CPU, HOME_PATH, lig_path, rcp_path, vina_exe, job_para);
        pthread_create(&c_tid[i], NULL, vina_worker, (void*)job_para);
    }
    
    for (i = 0; i < mic_tnum; i++) {
        job_para_init(jp, cf, MIC, HOME_PATH, lig_path, rcp_path, vina_exe, job_para);
        pthread_create(&m_tid[i], NULL, vina_worker, (void*)job_para);
    }
    */
	// MPI_Init(&argc, &argv);
    

	// MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// MPI_Comm_size(MPI_COMM_WORLD, &p);

	//MPI_Send(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm)
	//MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm, MPI_Status *status)

	// MPI_Finalize();

	return 0;
}

int cal_tnum(int core_num, int t_perjob)
{
    int rst;
    if ( t_perjob >= core_num) {
        return 1;
    } else {
        if ((core_num%t_perjob) != 0) {
            rst = core_num/t_perjob + 1;
        } else {
            //divide ideally
            rst = core_num/t_perjob;
        }
    }
    return rst;
}

void env_setup()
{
    char cmd[MAX_CMD_LEN];
    
    strcpy(cmd, "mkdir ");
    strcat(cmd, HOME_PATH);
    if (system(cmd)) {
        exit(1);
    }
/*
    strcpy(cmd, "ssh mic0 ");
    strcat(cmd, "\"mkdir ");
    strcat(cmd, HOME_PATH);
    strcat(cmd, "\"");
    if (system(cmd)) {
        exit(1);
    }*/
    
}
