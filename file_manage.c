#include <file_manage.h>
#include <sys/stat.h>
#include <string.h>

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
