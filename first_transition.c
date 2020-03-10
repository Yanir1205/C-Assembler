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
#include "first_transition.h"

void first_transition(int line_num) /*line_num indicates the number of code lines in the given program*/
{
	char l[MAX_LINE]={'\0'}; /*stores one line content from the input program*/
	int cost; /*stores the cost in memory cells for a given action*/
	char *l2; /*pointer for the line content*/
	char label[MAX_LABEL]; /*stores the label name given*/
	char m_label[MAX_LABEL];
	char word[MAX_LABEL]; /*stores a single word from the line*/
	char word1[MAX_LABEL]; /*stores a single word from the line*/
	char *temp;
	line1=1;
	temp=new_string(MAX_LABEL);
	while (line1<line_num) /*while we haven't reached the end of the input program*/
	{
		strcpy(l,lines[line1]); /*copies line no. line1 into parameter l*/
		l2=l; /*pointer to the begining of the line*/
		if (is_empty(l2)) /*if this is an empty line*/
		{
			line1++;
			continue;
		}
		else if (is_note(l2)) /*if this is a note line*/
		{
			line1++;
			continue;
		}
		next_word(l2,word); /*stores the first word in parameter word*/
		if (is_label(word,label)) /*if the first word is a label - store it in a variable called label*/
		{
			l2+=strlen(word);
			if (!(next_word(l2,word))) /*if there is no word after the label - report an error*/
			{
				line1++;
				continue;
			}
		}
		if (matrix_label(l2,m_label))
			strcpy(word,m_label);
		strcpy(word1,word);
		tolowerword(word1,word);
		if (is_instruction(word)) /*if the next word is an instruction*/
		{
			cost=line_cost(l,word);
			if (label[0]!='\0')
				add_var(label,DC,0,0); /*adds the label to the variable table and if its already there- reports an error*/
			if (cost>0)
			{
				instruction_coder(l,word,temp); /*encodes the code for the instruction in line l*/
				DC+=cost;
			}
			line1++;
			continue;
		}
		else if (is_data(word)) /*if this is a data line*/
		{	
			cost=line_cost(l,word);
			if (label[0]!='\0')
				add_var(label,IC,0,1);
			if (cost>0)
			{
				data_coder(l,word); /*adds the order in binary code to the linked list-without adding the value lines!*/
				blank_coder(cost-1); /*adds the value lines as blank binary code for the first transition*/
				value_coder(l,word);
				IC+=cost;
			}
			line1++;
			continue;
		}
		if (is_extern(word)) /*if this is an extern line*/
		{
			l2+=strlen(word);			
			next_word(l2,word); 
			if (check_label(word))
				strcpy(label,word); /*stores the next word into label*/
			if (label[0]=='\0') /*if there is no label after ".extern"*/
			{
				add_error("illegal variable given in .extern command",line1);
				line1++;
				continue;
			}				
			add_var(label,0,1,0); /*adding the new label node to the linked list*/
			line1++;
			continue;
		}
		if (is_entry(word)) /*if it is an entry line - we shall skip it in the first transition*/
		{
			l2+=strlen(word);
			next_word(l2,word);
			if (check_label(word))
				strcpy(label,word);
			if (label[0]=='\0')
			{
				line1++;
				continue;
			}
			line1++;
			continue;
		}
		else /*if this line is non of the above*/
		{
			if (*word!='\0')
				add_error("illegal statement in line",line1);
			line1++;
			continue;
		}
	}
}
