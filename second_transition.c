/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : second_transition.h
 Semester    : 2017b
 Students	 : Gilad Gavze (ID 300037645), Yanir Shaked (ID 021988001)
 Description : This file contains the command that performs the second
 	 	 	   assembler transition and catches errors
 ============================================================================
*/
#include "second_transition.h"

void second_transition(int line_num) /*line_num indicates the number of code lines in the given program*/
{
	ptr3 p2; /*pointer to the head of the program code linked list*/
	ptr3 p1,p3; /*pointer to the instructions code linked list*/
	ptr v1; /*pointer to the variable table linked list*/
	int addr1,line2=1; /*addr1 marks the next address to write the instructions to, line2 marks the current line number being read*/
	char l[MAX_LINE]; /*stores one line content from the input program*/
	char *l2; /*pointer to a char in line l*/
	char word[MAX_LINE]; /*stores a single word from the line*/
	char word2[MAX_LINE];
	int word_counter; /*counter for the word number in each line*/
	char label[MAX_LABEL]; /*stores the label name*/
	int label_addr; /*stores the label address*/
	int m1; /*stores the directive method of use in action words*/
	int register_state; /*states if the operand is a register*/
	char m_label[MAX_LABEL]={'\0'}; /*stores the matrix label*/
	int a[11]; /*stores the word code in binary basis*/
	int i;
	int error_flag;

	addr1=IC;
	p2=program_hptr; /*p2 points to the head of the program linked list*/
	while (p2->next)
		p2=p2->next;
	p1=data_hptr; /*p1 points to the head of the instructions linked list*/
	p3=p1;
	while (p3) /*while we haven't reached the end of the instructions list*/
	{
		p3->code[0]=addr1; /*updating the instruction code address from IC and on*/
		addr1++; /*the next address to write instructions to*/
		p3=p3->next; /*points to the next code line in the instructions list*/
	}
	p2->next=p1; /*pasting the instructions code to the end of the program code*/
	v1=var_hptr; /*v1 points to the head of the variable linked list*/
	while (v1)
	{
		if (((v1->ex)==0)&&((v1->ac)==0)) /*if the variable is not external and it is not an action code*/
			v1->address=(v1->address)+IC; /*updating the address for variables which are not external and are not part of actions*/
		v1=v1->next;
	}
	word_counter=1;
	addr1=FIRST_ADDRESS; /*marks the next address to fix in the binary program code*/
	v1=var_hptr;
	while (v1)
		v1=v1->next;
	while (line2<line_num)
	{
		word_counter=1;
		strcpy(l,lines[line2]); /*copies line no. line2 into parameter l*/
		l2=l; /*pointer to the begining of the line*/
		if (is_empty(l2)) /*if this is an empty line*/
		{
			line2++;
			continue;
		}
		else if (is_note(l2)) /*if this is a note line*/
		{
			line2++;
			continue;
		}
		else if (is_error(line2))
		{
			line2++;
			continue;
		}
		next_word(l2,word); /*stores the first word in parameter word*/
		if ((is_label(word,label))&&(word_counter==1)) /*if the first word is a label - store it in a variable called label*/
		{
			word_counter++;
			l2+=strlen(word);
			next_word(l2,word); /*if the first word in the line is a label - skip to the next word*/
		}
		if (matrix_label(word,m_label)) /*if there is a matrix label in the line*/
			strcpy(word,m_label);
		tolowerword(word,word2);
		strcpy(word,word2);
		if (is_instruction(word))
		{
			line2++;
			addr1++;
			continue;
		}
		if (is_extern(word))
		{
			line2++;
			continue;
		}
		if (is_entry(word))
		{
			l2+=strlen(word)+1;
			next_word(l2,word); /*if the word is .entry - we shall read the next word and see if its a valid label*/
			if (!(check_label(word)))
			{
				add_error("the label given by .entry is not valid",line2);
				line2++;
				continue;
			}
			if (label_in_table(word,&label_addr))
			{
				add_entry(word,label_addr);
				line2++;
				continue;
			}
			else /*meaning the label doesn't exist in the variable table*/
			{
				add_error("the label given by .entry doesn't exist in the variable table",line2);
				line2++;
				continue;
			}
		}
		else /*meaning this is an action word such as mov and so on*/
		{
			addr1++; /*the action item itself takes one memory line*/
			p2=program_hptr; /*p2 points to the head of the program linked list*/
			while (p2) {
				if (p2->code[0]!=addr1)
					p2=p2->next;
				else
					break;
			}
			register_state=0;
			l2+=strlen(word)+1; /*l2 points to the first char after the action word*/
			next_word(l2,word); /*taking the next word after the action word*/
			error_flag=0;
			while ((next_word(l2,word))&&(error_flag==0)) /*while there are still opernads in the line*/
			{
				error_flag=0;
				m1=method(word);
				switch (m1)
				{
					case 0: /*meaning this is a dierct number starting in '#' - do nothing since it is already coded!*/
					{
						addr1++;
						p2=p2->next;
						break;
					}
					case 1: /*meaning there is a label*/
					{
						if (!(label_in_table(word,&label_addr))) /*if the label doesn't appear in the variable table*/
						{
							addr1++;
							p2=p2->next;
							line2++;
							error_flag=1;
						}
						else /*meaning the label appears in the var table*/
						{
							num_coder(a,label_addr); /*returns the number n in binary array inside a*/
							for (i=1;i<9;i++)
								p2->code[i]=a[i+1];
							if (label_addr==0) /*meaning this is an external label-we shall write it to the EXT file*/
							{
								add_external(word,addr1); /*adding the label name and address to the EXT file*/
								p2->code[9]=0;
								p2->code[10]=1;
							}
							else
							{
								p2->code[9]=1;
								p2->code[10]=0;
							}
							addr1++;
							p2=p2->next;	
						}
						break;
					}
					case 2: /*meaning this is a matrix*/
					{
						matrix_label(word,m_label);
						if ((label_in_table(m_label,&label_addr))==0) /*if the matrix label doesn't appear in the variable table*/
						{
							add_error("the label doesn't exist in the variable table",line2);
							addr1++;
							p2=p2->next;
							error_flag=1;
						}
						else
						{	
							num_coder(a,label_addr); /*returns the number n in binary array inside a*/
							for (i=1;i<9;i++)
								p2->code[i]=a[i+1];
							if (label_addr==0) /*meaning this is an external label-we shall write it to the EXT file*/
							{
								add_external(word,label_addr); /*adding the label name and address to the EXT file*/
								p2->code[9]=0;
								p2->code[10]=1;
							}
							else
							{
								p2->code[9]=1;
								p2->code[10]=0;
							}
							p2=p2->next->next; /*adding an extra code word skip in order to skip the matrix registers code line which has already been writen in the first transition*/
							addr1+=2;
						}
						break;
					}
					case 3: /*meaning this is a register*/
					{
						if (register_state==1) /*if we already encountered with a register- we shall do nothing now*/
						{
						}
						else
						{
							register_state+=1; /*marks we have encountered with a register*/
							p2=p2->next; /*moving on to the next code word*/
							addr1++;
						}
						break;
					}
					case 4: /*meaning there is an error in the method - already reported*/
					{
						line2++;
						addr1++;
						p2=p2->next;
						error_flag=1;
						break; /*move on to the next line*/
					}
				}
				if (error_flag==0)
					l2+=strlen(word)+1;
			}
			line2++;
		}
	}
}
