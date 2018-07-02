/*
Geun Jeon
CS 283
Lab02
2018-2-22
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#define MAX_NUM_FILES 1024


DIR* Open_Dir(char* dir_name);
int Get_Files(DIR* dir, char** files); 
int Get_Reg_Files(char* directory, char** files, int num_files); 
bool File_Exists(char* file, char** array, int array_size); 
char* Full_Path(char* dir, char* file); 
void Delete_File(char* file); 
void Copy_From_To(char* from, char* to); 


int main(int argc, char** argv)
{

	if(argc != 3)
	{
		fprintf(stderr, "Invalid Number of Arguments\n");
		exit(1);
	}

	
	char* dir_a_name = argv[1]; char* dir_b_name = argv[2];
	if( dir_a_name[strlen(dir_a_name) - 1] != '/' ||
	 	 dir_b_name[strlen(dir_b_name) - 1] != '/' )
	{
		fprintf(stderr, "Directory Not Passed\n");
		exit(1);
	}

	
	DIR* dir_a; DIR* dir_b;
	char* dir_a_files[MAX_NUM_FILES]; char* dir_b_files[MAX_NUM_FILES];

	
	dir_a = Open_Dir(dir_a_name); dir_b = Open_Dir(dir_b_name);

	
	int dir_a_num_files = Get_Files(dir_a, dir_a_files); 
	int dir_b_num_files = Get_Files(dir_b, dir_b_files);
	dir_a_num_files = Get_Reg_Files(dir_a_name, dir_a_files,dir_a_num_files);	
	dir_b_num_files = Get_Reg_Files(dir_b_name, dir_b_files,dir_b_num_files);	

	int DELETE_FILE = -1;
	int CP_TO_A = 1;
	int CP_TO_B = 2;
	
	int dir_a_sync[dir_a_num_files];
	memset(dir_a_sync, 0, dir_a_num_files * sizeof(int));
	int dir_b_sync[dir_b_num_files];
	memset(dir_b_sync, 0, dir_b_num_files * sizeof(int));
	
	for(int i = 0; i < dir_a_num_files; i++)
	{
		if( ! File_Exists(dir_a_files[i], dir_b_files, dir_b_num_files) )
		{
			dir_a_sync[i] = CP_TO_B;
		}
	}
	
	for(int i = 0; i < dir_b_num_files; i++)
	{
		if( ! File_Exists(dir_b_files[i], dir_a_files, dir_a_num_files) )
		{
			dir_b_sync[i] = DELETE_FILE;
		}
	}	
	
	for(int i = 0; i < dir_a_num_files; i++)
	{
		if( File_Exists(dir_a_files[i], dir_b_files, dir_b_num_files) )
		{
			struct stat file_in_a;
			char* file_in_a_path = Full_Path(dir_a_name, dir_a_files[i]);
			if(stat(file_in_a_path, &file_in_a) < 0)
				fprintf(stderr, "Error Accessing %s\n", file_in_a_path);

			struct stat file_in_b;
			char* file_in_b_path = Full_Path(dir_b_name, dir_a_files[i]);
			if(stat(file_in_b_path, &file_in_b) < 0)
				fprintf(stderr, "Error Accessing %s\n", file_in_b_path);
			
			if( difftime(file_in_a.st_mtime, file_in_b.st_mtime) > 0 )
			{
				dir_a_sync[i] = CP_TO_B;
			}
			free(file_in_a_path); free(file_in_b_path);
		}
	}

	for(int i = 0; i < dir_b_num_files; i++)
	{
		if( File_Exists(dir_b_files[i], dir_a_files, dir_a_num_files) )
		{
			
			struct stat file_in_a;
			char* file_in_a_path = Full_Path(dir_a_name, dir_a_files[i]);
			if(stat(file_in_a_path, &file_in_a) < 0)
				fprintf(stderr, "Error Accessing %s\n", file_in_a_path);

			struct stat file_in_b;
			char* file_in_b_path = Full_Path(dir_b_name, dir_a_files[i]);
			if(stat(file_in_b_path, &file_in_b) < 0)
				fprintf(stderr, "Error Accessing %s\n", file_in_b_path);

			
			if( difftime(file_in_a.st_mtime, file_in_b.st_mtime) < 0 )
			{
				dir_b_sync[i] = CP_TO_A; 
			}
			free(file_in_a_path); free(file_in_b_path);
		}
	}


	
	for(int i = 0; i < dir_a_num_files; i++)
	{
		
		if(dir_a_sync[i] == DELETE_FILE)
		{
			char* full_path = Full_Path(dir_a_name, dir_a_files[i]);
			Delete_File(full_path);
			fprintf(stdout, "Deleted %s\n", full_path);
			free(full_path);
		}
		
		
		else if(dir_a_sync[i] == CP_TO_B)
		{
			char* cp_from = Full_Path(dir_a_name, dir_a_files[i]);
			char* cp_to = Full_Path(dir_b_name, dir_a_files[i]);
			Copy_From_To(cp_from, cp_to);
			fprintf(stdout, "Copied %s to %s\n", cp_from, cp_to);
			free(cp_from); free(cp_to);			
		}
	}
	
	
	for(int i = 0; i < dir_b_num_files; i++)
	{
		
		if(dir_b_sync[i] == DELETE_FILE)
		{
			char* full_path = Full_Path(dir_b_name, dir_b_files[i]);
			Delete_File(full_path);
			fprintf(stdout, "Deleted %s\n", full_path);
			free(full_path);			
		}
		
		
		else if(dir_b_sync[i] == CP_TO_A)
		{
			char* cp_from = Full_Path(dir_b_name, dir_b_files[i]);
			char* cp_to = Full_Path(dir_a_name, dir_b_files[i]);
			Copy_From_To(cp_from, cp_to);
			fprintf(stdout, "Copied %s to %s\n", cp_from, cp_to);
			free(cp_from); free(cp_to);			
		}
	}	
	
	exit(0);
}


char* Full_Path(char* dir, char* file)
{
	char* out = malloc( strlen(dir) + strlen(file) + 1 );
	strcpy(out, dir); 
	strcat(out, file);
	return out;
}


void Delete_File(char* file)
{
	if(remove(file) != 0)
	{
		fprintf(stderr, "Unable to Delete File%s\n", file);
	}
}



void Copy_From_To(char* from, char* to)
{
	
	struct stat from_file_permissions; stat(from, &from_file_permissions);

	
	FILE* from_file = fopen(from, "r");
	FILE* to_file = fopen(to, "w+");
	if(from_file == NULL)
		fprintf(stderr, "Error Opening %s\n", from);

	if(to_file == NULL)
	{
		fprintf(stderr, "Error Opening %s\n", to);
	}

	char buffer[256];
	int bytes_read;

	while(1)
	{
		size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer),from_file);
		fwrite(buffer, sizeof(char), bytes_read, to_file);
		if(bytes_read < sizeof(buffer) && feof(from_file))
			break;
	}
	fclose(from_file); fclose(to_file);


	if( chmod(to, from_file_permissions.st_mode & 07777) == -1 )
		fprintf(stderr, "Error, file permission for %s\n", to);
}


bool File_Exists(char* file, char** array, int array_size)
{
	for(int i = 0; i < array_size; i++)
	{
		if(strcmp(file, array[i]) == 0)
			return true;
	}
	return false;
}


int Get_Reg_Files(char* directory, char** files, int num_files)
{
	char* tmp;
	struct stat check_file;
	int c = 0;

	for(int i = 0; i < num_files; i++)
	{
		tmp = malloc( strlen(directory) + strlen(files[i]) + 1 );
		tmp[0] = '\0';
		strcat(tmp, directory);
		strcat(tmp, files[i]);
		stat(tmp, &check_file);	
	
		if( S_ISREG(check_file.st_mode) )
		{
			char* tmp2 = malloc( strlen(files[i]) + 1 );
			strcpy(tmp2, files[i]);
			free(files[c]);
			files[c] = malloc( strlen(files[i]) + 1);
			strcpy(files[c], tmp2);
			free(tmp2);
			c++;
		}
		free(tmp);
	}
	return c;
}
int Get_Files(DIR* dir, char** files)
{
	int c = 0;
	struct dirent* directory_ent;
	while( (directory_ent = readdir(dir)) != NULL )
	{
		files[c] = malloc( sizeof(directory_ent->d_name) );
		strcpy(files[c], directory_ent->d_name);
		c++;
	}
	return c; 
}

DIR* Open_Dir(char* dir_name)
{
	DIR* dir;
	if( (dir = opendir(dir_name)) == NULL )
	{
		fprintf(stderr, "Error on Open Directory %s\n", dir_name);
		exit(1);
	}
	return dir;
}
