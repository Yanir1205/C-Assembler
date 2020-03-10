/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : first_transition.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains the command that performs the first
 	 	 	   assembler transition and catches errors
 ============================================================================
*/

#ifndef FIRST_TRANSITION_H
#define FIRST_TRANSITION_H

#include <string.h>
#include "commands.h"
#include "data.h"
#include "line_cost.h"

void first_transition(int line_num); /*line_num indicates the number of code lines in the given program*/

#endif /* FIRST_TRANSITION_H */
