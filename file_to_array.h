/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : file_to_array.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains the command that imports the program textual
 	 	 	   code and adds it to a char matrix while removing whitepsaces,
			   tabs, comments and catches syntax errors
 ============================================================================
*/

#ifndef FILE_ARRAY_H_
#define FILE_ARRAY_H_

#include <stdio.h>
#include "data.h"
#include "commands.h"

int file_to_array(FILE* fp); /*recieves a pointer to a file and insert the file's lines to an array + remove whitespaces */

#endif /* FILE_ARRAY_H_ */