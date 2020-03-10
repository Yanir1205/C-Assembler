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
#include "line_cost.h"

void update_cost(int linenum, int cost) { /* updates the cost table with the line cost, recieves the line number and its cost */
	cost_table[linenum] = cost;
}

int is_cost(int linenum) { /* returns the line cost that is already stored in a line cost array. recieves the line number */
	return cost_table[linenum];
}

int cost_switch(int switcher) { /* recieves the method number (0-3) and returns the cost*/
	switch (switcher) {
		case 0:	/*imidiate method*/
			return 1;
			break;
		case 1: /*direct method*/
			return 1;
			break;
		case 2: /*matrix access method*/
			return 2;
			break;
		case 3:
			return 1;
			break;
		default:
			return 0;
	}
}

int line_cost(char *l,char order[MAX_LABEL]) /* recieves a pointer to a line and the line's order, returns its cost in meaning of number of nodes in the program linked list*/
{
	int cost=0; /*initialized to 1 because the order itself is a single binary word in the code*/
	char *l2,*l1;
	char word[MAX_LABEL]={'\0'};
	char word1[MAX_LABEL]={'\0'};
	char word2[MAX_LABEL]={'\0'};
	int n,n1,n2;
	int count=0;
	int case1=0,case2=0;
	l2=l;	
	if (is_instruction(order)) /*computes the cost in case of instruction line*/
	{
		if (strcmp(order,".data")==0) /*computes the cost in case of .data sentence*/
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen(".data"); /*l2 points to the next char after '.data'*/
			while (next_word(l2,word)) /*while there are values after the .data statement separated by ','*/
			{
				cost+=1;
				l2+=strlen(word)+1; /*l2 points to the next char after the new word*/
			}
		}
		else if (strcmp(order,".string")==0) /*computes the cost in case of .string sentence*/
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen(".string")+1; /*l2 points to the first quotation mark in the string*/			
			if (*l2!='"') /*if there is no quotation mark for the string*/
			{
				add_error("there is no quotation marks at the beginning of the string",line1);
				line1++;
				return 0;
			}
			l2+=1; /*l2 points to the first char of the string*/
			while ((*l2!='"')||(*l2=='\0')||(*l2=='\n')) /*while we haven't reached the end of the string*/
			{
				if ((!isalpha(*l2)) && (!isdigit(*l2)))
					break;
				l2++;
				cost+=1;
			}
			if (*l2!='"')
			{
				add_error("there is no quotation mark at the end of the string",line1);
				cost=0;
				line1++;
				return 0;
			} 
			cost+=1; /*another binary word is used to mark the end of the string*/
		}
		else if (strcmp(order,".mat")==0) /*computes the cost in case of .mat sentence*/
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen(".mat"); /*l2 points to the first char after .mat*/
			if (!(matrix_instruction_nums(l2,&n1,&n2))) /*meaning the matrix definition is incorrect*/
			{
				add_error("Matrix definition is incorrect",line1);
				line1++;
				return 0;
			}
			l2=strstr(l2,"]");
			l2++;
			l2=strstr(l2,"]");
			l2++; /*l2 points to the first space after the matrix definition*/
			n=n1*n2; /*this is the number of values the matrix should receive*/
			cost=n;
			while (next_word(l2,word)) /*as long as there are still values*/
			{
				l2+=strlen(word)+1; /*l2 points to the char after the value entered*/ 
				count+=1;
			}
			if (count>n) /*if the number of values entered is larger then the matrix capacity*/
			{
				cost=0;
				add_error("there are too many numbers in the matrix",line1);
				line1++;
				return 0;
			}
		}
	}
	else if(is_data(order))
	{
		cost=1; /*every order sentence requires at least 1 binary word in the memory*/
		if (strcmp(order,"mov")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("mov"); /*points to the char after 'mov'*/
			l2+=strlen(word)+1;
			if (!(next_word(l2,word))) /*if there are no operands after the order*/
			{
				add_error("there are no operands for command 'MOV'",line1);
				line1++;
				return 0;
			}
			case1=method(word); /*checks what method of directive is used on the first operand*/
			cost+=cost_switch(case1);
			l2+=strlen(word)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there is no second operand for command 'MOV'",line1);
				line1++;
				return 0;
			}
			case2=method(word);
			cost+=cost_switch(case2);
			l2+=strlen(word);
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'MOV'",line1);
				line1++;
				return 0;
			}
			if ((case1==3)&&(case2==3)) /*if both operands are registers - they share a common binary word and cost 1 line only*/
				cost--;
		}
		else if (strcmp(order,"add")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("add");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'ADD'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			l2+=strlen(word)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there is no second operand for command 'ADD'",line1);
				line1++;
				return 0;
			}
			case2=method(word);
			cost+=cost_switch(case2);
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'ADD'",line1);
				line1++;
				return 0;
			}
			if ((case1==3)&&(case2==3)) /*if both operands are registers - they share a common binary word and cost 1 line only*/
				cost--;
		}
		else if (strcmp(order,"cmp")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("cmp");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'CMD'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			l2+=strlen(word)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there is no second operand command 'CMD'",line1);
				line1++;
				return 0;
			}
			case2=method(word);
			cost+=cost_switch(case2);
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'CMP'",line1);
				line1++;
				return 0;
			}
			if ((case1==3)&&(case2==3)) /*if both operands are registers - they share a common binary word and cost 1 line only*/
				cost--;
		}
		else if (strcmp(order,"sub")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("sub");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'SUB'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			l2+=strlen(word)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there is no second operand for command 'SUB'",line1);
				line1++;
				return 0;
			}
			case2=method(word);
			cost+=cost_switch(case2);
			if (case2==0) {
				add_error("there is no such method for command 'SUB'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'SUB'",line1);
				line1++;
				return 0;
			}
			if ((case1==3)&&(case2==3)) /*if both operands are registers - they share a common binary word and cost 1 line only*/
			{
				cost--;
			}
		}
		else if (strcmp(order,"lea")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("lea");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'LEA'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if ((case1==0)||(case1==3)) {
				add_error("there is no such method for 'LEA'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there is no second operand for 'lea'",line1);
				line1++;
				return 0;
			}
			case2=method(word);
			cost+=cost_switch(case2);
			if ((case2==0)||(case2==3)) {
				add_error("there is no such method for 'LEA'",line1);
				line1++;
				return 0;
			}
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'LEA'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"not")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("not");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'NOT'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'NOT'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'NOT'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"clr")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("clr");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'CLR'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for 'CLR'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'CLR'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"inc")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			next_word(l2,word1);
			strcpy(word2,word1);
			tolowerword(word2,word1);
			while ((strcmp(word1,order))!=0)
			{
				l2+=strlen(word1);
				next_word(l2,word1);
				strcpy(word2,word1);
				tolowerword(word2,word1);
			}
			l2+=strlen(word1)+1;
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'INC'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for 'INC'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'INC'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"dec")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("dec");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'DEC'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'DEC'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'DEC'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"jmp")==0)
		{
		l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("jmp");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'JMP'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'JMP'",line1);
				line1++;
				return 0;
			}

			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'JMP'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"bne")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("bne");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'BNE'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'BNE'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'BNE'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"red")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("red");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'RED'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'RED'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'RED'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"prn")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("prn");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'PRN'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			cost+=cost_switch(case1);
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'PRN'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"jsr")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("jsr");
			if (!(next_word(l2,word)))
			{
				add_error("there are no operands for command 'JSR'",line1);
				line1++;
				return 0;
			}
			case1=method(word);
			switch (case1)
			cost+=cost_switch(case1);
			if (case1==0) {
				add_error("there is no such method for command 'JSR'",line1);
				line1++;
				return 0;
			}
			l2+=strlen(word)+1;
			if (next_word(l2,word))
			{
				add_error("there are too many operands for command 'JSR'",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"rts")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("rts");
			if (next_word(l2,word))
			{
				add_error("there are operands on command 'RTS' which is illegal",line1);
				line1++;
				return 0;
			}
		}
		else if (strcmp(order,"stop")==0)
		{
			l1=strstr(l2,":");
			if (l1!=NULL)
				l2=l1+2;
			l2+=strlen("stop");
			if (next_word(l2,word))
			{
				add_error("there are operands on command 'STOP' which is illegal",line1);
				line1++;
				return 0;
			}
		}
	}
	update_cost(line1,cost);
	return cost;
}
