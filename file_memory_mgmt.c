/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : file_memory_mgmt.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains all the commands related to file creation, 
	           variable cleanup and bypass
 ============================================================================
*/

#include "file_memory_mgmt.h"

void initialize() { /* initialize all the variables used by the program */
	IC=FIRST_ADDRESS; /* bypass the instruction code to the first address, as there are no program codes */
	DC=0; /* bypass the data coder to 0, as there are no data codes */
	line1=1; /* first line number */
	var_hptr = NULL; /* clear the var linked list head */
	data_hptr=NULL; /* clear the data linked list head */
	program_hptr=NULL; /* clear the program code linked list head */
	error_hptr=NULL; /* clear the error linked list head */
	ent_hptr=NULL; /* clear the entries linked list head */
	ext_hptr=NULL; /* clear the externals linked list head */
	memset(lines,'\0',sizeof(char)*MAX_LINE*CODE_SIZE); /* clear the char array that has the original program textual code */
	memset(error_table,0,sizeof(int)*(CODE_SIZE+1)); /* clear the int array that points if a line has an error or not */
	memset(cost_table,0,sizeof(int)*(CODE_SIZE+1)); /* clear the int array that knows how much line of bitwise code the specific line consumes */
}

void create_ob_file(char* filename) {	/* run thru the matrix that contains the lines numbers and commands in binary, translate them to base4weird and create the file */
	FILE *fp;
	ptr3 temp;
	int i;
	char* new_filename = new_string(strlen(filename)+4);
	strcpy(new_filename,filename);
	strcat(new_filename,".ob");
	temp=program_hptr;
	i=FIRST_ADDRESS;
	if ((temp->next) != NULL) { /*create the file only if there are entries in the program code linked list*/
		fp=fopen(new_filename,"wt"); /* open the file for writing */
		if (fp != NULL)
		{
			fprintf(fp,"%s\n",base4heading());
			while (temp) {
				fprintf(fp,"%s\n",base4line(temp,i));
				temp=temp->next;
				i++;
			}
			fclose(fp);
			printf("File %s has been written.\n",new_filename);
		}
		else
			printf("Error writing %s file.\n",new_filename);
	}
}

void create_ext_file(char* filename) {	/* run thru the ext linked list and create the ext if there is data to write */
	FILE *fp;
	ptr6 temp;
	char*new_filename = new_string(strlen(filename)+4);
	strcpy(new_filename,filename);
	strcat(new_filename,".ext");
	temp=ext_hptr;
	if (temp) { /*create the file only if there are entries in the externals linked list*/
		fp=fopen(new_filename,"wt"); /* open the file for writing */
		if (fp != NULL) {	
			while (temp) {
					fprintf(fp,"%s\t",temp->label);
					fprintf(fp,"%s\n",temp->code);
				temp=temp->next;
			}
			fclose(fp);
			printf("File %s has been written.\n",new_filename);
		}
		else
			printf("Error writing %s file.\n",new_filename);
	}
}

void create_ent_file(char* filename) {	/* run thru the ent linked list and create the ent if there is data to write */
	FILE *fp;
	ptr5 temp;
	char*new_filename = new_string(strlen(filename)+4);
	strcpy(new_filename,filename);
	strcat(new_filename,".ent");
	temp=ent_hptr;
	if ((temp->next) != NULL) { /*create the file only if there are entries in the entries linked list*/
		fp=fopen(new_filename,"wt"); /* open the file for writing */
		if (fp != NULL)
		{	
			while ((temp) != NULL) {
					fprintf(fp,"%s\t",temp->label);
					fprintf(fp,"%s\n",temp->code);
				temp=temp->next;
			}
			fclose(fp);
			printf("File %s has been written.\n",new_filename);
		}
		else
			printf("Error writing %s file.\n",new_filename);
	}
}

void file_creation(char* filename) { /* create the ob, ext, ent files */
	char* new_filename;
	if (strchr(filename,'.') != NULL){ /* if there's an extension to the original file, clear it */
		new_filename = new_string(strlen(filename)-3);
		strcpy(new_filename,filename);
		new_filename = strtok(new_filename,".");
	}
	else { /* there's no extension to the original file */
		new_filename = new_string(strlen(filename));
		strcpy(new_filename,filename);
	}
	create_ob_file(new_filename);
	create_ext_file(new_filename);
	create_ent_file(new_filename);
}

void clear_var_list() { /* clear var linked list */
	ptr current = var_hptr;
	ptr next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	var_hptr = NULL;
}

void clear_data_list() { /* clear data linked list */
	ptr3 current = data_hptr;
	ptr3 next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	data_hptr = NULL;
}

void clear_program_list() { /* clear program code linked list */
	ptr3 current = program_hptr;
	ptr3 next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	program_hptr = NULL;
}



void clear_error_list() { /* clear error linked list */	
	ptr4 current = error_hptr;
	ptr4 next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	error_hptr = NULL;
}

void clear_ent_list() { /* clear entries file linked list */
	ptr5 current = ent_hptr;
	ptr5 next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	ent_hptr = NULL;
}

void clear_ext_list() { /* clear externals file linked list */
	ptr6 current = ext_hptr;
	ptr6 next;
	while (current != NULL)
	{
	   next = current->next;
	   free(current);
	   current = next;
	}
	ext_hptr = NULL;
}

void cleanup(FILE *fp) { /* releases all used memory */
	clear_var_list(); /* clear the var linked list */
	clear_program_list(); /* clear the program linked list */
	clear_data_list(); /* clear the data linked list */
	clear_error_list(); /* clear the error linked list */
	clear_ent_list(); /* clear the entries linked list */
	clear_ext_list(); /* clear the externals linked list */
	fclose(fp); /* close the .as file */
}
