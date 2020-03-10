/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : assy.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains main command that runs the assembler
 ============================================================================
*/

#ifndef ASSY_H_
#define ASSY_H_

#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "commands.h"
#include "file_memory_mgmt.h"
#include "file_to_array.h"
#include "first_transition.h"
#include "line_cost.h"
#include "second_transition.h"

/* first definition of all global variables of the program */
/* comments regarding each variable usage can be found in data.h */
char lines[CODE_SIZE][MAX_LINE];
int IC;
int DC;
int line1;
int error_state;
int error_table[CODE_SIZE+1];
int cost_table[CODE_SIZE+1];
ptr var_hptr;
ptr3 data_hptr;
ptr3 program_hptr;
ptr4 error_hptr;
ptr5 ent_hptr;
ptr6 ext_hptr;

int main(int argc, char *argv[]);

#endif /* ASSY_H_ */
