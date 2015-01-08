#include "file_manage.h"
#include <sys/stat.h>
#include <dirent.h>

void conf_init(struct conf* cf);
void get_workpath(const char* home_path, int index, char* work_path);

char *lig_dic[MAX_JOBS];

void gen_filename(char* filename, int index, int file_type)
{
	//char fname[MAX_FILENAME], tmp[MAX_INDEX];
	char tmp[MAX_INDEX];
	//filename = (char *)malloc(MAX_FILENAME);

	if (file_type == LIG)
	{
        strcpy(filename, lig_dic[index]);
        
	} else if (file_type == CONF) {
		strcpy(filename, "conf_");
		itoa(index, tmp, 10);
		strcat(filename, tmp);
		strcat(filename, ".txt");
	}
	
	return;
}

int gen_conf(char* conf_file, struct conf* cf, int index)
{
	char tmp[MAX_INDEX];
	
	if (cf->rcp == NULL)
	{
		print("Invalide pairs\n");
		return -1;
	}
   
    
	strcpy(conf_file, "receptor = ");
	//strcat(conf_file, cf->rcp);
    strcat(conf_file, cf->rcp_name);
	strcat(conf_file, "\nligand = ");
	//strcat(conf_file, cf.lig);
    strcat(conf_file, lig_dic[index]);
	strcat(conf_file, "\n\nout = ");
	if (strlen(cf->outfile) != 0)
	{
		strcat(conf_file, cf->outfile);
        strcat(conf_file, "_");
        itoa(index, tmp, 10);
        strcat(conf_file, tmp);
	} else {
		strcat(conf_file, "output_");
		itoa(index, tmp, 10);
		strcat(conf_file, tmp);
		strcpy(cf->outfile, "output");
	}
	strcat(conf_file, "\n\ncenter_x = ");
	if (cf->cent != NULL)
	{
		//itoa(cf.cent[0], tmp, 10);
        sprintf(tmp, "%lf", cf->cent[0]);
		strcat(conf_file, tmp);
		strcat(conf_file, "\ncenter_y = ");
		//itoa(cf.cent[1], tmp, 10);
        sprintf(tmp, "%lf", cf->cent[1]);
		strcat(conf_file, tmp);
		strcat(conf_file, "\ncenter_z = ");
		//itoa(cf.cent[2], tmp, 10);
        sprintf(tmp, "%lf", cf->cent[2]);
		strcat(conf_file, tmp);
	} else {
		strcat(conf_file, "11\ncenter_y = 90.5\ncenter_z = 57.5");
	}

	strcat(conf_file, "\n\nsize_x = ");

	if (cf->size != NULL)
	{
		//itoa(cf.size[0], tmp, 10);
        sprintf(tmp, "%lf", cf->size[0]);
		strcat(conf_file, tmp);
		strcat(conf_file, "\nsize_y = ");
		//itoa(cf.size[1], tmp, 10);
        sprintf(tmp, "%lf", cf->cent[1]);
		strcat(conf_file, tmp);
		strcat(conf_file, "\nsize_z = ");
		//itoa(cf.size[2], tmp, 10);
        sprintf(tmp, "%lf", cf->cent[2]);
		strcat(conf_file, tmp);
	} else {
		strcat(conf_file, "11\nsize_y = 90.5\nsize_z = 57.5");
	}
	itoa(cf->exht, tmp, 10);
	strcat(conf_file, "\n\nexhaustiveness = ");
	strcat(conf_file, tmp);

	itoa(cf->cpu, tmp, 10);
	strcat(conf_file, "\n\ncpu = ");
	strcat(conf_file, tmp);
    strcat(conf_file, "\n");
	return 0;

}

int write_conf(const char* path, struct conf* cf, int index)
{
	FILE* f_hdl;
	char* fwstream = (char*) malloc(MAX_CONF_SIZE); //conf file write stream
	int len = strlen(path) + strlen("conf_") + MAX_INDEX + strlen(".txt") + 1;
	char *conf_path = (char*) malloc(len), tmp[MAX_INDEX+1];
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
	if (!(f_hdl = fopen(conf_path, "w")))
	{
		print("Write Config File Error\n");
		return -1;
	}

	fwrite(fwstream, sizeof(char), strlen(fwstream), f_hdl);
    fclose(f_hdl);
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

int setup(struct conf* cf, const char* home_path, int index, type t)
{
	//get all pair files and conf file ready in individual folder
	char cmd[MAX_CMD_LEN], tmp[MAX_PATH], cindex[MAX_INDEX], work_path[MAX_PATH];
	char filename[MAX_FILENAME];
	if (t == CPU)
	{
		strcpy(cmd, "mkdir ");
		strcat(cmd, home_path);
		//strcat(cmd, "work_");
		itoa(index, cindex, 10);
		strcat(cmd, cindex);
	} else {
		strcpy(cmd, "ssh mic0 ");
		strcat(cmd, "\"mkdir ");
		strcat(cmd, home_path);
        itoa(index, cindex, 10);
        strcat(cmd, cindex);
		strcat(cmd, "\"");
	}
	if (system(cmd))
	{
		print("Work Path Setup Error\n");
		return -1;
	}
    

//file_trans(const char* file_path, const char* dst_loc, type t)
    
    /********************SEND LIG & RCP FILE***************************************/
	strcpy(tmp, cf->lig_lib);
	gen_filename(filename, index, LIG);
	strcat(tmp, filename);
	//free(filename);
	get_workpath(home_path, index, work_path);
	//if (file_trans(tmp, work_path, t) && file_trans(rcp_loc, work_path, t)) //BUG!
    if (file_trans(tmp, work_path, t) || file_trans(cf->rcp, work_path, t))
	{
		print("File Trans Error\n");
		return -1;
	}
    
    /*******************SEND CONF FILE***************************************/
    write_conf(home_path, cf, index);
    gen_filename(filename, index, CONF);
    strcpy(tmp, home_path);
    strcat(tmp, filename);
    if (file_trans(tmp, work_path, t)) {
        print("File Trans Error\n");
        return -1;
    }
    remove(tmp);
    
    /*******************SEND VINA*******************************************/
    if (t == CPU) {
        if (file_trans(cf->vina, work_path, t)) {
            print("File Trans Error\n");
            return -1;
        }
    } else {
        if (file_trans(cf->vina_mic, work_path, t)) {
            print("File Trans Error\n");
            return -1;
        }
    }
    
    return 0;
}

void cleanup(const char* work_folder, const char* rst_gather, type t, const char* out_name, int job)
{
    char cmd[MAX_CMD_LEN], cindex[MAX_INDEX];
    if (t == CPU) {
        strcpy(cmd, "cp ");
        strcat(cmd, work_folder);
	strcat(cmd, "/");
        strcat(cmd, out_name);
        strcat(cmd, "_");
        itoa(job, cindex, 10);
        strcat(cmd, cindex);
        strcat(cmd, " ");
        strcat(cmd, rst_gather);
        if (system(cmd)) {
            print("Clean up error");
            exit(1);
        }
        //remove(work_folder);
	strcpy(cmd, "rm -rf ");
	strcat(cmd, work_folder);
	if (system(cmd)) {
		print("Clean up error");
		exit(1);
	}
    } else {
        strcpy(cmd, "scp ");
        strcat(cmd, MIC_NAME);
        strcat(cmd, ":");
        strcat(cmd, work_folder);
	strcat(cmd, "/");
        strcat(cmd, out_name);
        strcat(cmd, "_");
        itoa(job, cindex, 10);
        strcat(cmd, cindex);
        strcat(cmd, " ");
        strcat(cmd, rst_gather);
        if (system(cmd)) {
            print("Clean up error");
            exit(1);
        }
        
        strcpy(cmd, "ssh ");
        strcat(cmd, MIC_NAME);
        strcat(cmd, " \"rm -rf ");
        strcat(cmd, work_folder);
        strcat(cmd, "\"");
        if (system(cmd)) {
            print("Clean up error");
            exit(1);
        }
    }
}

//have not set ligand
//void conf_parser(struct conf* cf, const char* in_conf, const char* rcp)
void conf_parser(struct conf* cf, const char* in_conf, const char* lig_lib, const char* rcp, const char* vina, const char* vina_mic, const char* gather_loc)
{
    FILE* fp;
    fp = freopen(in_conf, "r", stdin);
    char s[MAX_CONF_SIZE], *delim = "= \n", *p;
    char tmp[MAX_CONF_SIZE], *delim2 = "/";
    
    conf_init(cf);
    
    while (gets(s)) {
        if (!strcmp(s, "\r") || !strcmp(s, "\n")) {
            continue;
        }
        p = strtok(s, delim);
        if (!strcmp(p, "center_x")) {
            p = strtok(NULL, delim);
            cf->cent[0] = atof(p);
        } else if (!strcmp(p, "center_y")) {
            p = strtok(NULL, delim);
            cf->cent[1] = atof(p);
        } else if (!strcmp(p, "center_z")) {
            p = strtok(NULL, delim);
            cf->cent[2] = atof(p);
        } else if (!strcmp(p, "size_x")) {
            p = strtok(NULL, delim);
            cf->size[0] = atof(p);
        } else if (!strcmp(p, "size_y")) {
            p = strtok(NULL, delim);
            cf->size[1] = atof(p);
        } else if (!strcmp(p, "size_z")) {
            p = strtok(NULL, delim);
            cf->size[2] = atof(p);
        } else if (!strcmp(p, "out")) {
            p = strtok(NULL, delim);
            strcpy(cf->outfile, p);
        } else if (!strcmp(p, "exhaustiveness")) {
            p = strtok(NULL, delim);
            cf->exht = atoi(p);
        } else if (!strcmp(p, "cpu")) {
            p = strtok(NULL, delim);
            cf->cpu = atoi(p);
        } else {
            print("%s\n", "Unknown Para in configure file");
            exit(1);
        }
    }
    strcpy(cf->lig_lib, lig_lib);
    strcpy(cf->rcp, rcp);
    strcpy(cf->vina, vina);
    strcpy(cf->vina_mic, vina_mic);
    strcpy(cf->gather_loc, gather_loc);
    
    strcpy(tmp, rcp);
    char* prev_p = p;
    p = strtok(tmp, delim2);
    while (p != NULL) {
        prev_p = p;
        p = strtok(NULL, delim2);
    }
    strcpy(cf->rcp_name, prev_p);
    //test("%s", prev_p);
    freopen("/dev/console", "r", stdin);
    fclose(fp);
}

int traverse(const char* lig_lib)
{
    DIR* dir;
    int i = 0;
    struct dirent* ptr;
    if ((dir = opendir(lig_lib)) == NULL) {
        print("%s\n", "Cannot open ligand library");
        exit(1);
    }
    
    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] == '.') {
            //skip hidden file
            continue;
        }
        lig_dic[i] = (char* )malloc(strlen(ptr->d_name) + 1);   //TODO
        strcpy(lig_dic[i++], ptr->d_name);
    }
    //printf("1");
    return i;
}

void conf_init(struct conf* cf)
{
    //strcpy(cf->lig, "");
    strcpy(cf->rcp, "");
    strcpy(cf->outfile, "");
    cf->cent[0] = cf->cent[1] = cf->cent[2] = 0;
    cf->size[0] = cf->size[1] = cf->size[2] = 30;
    cf->cpu = 24;
    cf->exht = 24;
}

void get_workpath(const char* home_path, int index, char* work_path)
{
    char tmp[MAX_INDEX];
    itoa(index, tmp, 10);
    //work_path = (char*) malloc(MAX_PATH);
    strcpy(work_path, home_path);
    strcat(work_path, tmp);
}
