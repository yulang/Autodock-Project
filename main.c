#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "job_pool.h"
#include "file_manage.h"

//#include "mpi.h"

//#define MPI_ON 0
#define ARG_NUM 6
#define MIC_NUM_THREADS 240



int main(int argc, char const *argv[])
{
    /*
     * Argument: 1.lig_path 2. lig_num 3.rcp_path 4.conf_path 5.out_path
     */
	int my_rank, p; //p is the total number of MPI process
	// MPI_Status status;
	char *lig_path = argv[1], *rcp_path = argv[3], *conf = argv[4], *out = argv[5];
    int lig_num = atoi(argv[2]);
    struct job_pool jp;
    if (argc == ARG_NUM) {
        if (lig_path == NULL || rcp_path == NULL || conf == NULL || out == NULL) {
            print("%s\n", "Illegal argument!");
            exit(1);
        }
    } else {
        print("%s\n", "Illegal argument number!");
        exit(1);
    }
    
    struct conf* cf = (struct conf*) malloc(sizeof(struct conf));   //conf structure in the program
    
    conf_parser(cf, conf);
    job_pool_init(0, 1, lig_num, &jp);
    
    int cpu_tnum = (CPU_CORE%(cf->cpu))? CPU_CORE/cf->cpu + 1 :CPU_CORE/cf->cpu;
    int mic_tnum = (CPU_CORE%(cf->cpu))? CPU_CORE/cf->cpu + 1 :CPU_CORE/cf->cpu;
    
    pthread_t c_tid[cpu_tnum], m_tid[mic_tnum];
    struct para job_para;
    
    int i, my_job;
    for(i = 0; i < cpu_tnum; i++) {
        // autodock vina tasks
        job_para_init(<#int job#>, <#type t#>, <#const char *wk_path#>, <#struct para *p#>)
        pthread_create(&c_tid[i], NULL, do_job, <#void *restrict#>)
    }
    
	// MPI_Init(&argc, &argv);
    

	// MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	// MPI_Comm_size(MPI_COMM_WORLD, &p);

	//MPI_Send(void *buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm)
	//MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm, MPI_Status *status)

	// MPI_Finalize();

	return 0;
}