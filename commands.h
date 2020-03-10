/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : commands.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains all the general commands, related to the 
 	 	 	   assembler scans and linked list data management
 ============================================================================
*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "data.h"

char* new_string(int size); /* return a pointer to a new string with the requested size */

int is_error(int linenum);/* check if a specific code line has an error */

void clear_line(int linenum); /* clear a line on the program code char array */

void add_error(char *errortext,int linenum);/* add error to the error linked list at the currect place (by line number) */

int convert2base4(int number); /* converts a decimal integer to a base 4 integer using recursion */

void replacechar(char *str, char orig, char rep); /* replaces a char in a char array with a different char */

char* convert2base4w(int num); /* converts a decimal integer to a weird base 4 number */

char* base4line(ptr3 lineptr,int linenum); /* returns a weird base 4 command + data line, ready to be written to a file */

char* base4heading(); /*returns the heading of the ob file, that includes the number of commands and size of data area */

void print_errors(); /* print all the errors from the error linked list */

void add_entry(char *labeltext, int address); /* add an item to the entry linked list */

void add_external(char *labeltext, int address); /* add an item to the externals linked list */

void add_var(char *label1,int DC1,int external,int action);

void blank_coder(int num1); /*receives the line content, the order given and the number of blank items to be added*/

int check_label(char label[MAX_LABEL]);

int is_data(char word[MAX_LINE]); /*checks if line l is a data line or not*/

int is_empty(char l[MAX_LINE]);

int is_entry(char word[MAX_LABEL]);

int is_extern(char word[MAX_LABEL]);

int is_instruction(char word[MAX_LABEL]); /*checks if word is an isntruction or not*/

int is_label(char word[MAX_LABEL],char label[MAX_LABEL]); /*checks if the word is a label and if so returns the label itself inside parameter label*/

int is_note(char l[MAX_LINE]);

int label_in_table(char label1[MAX_LABEL],int *addr1);

void tolowerword(char word1[MAX_LABEL],char word2[MAX_LABEL]);

void toupperword(char word1[MAX_LABEL],char word2[MAX_LABEL]);

int next_word(char l2[MAX_LABEL],char *word);

int matrix_register(char word[MAX_LABEL],char *reg1,char *reg2);

int matrix_instruction_nums(char word[MAX_LABEL],int *num1,int *num2);

int matrix_label(char word[MAX_LINE],char label[MAX_LABEL]);

int method(char word[MAX_LABEL]);

void num_coder(int *a, int n); /*receives an array to write the number n to in binary basis*/

void instruction_coder(char l[MAX_LINE],char order[MAX_LABEL],char *temp); /*receives the entire line l and the order to perfrom*/

void data_coder(char l[MAX_LINE],char order[MAX_LABEL]); /*codes the action word in binary basis in the program linked list*/

void value_coder(char l[MAX_LINE],char order[MAX_LABEL]); /*codes the values after the order itself in case of data orders such as mov and on*/

#endif /* COMMANDS_H_ */
