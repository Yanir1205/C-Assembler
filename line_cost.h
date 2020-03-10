/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : line_cost.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains the command that calculate the cost of an
 	 	 	   assembler command, in terms of lines of 10-bit binary command
			   codes
 ============================================================================
*/

#ifndef LINE_COST_H
#define LINE_COST_H

#include <string.h>
#include "commands.h"
#include "data.h"

void update_cost(int linenum, int cost); /* updates the cost table with the line cost */
 
int is_cost(int linenum); /* return the line cost from the line cost table */

int cost_switch(int switcher);

int line_cost(char *l,char order[MAX_LABEL]); /* calculate the line's cost in therms of lines of 10-bit binary command codes */

#endif /* LINE_COST_H */
