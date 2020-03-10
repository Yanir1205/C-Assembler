/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : assy.h
 Semester    : 2017b
 Description : This file contains main command that runs the assembler
 ============================================================================
*/
#include "main.h"

int main(int argc, char *argv[]) {
	int i,numOfLines;
	FILE *fp;
	printf("MMN14 (C Programming Lab Project)- Two passes assembler\nWritten by Yanir Shaked and Gilad Gavze\n===============================\n");
	if (argc<2) /*only the program name has been given by the user*/
	{
		printf("No file name was given.\nGoodbye\n");
		fprintf(stderr,"\nno file location and name has been given");
		exit(0);
	}
	for (i=1;i<argc;i++) /*reading all the files the user has writen*/
	{
		if (!(fp=fopen(*(argv+i),"r+"))) /*if the file cannot be opened we will write an error*/
		{
			fprintf(stderr,"\ncannot open file");
			exit(0);
		}
		printf("Starting assembler on %s\n",argv[i]);
		initialize(); /* initialize all objects in memory*/
		numOfLines=file_to_array(fp); /* insert the lines of the text file to an array + white space removal */
		first_transition(numOfLines); /* run the assembler's first transition */
		second_transition(numOfLines); /* run the assembler's second transition */
		printf("Assembler done on %s\n",argv[i]);
		if (error_state==0) /* no errors found on the .as file, create the files */
			file_creation(argv[i]);
		else /* if errors were found, print them. don't create the file */
			print_errors();
		cleanup(fp); /* clear all objects in memory */
	}
	return 0;
}
