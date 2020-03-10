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

#ifndef FILE_MEMORY_H_
#define FILE_MEMORY_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"
#include "commands.h"

void initialize(); /* initialize all the variables used by the program */

void create_ob_file(char* filename); /* run thru the matrix that contains the lines numbers and commands in binary, translate them to base4weird and create the file */

void create_ext_file(char* filename); /* run thru the ext linked list and create the ext if there is data to write */

void create_ent_file(char* filename); /* run thru the ent linked list and create the ent if there is data to write */

void file_creation(char* filename); /* create the ob, ext, ent files */

void clear_var_list(); /* clear var linked list */

void clear_data_list(); /* clear data linked list */

void clear_program_list(); /* clear program code linked list */

void clear_error_list(); /* clear error linked list */

void clear_ent_list(); /* clear ent file linked list */

void clear_ext_list(); /* clear ext file linked list */

void cleanup(FILE *fp); /* releases all used memory */

#endif /* FILE_MEMORY_H_ */
