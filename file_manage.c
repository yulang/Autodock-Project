#include <file_manage.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void abspath_gen(char* path, char* filename, char* abs_path);

void abspath_gen(char* path, char* filename, char* abs_path) 
{
	strcpy(abs_path, path);
	strcat(abs_path, filename);
}

void write_conf(char* path, char* lig_name, char* rcp_name, int thread_num, int index, int is_mic) 
{
	FILE* f_hdl;
	char* fwstream;
	int len = strlen(path) + strlen("conf_") + (index/10) + strlen(".txt") + 1;
	char *conf_path = (char*) malloc(len), *tmp = (char*) malloc(MAX_INDEX + 1);
	itoa(index, tmp, 10);
	strcpy(conf_path, "conf_");
	strcat(conf_path, tmp);

}