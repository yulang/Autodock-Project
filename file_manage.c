#include <file_manage.h>
#include <sys/stat.h>
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
	int len = strlen(path) + (index/10 + 1) + 1;
	char* conf_path = (char*) malloc(len);



}