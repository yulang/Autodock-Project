#include <file_manage.h>
#include <sys/stat.h>
#include <string.h>

char* gen_filename(int index)
{
	char fname[MAX_FILENAME], tmp[MAX_INDEX];

	strcat(fname, "specs_");
	itoa(index, tmp, 10);
	strcat(fname, tmp);
	strcat(fname, ".pdbqt");

	return fname;
}

int gen_conf(char* conf_file, struct conf cf, int index)
{
	char tmp[MAX_INDEX];

	if (cf.lig == NULL || cf.rcp == NULL)
	{
		print("Invalide pairs\n");
		return -1;
	}

	strcpy(conf_file, "receptor = ");
	strcat(conf_file, cf.rcp);
	strcat(conf_file, "\nligand = ");
	strcat(conf_file, cf.lig);
	strcat(conf_file, "\n\nout = ");
	if (cf.outfile != NULL)
	{
		strcat(conf_file, cf.outfile);
	} else {
		strcat(conf_file, "output_");
		itoa(index, tmp, 10);
		strcat(conf_file, tmp);
	}
	strcat(conf_file, "\n\ncenter_x = ");
	if (cf.cent != NULL)
	{
		itoa(tmp, cf.cent[0]);
		strcat(conf_file, tmp);
		strcat(conf_file, "cent_y = ");
		itoa(tmp, cf.cent[1]);
		strcat(conf_file, tmp);
		strcat(conf_file, "cent_z = ");
		itoa(tmp, cf.cent[2]);
		strcat(conf_file, tmp);
	} else {
		strcat(conf_file, "11\ncenter_y = 90.5\ncenter_z = 57.5");
	}

	strcat(conf_file, "\n\n");

	if (cf.size != NULL)
	{
		itoa(tmp, cf.size[0]);
		strcat(conf_file, tmp);
		strcat(conf_file, "cent_y = ");
		itoa(tmp, cf.size[1]);
		strcat(conf_file, tmp);
		strcat(conf_file, "cent_z = ");
		itoa(tmp, cf.size[2]);
		strcat(conf_file, tmp);
	} else {
		strcat(conf_file, "11\ncenter_y = 90.5\ncenter_z = 57.5");
	}
	itoa(cf.exht, tmp, 10);
	strcat(conf_file, "\n\nexhaustiveness = ");
	strcat(conf_file, tmp);
	return 0;

}

int write_conf(char* path, struct conf cf, int index) 
{
	FILE* f_hdl;
	char* fwstream = (char*) malloc(MAX_CONF_SIZE); //conf file write stream
	int len = strlen(path) + strlen("conf_") + MAX_INDEX + strlen(".txt") + 1;
	char *conf_path = (char*) malloc(len), *tmp = (char*) malloc(MAX_INDEX + 1);
	itoa(index, tmp, 10);
	strcpy(conf_path, path);
	strcat(conf_path, "conf_");
	strcat(conf_path, tmp);
	strcat(conf_path, ".txt");
	//gen_conf(char* conf_file, struct conf cf, int index)
	if (gen_conf(fwstream, cf, index))
	{
		print("Gen Config File Error\n");
		return -1;
	} 
	if (!(f_hdl = fopen(conf_path, "w"))
	{
		print("Write Config File Error\n");
		return -1;
	}

	fwrite(fwstream, sizeof(char), strlen(fwstream), f_hdl); 
	return 0;

}

int file_trans(const char* file_path, const char* dst_loc, type t)
{
	char cmd[MAX_CMD_LEN];
	if (t == CPU)
	{
		strcpy(cmd, "cp ");
		strcat(cmd, file_path);
		strcat(cmd, " ");
		strcat(cmd, dst_loc);
	} else {
		strcpy(cmd, "scp ");
		strcat(cmd, file_path);
		strcat(cmd, " root@mic0:");
		strcat(cmd, dst_loc);
	}
	return system(cmd);
}

int setup(const char* lig_lib, const char* rcp_loc, const char* work_path, int index, type t, struct conf cf) 
{
	//get all pair files and conf file ready in individual folder
	char cmd[MAX_CMD_LEN], lig[MAX_PATH], cindex[MAX_INDEX];
	if (t == CPU)
	{
		strcpy(cmd, "mkdir ");
		strcat(cmd, work_path);
		strcat(cmd, "work_");
		itoa(index, cindex, 10);
		strcat(cmd, cindex);
	} else {
		strcpy(cmd, "ssh mic0 ");
		strcat(cmd, "\"mkdir ");
		strcat(cmd, work_path);
		strcat(cmd, "\"");
	}
	if (!system(cmd)) 
	{
		print("Work Path Setup Error\n");
		return -1;
	}

//file_trans(const char* file_path, const char* dst_loc, type t)
	strcpy(lig, lig_lib);
	strcat(lig, gen_filename(index));
	
	if (file_trans(lig, work_path, t))
	{
		print("File Trans Error\n");
		return -1;
	}

	if (file_trans(rcp_loc, work_path, t))
	{
		print("File Trans Error\n");
		return -1;
	}

//int write_conf(char* path, struct conf cf, int index) 
	if (write_conf(work_path, cf, index))
	{
		print("Setup Error\n");
		return -1;
	}
	return 0;
}