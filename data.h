/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : data.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains the consts, variables and data types used
               by the assembler program
 ============================================================================
*/

#ifndef DATA_H_
#define DATA_H_

/* Constants */
#define MAX_LABEL 30 /*max chars in each variable in the program given*/
#define MAX_LINE 81 /*max chars of each code line in the program given*/
#define FIRST_ADDRESS 100 /*the first address which we will write to*/
#define FIRST_DATA_ADDRESS 0 /*the first address for the data structure*/
#define CODE_SIZE 500 /*max lines of code in the program give*/
#define MAX_CODE 100 /*max code chars in "4 weird" basis*/

/* Linked list types */
typedef struct node *ptr;
typedef struct node{ /* Variable table linked list type */
char label[MAX_LABEL]; /* Variable's label */
int address; /* Variable's address in decimal format */
int ex; /* Is it an external variable */
int ac; /* Is this an action variable */
ptr next; /* Pointer to the next node on the variable linked list */
}var_table;

typedef struct node3 *ptr3;
typedef struct node3{ /* Program and data code (binary based) linked list type */
int code[11]; /* The 10-base binary code, represented by an int array. The cell in [0] represents the address*/
ptr3 next;  /* Pointer to the next node on the program and data linked list */
}program_code;

typedef struct node4 *ptr4;
typedef struct node4{ /* Errors linked list type */
int line; /* The line number containing the error */
char text[MAX_LINE]; /* The text describing the error found */
ptr4 next; /* Pointer to the next node on the errors linked list */
}error_list;

typedef struct node5 *ptr5;
typedef struct node5{ /* Entries linked list type */
char label[MAX_LABEL]; /* The label of the entry */
char code[MAX_CODE]; /* The base-4-weird code address of the entry */
ptr5 next; /* Pointer to the next node on the entries linked list */
}ent_list;

typedef struct node6 *ptr6;
typedef struct node6{ /* Externals linked list type */
char label[MAX_LABEL]; /* The label of the external symbol */
char code[MAX_CODE]; /* The base-4-weird code address where the external variable is used */
ptr6 next; /* Pointer to the next node on the externals linked list */
}ext_list;

/* Global variables used in various places in the program */
extern char lines[CODE_SIZE][MAX_LINE]; /*matrix which contains the content n text of all the code lines of the program given*/
extern int IC; /*counter for the program address. initialized with the first address*/
extern int DC; /*counter for the data address*/
extern int line1; /* counter to know which line are we checking now */
extern int error_state; /* flag to show if the program has errors */
extern int error_table[CODE_SIZE+1]; /* error array that knows if a line has an error or note */
extern int cost_table[CODE_SIZE+1]; /* cost array that knows how much line of bitwise code the specific line consumes */

/* Pointers to the head of the linked lists */
extern ptr var_hptr; /* Head of the variables (symbols) linked list */
extern ptr3 data_hptr; /* Head of the data code linked list */
extern ptr3 program_hptr; /* Head of the program code linked list */
extern ptr4 error_hptr; /* Head of the errors linked list */
extern ptr5 ent_hptr; /* Head of the entries linked list */
extern ptr6 ext_hptr; /* Head of the externals linked list */

#endif /* DATA_H_ */
