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

#include "commands.h"

char* new_string(int size) { /* return a pointer to a new string with the requested size */
	char*str = (char*)malloc(sizeof(char)*size);
	return str;
}

int is_error(int linenum) {/* check if a specific code line has an error */
	return (error_table[linenum]);
}

void clear_line(int linenum) { /* clear a line on the program code char array */
	memset(lines[linenum], '\0', sizeof(char) * MAX_LINE);
}

void add_error(char *errortext,int linenum)   /* add an item to the entry linked list */
{
	ptr4 temp = error_hptr;
	ptr4 item;
	item = (ptr4)malloc(sizeof(error_list)); 
	if (error_state==0)
		error_state=1;
	if (!item) /* check for memory allocation error */
	{
		printf("\n\nmemory allocation failure\n\n");
		exit(0);
    	}
	if (error_table[linenum]==0) {
		clear_line(linenum); /* clear the line on the command char array - we won't need to process anything on this line, as it is not compatible with guidelines  */
		error_table[linenum]=1; /* update the error table that the current line has an error */
	}
	if (temp) { 
		while((temp->next) && ((temp->line)<linenum) && ((temp->next->line)<=linenum))
			temp = temp->next;
		if ((temp->line)>linenum) { /*if we reached a state where the head node has a larger line number, replace it with the new item */
			error_hptr=item;
			item->next=temp;
		}
		else { /* this is not the head */
			item->next=temp->next;
			temp->next=item;
		}
	}
	else {/* this is the first error in the program, modify the head values */
		error_hptr=item;
		item->next=NULL;
	}
	strcpy(item->text,errortext);  /* insert error text to the new error node */
	item->line=linenum; /* insert line number to the new error node */
	return;
}

int convert2base4(int number){ /* converts a decimal integer to a base 4 integer using recursion */
    if(number == 0)
        return number;
    return (number % 4) + 10*convert2base4(number / 4);
}

void replacechar(char *str, char orig, char rep) { /* replaces a char in a char array with a different char */
    char *temp = str;
    int n = 0;
    while((temp = strchr(temp, orig)) != NULL) {
        *temp++ = rep;
        n++;
    }
}

char *convert2base4w(int num) { /* converts a decimal integer to a weird base 4 number */
	char *base4w;
	int base4num=convert2base4(num); /* convert the given number to a base 4 number */
	base4w = new_string((int)floor(log10(abs(base4num)))+1); /* calcluate the number of digits in the given number */
	sprintf(base4w, "%d", base4num);
	replacechar(base4w,'0','a');
	replacechar(base4w,'1','b');
	replacechar(base4w,'2','c');
	replacechar(base4w,'3','d');
	return base4w;
}

char* base4line(ptr3 lineptr,int linenum) { /* returns a weird base 4 command + data line, ready to be written to a file */
	char* ob_line = new_string(15); /* created a char pointer because we return it at the end of the function */
	int temp,i;
	strcpy(ob_line,convert2base4w(linenum));
	strcat(ob_line,"\t");
	for (i=1;i<10;i+=2) { /* convert the code to base 4 type */
		temp=((lineptr->code)[i])*10+((lineptr->code)[i+1]);
		switch(temp) {
			case 0:
				strcat(ob_line,"a");
				break;
			case 1:
				strcat(ob_line,"b");
				break;
			case 10:
				strcat(ob_line,"c");
				break;
			case 11:
				strcat(ob_line,"d");
		}
	}
	return ob_line;
}

char* base4heading() { /*returns the heading of the ob file, that includes the number of commands and size of data area */
	char* ob_heading = new_string(10); /* created a char pointer because we return it at the end of the function */
	strcpy(ob_heading,convert2base4w(IC-FIRST_ADDRESS)); /* number of commands */
	strcat(ob_heading,"\t"); /* number of commands */
	strcat(ob_heading,convert2base4w(DC)); /* number of commands */
	return ob_heading;
}

void print_errors() { /* print all the errors from the error linked list */
	ptr4 temp=error_hptr;
	if (temp) 
		printf("Errors were found on the given file!\n");
	while (temp) {
	    if ((temp->line)!=0) 
		    printf("Line %d: %s\n",temp->line,temp->text);
	    temp=temp->next;
	}
}	

void add_entry(char *labeltext, int address)  /* add an item to the entry linked list */
{
	ptr5 temp = ent_hptr;
	ptr5 item;
	item = (ptr5)malloc(sizeof(ent_list)); 
	if (!item) /* check for memory allocation error */
	{
		printf("\n\nmemory allocation failure\n\n");
		exit(0);
    	}
	strcpy(item->label,labeltext);  /* copy the label */
   	strcpy(item->code,convert2base4w(address)); /* copy the address in a base-4-weird format */
	item->next=NULL; /* the new item has no next node */
	if (temp==NULL)
		ent_hptr=item;
	else
	{	
		while (temp->next)
			temp = temp->next;
		temp->next=item;
	}
	return;
}

void add_external(char *labeltext, int address)  /* add an item to the externals linked list */
{
	ptr6 temp = ext_hptr;
	ptr6 item;
	item = (ptr6)malloc(sizeof(ent_list)); 
	if (!item) /* check for memory allocation error */
	{
		printf("\n\nmemory allocation failure\n\n");
		exit(0);
    	}
	strcpy(item->label,labeltext);  /* copy the label */
   	strcpy(item->code,convert2base4w(address)); /* copy the address in a base-4-weird format */
	item->next=NULL; /* the new item has no next node */
	if (temp==NULL)
		ext_hptr=item;
	else
	{	
		while (temp->next)
			temp = temp->next;
		temp->next=item;
	}
	return;
}

void add_var(char *label1,int DC1,int external,int action)
{
	ptr p2,item;
	p2=var_hptr;
	if (p2!=NULL) /*if the list is not empty*/
	{
		while (((p2->next)!=NULL)&&((strcmp(p2->label,label1))!=0)) /*while we haven't found the label in the list nor reached the end of the list*/
			p2=p2->next;
		if ((strcmp(p2->label,label1))==0) /*if the lable exists in the list - report an error*/
		{
			add_error("the label already appears in the list",line1);
			return;
		}
	}
	/*if the label doesn't appear in the list*/
	item=(ptr)malloc(sizeof(var_table));
	if (!item)
	{
		printf("\n\nmemory allocation failure\n\n");
		exit(0);
	}
	strcpy(item->label,label1);/*item->label=label1;*/
	item->address=DC1;
	item->ex=external;
	item->ac=action;
	item->next=NULL;
	if (var_hptr==NULL)
		var_hptr=item;
	else
		p2->next=item;
}
void blank_coder(int num1) /*receives the line content, the order given and the number of blank items to be added*/
{
	ptr3 item,p2;
	int i,j,addr1=FIRST_ADDRESS;

	p2=program_hptr;
	while (p2->next) /*adding the item to the end of the linked list*/
	{
		p2=p2->next;
		addr1=p2->code[0];
	}
	addr1++;
	for (i=0;i<num1;i++) /*while there are still blank items needed to be added to the list*/
	{
		item=(ptr3)malloc(sizeof(program_code));
		if (!item)
		{
			printf("\n\nmemory allocation failure!");
			exit(0);
		}
		for (j=1;j<11;j++)
			item->code[j]=0;
		item->code[0]=addr1; /*the item's address gets the next available address*/
		addr1++; /*increases the next available address by one*/
		p2->next=item; /*links the item to the linked list*/
		p2=p2->next;
		item->next=NULL;
	}
}
int check_label(char label[MAX_LABEL])
{
	char *ch;
	if (label==NULL)
		return 0;
	if (!(isalpha(label[0])))
		return 0; /*meaning the first char is not a letter - the label isn't valid!*/
	ch=strstr(label,"[");
	if (ch!=NULL) /*meaning that the word is a matrix definition*/
		return 0;
	if ((strlen(label))>MAX_LABEL)
		return 0; /*meaning the label is too long*/
	return 1; /*meaning the label is valid!*/
}
int is_data(char word[MAX_LINE]) /*checks if line l is a data line or not*/
{	
	if ((strcmp(word,"mov"))==0)		
		return 1;
	else if ((strcmp(word,"cmp"))==0)
		return 1;
	else if ((strcmp(word,"add"))==0)
		return 1;
	else if ((strcmp(word,"sub"))==0)
		return 1;
	else if ((strcmp(word,"not"))==0)
		return 1;
	else if ((strcmp(word,"clr"))==0)
		return 1;
	else if ((strcmp(word,"lea"))==0)
		return 1;
	else if ((strcmp(word,"inc"))==0)
		return 1;	
	else if ((strcmp(word,"dec"))==0)
		return 1;
	else if ((strcmp(word,"jmp"))==0)
		return 1;
	else if ((strcmp(word,"bne"))==0)
		return 1;
	else if ((strcmp(word,"red"))==0)
		return 1;
	else if ((strcmp(word,"prn"))==0)
		return 1;
	else if ((strcmp(word,"jsr"))==0)
		return 1;
	else if ((strcmp(word,"rts"))==0)
		return 1;
	else if ((strcmp(word,"stop"))==0)
		return 1;
	else
		return 0;
}
int is_empty(char l[MAX_LINE])
{
	int i;
	int status=1;
	for (i=0;(l[i]!='\n')&&(l[i]!=EOF);i++) /*passes through the entire content of the line*/
	{
		if ((l[i]!='\t')&&(l[i]!=' ')) /*if we have encountered with a non space char*/
		{
			status=0;
		}
		return status;
	}
	return status;
}
int is_entry(char word[MAX_LABEL])
{
	if ((strcmp(word,".entry"))==0)
		return 1;
	else
		return 0;
}
int is_extern(char word[MAX_LABEL])
{
	if ((strcmp(word,".extern"))==0)
		return 1;
	else
		return 0;
}
int is_instruction(char word[MAX_LABEL]) /*checks if word is an isntruction or not*/
{		
	if ((strcmp(word,".data"))==0) /*if the word after l2 pointer is ".data"*/		
		return 1;
	else if ((strcmp(word,".string"))==0)		
		return 1;
	else if ((strcmp(word,".mat"))==0)	
		return 1;
	else
		return 0;
}
int is_label(char word[MAX_LABEL],char label[MAX_LABEL]) /*checks if the word is a label and if so returns the label itself inside parameter label*/
{
	int len;
	int i;

	label[0]='\0';
	len=strlen(word);
	if ((word[len-1])==':') /*meaning that this is a label declaration*/
	{
		for (i=0;i<len-1;i++)
			label[i]=word[i]; /*stores the label without the ':' inside parameter label*/
		label[i]='\0';
		return 1;
	}
	else
		return 0;
}
int is_note(char l[MAX_LINE])
{
	int status=0;
	int i=0;
	while ((l[i]!=EOF)&&(l[i]!='\n'))
	{
		if ((l[i]==' ')&&(l[i]=='\t'))
		{
			i++;
			continue;
		}
		else if (l[i]==';')
		{
			status=1;
			return status;
		}
		else
			return status;
	}
	return 0;
}
int label_in_table(char label1[MAX_LABEL],int *addr1)
{
	ptr p1;

	p1=var_hptr;
	while (p1)
	{
		if ((strcmp(p1->label,label1))==0)
		{
			(*addr1)=p1->address;
			return 1;
		}
		p1=p1->next;
	}
	*addr1=0;
	return 0;
}
void tolowerword(char word1[MAX_LABEL],char word2[MAX_LABEL])
{
	int i;

	for (i=0;word1[i]!='\0';i++)
		word2[i]=(char)tolower(word1[i]);
	word2[i]='\0';
}
void toupperword(char word1[MAX_LABEL],char word2[MAX_LABEL])
{
	int i;

	for (i=0;word1[i]!='\0';i++)
		word2[i]=(char)toupper(word1[i]);
}
int next_word(char *l2,char *word) /*l2 is a pointer to somewhere inside the line and word is the return value of the next word from l2 pointer*/
{
	char *p2,*p1;
	char c;
	int i=0;

	p2=l2; /*initialize p2 to l2 pointer*/
	c=*p2; /*stores the first char of p2*/
	while ((!(isalpha((int)c)))&&(!(isdigit(c)))&&(c!='.')&&(c!='-')&&(c!='\0')&&(c!='\n')&&(p2!=NULL)&&(c!='#')) /*while we havent reached an alpha char or digit or '.' or the NULL or '\n' - p2 progresses*/
	{
		p2++;
		c=*p2;
	}
	if ((c=='\0')||(c=='\n')||(p2==NULL)) /*meaning there is no next word*/
	{
		*word='\0';
		return 0;
	}
	else /*meaning there is a next word*/
	{
		p1=p2; /*p1 points to the begining of the next word*/
		i=0;
		c=*p2;
		while ((c!=' ')&&(c!='\t')&&(c!='\n')&&(c!=',')&&(c!='\0')) /*while we havent reached the end of the word - p2 progresses*/
		{
			p2++;
			i++;
			c=*p2;
		}
		strncpy(word,p1,i); /*word gets i-1 chars from p1 and on*/
		word[i]='\0';
		return 1;
	}
}
int matrix_register(char word[MAX_LABEL],char *reg1,char *reg2)
{
	char *p1,*p2;
	int length;
	p1=strstr(word,"[");
	p1++;
	p2=strstr(word,"]");
	if ((!p1)||(!p2)) /*meaning there is no first register*/
		return 0;
	length=p2-p1;
	strncpy(reg1,p1,length);
	p1=strstr(p1,"[");
	p1++;
	p2=strstr(p1,"]");
	if ((!p1)||(!p2)) /*meaning there is no second register*/
		return 0;
	length=p2-p1;
	strncpy(reg2,p1,length);
	return 1;
}
int matrix_instruction_nums(char word[MAX_LINE],int *num1,int *num2)
{
	char *p1,*p2;
	int length;
	char n1[MAX_LABEL]={'\0'};
	char n2[MAX_LABEL]={'\0'};
	p1=strstr(word,"[");
	p1++;
	p2=strstr(word,"]");
	if ((!p1)||(!p2)) /*meaning there is no first register*/
		return 0;
	length=strlen(p1)-strlen(p2);
	strncpy(n1,p1,length);
	*num1=atoi(n1);
	p1=strstr(p1,"[");
	p1++;
	p2=strstr(p1,"]");
	if ((!p1)||(!p2)) /*meaning there is no second register*/
		return 0;
	length=p2-p1;
	strncpy(n2,p1,length);
	*num2=atoi(n2);
	return 1;
}
int matrix_label(char word[MAX_LINE],char label[MAX_LABEL])
{
	char *p1,*p2;
	int length;
	char data_word[MAX_LABEL]={'\0'};
	label[0]='\0';
	p2=word;
	if (*p2==' ')
		p2++;
	next_word(word,data_word);
	if (is_data(data_word))
		return 0;
	p1=strstr(word,"["); /*returns a pointer to where '[' appears in word*/
	if (!p1)
		return 0; /*meaning the char '[' doesn't appear in the word and the matrx definition is wrong*/
	length=strlen(p2)-strlen(p1); /*returns the length of the matrix label*/
	if (length==0) 
		return 0;
	strncpy(label,p2,length);
	label[length]='\0';
	if ((strcmp(label,".MAT"))==0)
		return 1;
	if (!(check_label(label)))
		return 0;
	return 1;
}
int method(char word[MAX_LABEL])
{		
	int n;
	int i=0;
	char reg1[3]={'\0'};
	char reg2[3]={'\0'};
	char label1[MAX_LABEL]={'\0'};
	char c;
	if (word[i]=='#') /*method number 0*/
	{
		i++;
		if (word[i]=='-') /*if there is a minus sign before the number*/
			i++;
		else if (word[i]=='+') /*if there is a plus sign before the number*/
			i++;
		if (isdigit(word[i])) /*if there is a number in the word*/
			return 0;
		else /*meaning there is a space between # and the number itself or between the sign and the number*/
		{
			add_error("there is an unnecessary space in the number",line1);
			line1++;
			return 4;
		}
	}
	else if (((char)(tolower(word[i])))=='r') /*if there is a register number - method number 3*/
	{
		i++;
		c=word[i];
		/*n=atoi(c);*/
		n=c-'0';
		if ((n>7)||(n<0)||(!(isdigit(word[i])))) /*meaning the register no. is illegal or there is no number after the 'r'*/
		{
			add_error("there is no such register",line1);
			line1++;
			return 4;
		}
		return 3;
	}
	else if (check_label(word)) /*if the label is valid - method number 1*/
		return 1;
	memset(reg1,'\0',3);
	memset(reg2,'\0',3);
	matrix_label(word,label1); /*label1 gets the matrix label if its a valid label*/
	if (label1[0]!='\0') /*meaning the matrix label exists and is valid - method 2*/
	{
		if (matrix_register(word,reg1,reg2)) /*meaning the registers exist*/
		{
			if (((strcmp(reg1,"r0"))==0)||((strcmp(reg1,"r1"))==0)||((strcmp(reg1,"r2"))==0)||((strcmp(reg1,"r3"))==0)||((strcmp(reg1,"r4"))==0)||((strcmp(reg1,"r5"))==0)||((strcmp(reg1,"r6"))==0)||((strcmp(reg1,"r7"))==0))
			{
				if (((strcmp(reg2,"r0"))==0)||((strcmp(reg2,"r1"))==0)||((strcmp(reg2,"r2"))==0)||((strcmp(reg2,"r3"))==0)||((strcmp(reg2,"r4"))==0)||((strcmp(reg2,"r5"))==0)||((strcmp(reg2,"r6"))==0)||((strcmp(reg2,"r7"))==0))
					return 2;
			}
		}
		else
		{
			line1++;
			return 4;
		}
	}
	else /*there is no such method*/
	{
		add_error("there is no such method",line1);
		line1++;
		return 4;
	}
	return 4;
}
void num_coder(int *a, int n) /*receives an array to write the number n to in binary basis*/
{
	int *p1; /*pointer for the first cell in the array that the function will return*/
	int i=10; /*indicator for the array*/
	int res; /*stores the leftover of the number given after storing part of it in the array*/
	int j; /*indicates the number of 1's in the binary number*/
	int n2;

	p1=a; /*p1 points to the first cell of the array*/
	res=abs(n); /*res gets the absolute number n*/
	for (i=0;i<9;i++,p1++) /*taking care of the last 9 bits from the MSB downwards*/
	{
		if ((res<(pow(2,10-(i+1))))||(res==0)) /*if the cell location represents a higher number than the remaining or that the remaining is 0, then we shall update the array value to 0*/
			*p1=0;
		else /*meaning that the remaining is not 0 and the cell in the array represents a smaller or equal number to the remaining*/
		{
			*p1=1;
			res-=(pow(2,10-(i+1)));
		}
	} /*now we shall take care of the LSB*/
	if ((res<(pow(2,10-(i+1))))||(res==0)) /*if the cell location represents a higher number than the remaining or that the remaining is 0, then we shall update the array value to 0*/
		*p1=0;
	else /*meaning that the remaining is not 0 and the cell in the array represents a smaller or equal number to the remaining*/
	{
		*p1=1;
		res-=(pow(2,10-(i+1)));
	}
	if (n<0) /*if the number is negetive - we shall translate it to binary basis in the 2 complement method*/
	{
		j=0;
		for (i=9;i>0;i--,p1--) /*going through all the array from the LSB to 1 cell before the MSB*/
		{
			if  (j>0) /*if we have already encountered with the first 1 in the binary number*/
			{
				n2=*p1;
				if (n2==1)
					n2=0;
				else
					n2=1;
				*p1=n2;
			}
			if (*p1==1)
				j++;
		}
		if (j>0) /*moving on to the MSB in the array*/
		{
			n2=*p1;
			if (n2==1)
				n2=0;
			else
				n2=1;
			*p1=n2;
		}
	}
}
void instruction_coder(char l[MAX_LINE],char order[MAX_LABEL],char *temp) /*receives the entire line l and the order to perfrom*/
{
	ptr3 p2,item;
	char *l2,*l1;
	char c;
	int n,n1,n2,i;
	int sign=1;
	int a[10]={0};
	int addr1=FIRST_DATA_ADDRESS;
	l2=l;
	if (strcmp(order,".data")==0)
	{
		l1=strstr(l,":");
		if (l1!=NULL)
			l2=l1+2;
		l2+=strlen(".data");
		while (next_word(l2,temp)) /*while there are still values after the .data statement separated by ','*/
		{
			sign=1;
			l2=strstr(l2,temp);
			l2+=strlen(temp)+1;
			if (*temp=='-') /*if the number begins with a minus sign*/
			{
				sign=-1;
				temp++; /*points to the number itself*/
			}
			n=(atoi(temp))*sign;
			item=(ptr3)malloc(sizeof(program_code));
			if (!item)
			{
				printf("\n\nmemory allocation failure\n\n");
				exit(0);
			}
			num_coder(a,n); /*returns the number n in binary array inside a*/
			item->next=NULL;
			p2=data_hptr;
			if (p2==NULL) /*if the list is empty*/
			{
				item->code[0]=FIRST_DATA_ADDRESS;
				data_hptr=item;
			}
			else /*the list is not empty*/
			{
				addr1++;
				while ((p2->next)!=NULL) /*while we haven't reached the last item in the list*/
				{
					addr1++;
					p2=p2->next;	
				}
				item->code[0]=addr1;
				p2->next=item;
			}
			addr1=FIRST_DATA_ADDRESS;
		}
	}
	else if (strcmp(order,".string")==0)
	{
		l1=strstr(l,":");
		if (l1!=NULL)
			l2=l1+2;
		l2+=strlen(".string")+2; /*l2 points to the first quotation mark in the string*/
		while (*l2!='"') /*while we haven't reached the end of the string*/
		{
			c=*l2; /*stores a single char pointed by l2*/
			n=(int)c; /*stores the ascii value of that single char in c*/
			item=(ptr3)malloc(sizeof(program_code));
			if (!item)
			{
				printf("\n\nmemory allocation failure\n\n");
				exit(0);
			}
			num_coder(a,n); /*returns the number n in binary array inside a*/
			for (i=1;i<11;i++)
			{
				item->code[i]=a[i-1];
			} 
			item->next=NULL;
			p2=data_hptr;
			if (p2==NULL) /*if the list is empty*/
			{
				item->code[0]=FIRST_DATA_ADDRESS;
				data_hptr=item;
			}
			else /*the list is not empty*/
			{
				addr1++;
				while ((p2->next)!=NULL)
				{
					addr1++;
					p2=p2->next;	
				}
				item->code[0]=addr1;
				p2->next=item;
			}
			addr1=FIRST_DATA_ADDRESS;
			l2++; /*points to the next char to be entered to the list*/
		}
		item=(ptr3)malloc(sizeof(program_code));
		if (!item)
		{
			printf("\n\nmemory allocation failure\n\n");
			exit(0);
		}
		num_coder(a,0); /*returns the number n in binary array inside a*/
		for (i=1;i<11;i++)
			item->code[i]=a[i-1];
		item->next=NULL;
		p2=data_hptr;
		if (p2==NULL) /*if the list is empty*/
		{
			item->code[0]=FIRST_DATA_ADDRESS;
			data_hptr=item;
		}
		else /*the list is not empty*/
		{
			addr1++;
			while ((p2->next)!=NULL)
			{
				addr1++;
				p2=p2->next;	
			}
			item->code[0]=addr1;
			p2->next=item;
		}	
	}	
	else if (strcmp(order,".mat")==0)
	{
		l1=strstr(l,":");
		if (l1!=NULL)
			l2=l1+2;
		l2+=strlen(".mat");
		if (*l2==' ')
			l2++;
		if (!(matrix_instruction_nums(l2,&n1,&n2))) /*meaning the matrix definition is incorrect*/
		{
			add_error("matrix definition is incorrect",line1);
			line1++;
			return;
		}
		n=n1*n2; /*the number of values expected*/
		l2+=6+(int)floor(log10(abs(n1)))+(int)floor(log10(abs(n2)));
		next_word(l2,temp);
		if (!temp) /*if there are no numbers after the matrix definition - open blank lines*/
		{
			for (i=0;i<n;i++)
			{
				item=(ptr3)malloc(sizeof(program_code));
				if (!item)
				{
					printf("\n\nmemory allocation failure\n\n");
					exit(0);
				}
				for (i=1;i<11;i++) /*stores random zeros in the binary code of the matrix cells*/
					item->code[i]=0;
				item->next=NULL;
				p2=data_hptr;
				if (p2==NULL) /*if the list is empty*/
				{
					item->code[0]=FIRST_DATA_ADDRESS;
					data_hptr=item;
				}
				else /*the list is not empty*/
				{
					addr1++;
					while ((p2->next)!=NULL)
					{
						addr1++;
						p2=p2->next;	
					}
					item->code[0]=addr1;
					p2->next=item;
				}
				addr1=FIRST_DATA_ADDRESS;
			}
		}
		else /*meaning that there are numbers after the matrix definition*/
		{
			while (*temp) /*while there are still numbers after the matrix definition*/
			{
				item=(ptr3)malloc(sizeof(program_code));
				if (!item)
				{
					printf("\n\nmemory allocation failure\n\n");
					exit(0);
				}
				n=atoi(temp);
				num_coder(a,n); /*returns the number n in binary array inside a*/
				for (i=1;i<11;i++)
					item->code[i]=a[i-1];
				item->next=NULL;
				p2=data_hptr;
				if (p2==NULL) /*if the list is empty*/
				{
					item->code[0]=FIRST_DATA_ADDRESS;
					data_hptr=item;
				}
				else /*the list is not empty*/
				{	
					addr1++;
					while ((p2->next)!=NULL)
					{
						addr1++;
						p2=p2->next;	
					}
					item->code[0]=addr1;
					p2->next=item;
				}
				l2+=strlen(temp)+1;
				next_word(l2,temp);
				addr1=FIRST_DATA_ADDRESS;
			}
		}
	}
}
void data_coder(char l[MAX_LINE],char order[MAX_LABEL]) /*codes the action word in binary basis in the program linked list*/
{
	ptr3 p2,item;
	int i,j,m1;
	char *l2;
	char operand[MAX_LABEL]={'\0'};
	char operandup[MAX_LABEL]={'\0'};
	item=(ptr3)malloc(sizeof(program_code));
	if (!item)
	{
		printf("\n\nmemory allocation failure!");
		exit(0);
	}
	item->next=NULL; /*the new item is always added to the end of the list*/ 
	item->code[0]=IC; /*the line address equals the IC counter*/
	if (((strcmp(order,"mov"))==0)||((strcmp(order,"cmp"))==0)||((strcmp(order,"add"))==0)||((strcmp(order,"sub"))==0)||((strcmp(order,"lea"))==0)) /*if this is a 2 operands order*/
	{
		i=5;
		strcpy(operand,order);
		toupperword(operand,operandup);
		l2=strstr(l,operandup);
		if (l2==NULL)
			l2=strstr(l,operand);
		l2+=strlen(operand);
		for (j=0;j<2;j++) /*the two operands are treated here*/
		{
			next_word(l2,operand); /*stores the next operand*/
			l2+=strlen(operand)+1;
			m1=method(operand); /*returns the method of the first operand*/
			switch (m1)
			{
				case 0:
				{
					item->code[i]=0;
					item->code[i+1]=0;
					break;
				}
				case 1:
				{
					item->code[i]=0;
					item->code[i+1]=1;
					break;
				}
				case 2:
				{
					item->code[i]=1;
					item->code[i+1]=0;
					break;
				}
				case 3:
				{
					item->code[i]=1;
					item->code[i+1]=1;
					break;
				}
				case 4:
					break;
			}
			i+=2;
		}
		item->code[i]=0; /*this is an absolute coding so the E,R,A shall be '00'*/
		item->code[i+1]=0;
		if ((strcmp(order,"mov"))==0)
		{
			
			for (i=1;i<5;i++)
				item->code[i]=0;
		}
		else if ((strcmp(order,"cmp"))==0)
		{
			for (i=1;i<4;i++)
				item->code[i]=0;
			item->code[i]=1;
		}
		else if ((strcmp(order,"add"))==0)
		{
			for (i=1;i<3;i++)
				item->code[i]=0;
			item->code[i]=1;
			item->code[i+1]=0;
		}
		else if ((strcmp(order,"sub"))==0)
		{
			for (i=1;i<3;i++)
				item->code[i]=0;
			item->code[i]=1;
			item->code[i+1]=1;
		}
		else if ((strcmp(order,"lea"))==0)
		{
			item->code[1]=0;
			item->code[2]=1;
			item->code[3]=1;
			item->code[4]=0;
		}
	}
	else if (((strcmp(order,"not"))==0)||((strcmp(order,"clr"))==0)||((strcmp(order,"inc"))==0)||((strcmp(order,"dec"))==0)||((strcmp(order,"jmp"))==0)||((strcmp(order,"bne"))==0)||((strcmp(order,"red"))==0)||((strcmp(order,"prn"))==0)||((strcmp(order,"jsr"))==0)) /*one operand orders*/
	{
		i=7;
		strcpy(operand,order);
		toupperword(operand,operandup);	
		l2=strstr(l,operandup);
		if (l2==NULL)
			l2=strstr(l,operand);
		l2+=strlen(operand);
		next_word(l2,operand); /*stores the first operand*/
		m1=method(operand);
		switch (m1)
		{
			case 0:
			{
				item->code[i]=0;
				item->code[i+1]=0;
				break;
			}
			case 1:
			{
				item->code[i]=0;
				item->code[i+1]=1;
				break;
			}
			case 2:
			{
				item->code[i]=1;
				item->code[i+1]=0;
				break;
			}
			case 3:
			{
				item->code[i]=1;
				item->code[i+1]=1;
				break;
			}
			case 4:
				break;
		}
		i=5;
		item->code[i]=0; /*puts '0' due to one operand order*/
		item->code[i+1]=0;
		i=9;
		item->code[i]=0; /*this is an absolute coding so the E,R,A shall be '00'*/
		item->code[i+1]=0;
		if ((strcmp(order,"not"))==0)
		{
			item->code[1]=0;
			item->code[2]=1;
			item->code[3]=0;
			item->code[4]=0;
		}
		else if ((strcmp(order,"clr"))==0)
		{
			item->code[1]=0;
			item->code[2]=1;
			item->code[3]=0;
			item->code[4]=1;
		}
		else if ((strcmp(order,"inc"))==0)
		{
			item->code[1]=0;
			item->code[2]=1;
			item->code[3]=1;
			item->code[4]=1;
		}
		else if ((strcmp(order,"dec"))==0)
		{
			item->code[1]=1;
			item->code[2]=0;
			item->code[3]=0;
			item->code[4]=0;
		}
		else if ((strcmp(order,"jmp"))==0)
		{
			item->code[1]=1;
			item->code[2]=0;
			item->code[3]=0;
			item->code[4]=1;
		}
		else if ((strcmp(order,"bne"))==0)
		{
			item->code[1]=1;
			item->code[2]=0;
			item->code[3]=1;
			item->code[4]=0;
		}
		else if ((strcmp(order,"red"))==0)
		{
			item->code[1]=1;
			item->code[2]=0;
			item->code[3]=1;
			item->code[4]=1;
		}
		else if ((strcmp(order,"prn"))==0)
		{
			item->code[1]=1;
			item->code[2]=1;
			item->code[3]=0;
			item->code[4]=0;
		}
		else if ((strcmp(order,"jsr"))==0)
		{
			item->code[1]=1;
			item->code[2]=1;
			item->code[3]=0;
			item->code[4]=1;
		}
	}
	else /*meaning the order can be either rts or stop*/
	{
		for (i=5;i<11;i++)
			item->code[i]=0;
		if ((strcmp(order,"rts"))==0)
		{
			item->code[1]=1;
			item->code[2]=1;
			item->code[3]=1;
			item->code[4]=0;
		}
		else /*meaning the order is stop*/
		{
			item->code[1]=1;
			item->code[2]=1;
			item->code[3]=1;
			item->code[4]=1;
		}
	}
	p2=program_hptr;
	if (p2==NULL) /*meaning the linked list was empty*/
		program_hptr=item;
	else /*meaning the linked list is not empty*/
	{
		while (p2->next) /*adding the item to the end of the linked list*/
			p2=p2->next;
		p2->next=item;
	}
}
void value_coder(char l[MAX_LINE],char order[MAX_LABEL])
{
	ptr3 p2; /*pointer for the binary code head of linked list*/
	char *l2; /*pointer to the line given*/
	char operand[MAX_LABEL]={'\0'}; /*the next word from the line*/
	char operandup[MAX_LABEL]={'\0'}; /*the next word from the line in uppercase*/
	int sign=1; /*the sign of the number given in the line*/
	int n; /*stores the number from the given order*/
	int a[10]={0}; /*stores the binary number to write into the list*/
	int j; /*indicator for loop number*/
	int i; /*indicator for the binary number location*/
	char reg1[3]={'\0'}; /*char array for register number 1 for the matrix*/
	char reg2[3]={'\0'}; /*char array for register number 2 for the matrix*/
	int reg11, reg22; /*the register numbers in integers*/
	int register_state=0; /*the number of registers in the line - method 3*/
	char c; /*stores the register number */
	int m1; /*stores the method of the word given after the order*/
	char *l1; /*pointer for the number in case of method 0*/

	p2=program_hptr; /*pointer points to the head of the program linked list - binary code*/
	while (p2->code[0]!=IC) /*while we haven't reached the item representing the order given*/
		p2=p2->next;
	strcpy(operand,order);
	toupperword(operand,operandup);
	l2=strstr(l,operandup);
	if (l2==NULL)
		l2=strstr(l,operand);
	l2+=strlen(operand); /*l2 now points to the first space after the order given*/
	if (((strcmp(order,"mov"))==0)||((strcmp(order,"cmp"))==0)||((strcmp(order,"add"))==0)||((strcmp(order,"sub"))==0)||((strcmp(order,"lea"))==0)) /*if this is a 2 operands order*/
	{
		for (j=0;j<2;j++) /*the two operands are treated here*/
		{
			next_word(l2,operand); /*stores the next operand*/
			l2+=strlen(operand)+1;
			m1=method(operand); /*returns the method of the first operand*/
			switch (m1)
			{
				case 0:
				{
					p2=p2->next;
					if (operand[1]=='-')
						sign=-1;
					l1=&operand[2];
					n=atoi(l1)*sign;
					num_coder(a,n);
					for (i=1;i<9;i++) /*the last 8 bits are encoded into the item until the LSB*/
						p2->code[i]=a[i+2];
					p2->code[9]=0; /*E,R,A in case of number is absolute - 00*/
					p2->code[10]=0; /*E,R,A in case of number is absolute - 00*/
					break;
				}
				case 1:
				{
					p2=p2->next;
					break;
				}
				case 2:
				{
					p2=p2->next->next; /*the first empty item belongs to the matrix label address which will be added in the second transition*/
					matrix_register(operand,reg1,reg2); /*returns the matrix register numbers*/
					l1=&reg1[1];
					reg11=atoi(l1);
					l1=&reg2[1];
					reg22=atoi(l1);
					num_coder(a,reg11);
					for (i=1;i<5;i++) 
						p2->code[i]=a[i+5]; /*only the last 4 digits until the LSB are coded*/
					num_coder(a,reg22);
					for (i=5;i<9;i++)
						p2->code[i]=a[i+1]; /*only the last 4 digits until the LSB are coded*/
					p2->code[9]=0; /*E,R,A in case of registers is 00*/
					p2->code[10]=0; /*E,R,A in case of registers is 00*/
					break;
				}
				case 3:
				{
					c=operand[1]; /*stores the char of the register number*/
					n=c-'0'; /*stores the number of the register*/
					num_coder(a,n);
					if (register_state==0)
					{
						p2=p2->next;
						for (i=5;i<9;i++)
							p2->code[i]=a[i+1]; /*only the last 4 digits until the LSB are coded*/
					}
					else
					{
						for (i=1;i<5;i++)
							p2->code[i]=a[i+5]; /*only the last 4 digits until the LSB are coded*/
					}
					p2->code[9]=0; /*E,R,A in case of registers is 00*/
					p2->code[10]=0; /*E,R,A in case of registers is 00*/
					register_state++; /*flag for registers presence in the line*/
					break;
				}
				case 4:
					break;
			}
		}
	}
	else if (((strcmp(order,"not"))==0)||((strcmp(order,"clr"))==0)||((strcmp(order,"inc"))==0)||((strcmp(order,"dec"))==0)||((strcmp(order,"jmp"))==0)||((strcmp(order,"bne"))==0)||((strcmp(order,"red"))==0)||((strcmp(order,"prn"))==0)||((strcmp(order,"jsr"))==0)) /*one operand orders*/
	{
		strcpy(operand,order);
		toupperword(operand,operandup);	
		l2=strstr(l,operandup);
		if (l2==NULL)
			l2=strstr(l,operand);
		l2+=strlen(operand);
		next_word(l2,operand); /*stores the first operand*/
		m1=method(operand);
		switch (m1)
		{
			case 0:
			{
				p2=p2->next;
				if (operand[1]=='-')
					sign=-1;
				l1=&operand[2];
				n=atoi(l1)*sign;
				num_coder(a,n);
				for (i=1;i<9;i++)
					p2->code[i]=a[i+1]; /*the last 8 bits are encoded into the item until the LSB*/
				p2->code[9]=0; /*E,R,A in case of number is absolute - 00*/
				p2->code[10]=0; /*E,R,A in case of number is absolute - 00*/
				break;
			}
			case 1:
			{
				p2=p2->next;
				break;
			}
			case 2:
			{
				p2=p2->next->next; /*the first empty item belongs to the matrix label address which will be added in the second transition*/
				matrix_register(operand,reg1,reg2); /*returns the matrix register numbers*/
				l1=&reg1[1];
				reg11=atoi(l1);
				l1=&reg2[1];
				reg22=atoi(l1);
				num_coder(a,reg11);
				for (i=1;i<5;i++)
					p2->code[i]=a[i+5]; /*only the last 4 digits until the LSB are coded*/
				num_coder(a,reg22);
				for (i=5;i<9;i++)
					p2->code[i]=a[i+1]; /*only the last 4 digits until the LSB are coded*/
				p2->code[9]=0; /*E,R,A in case of registers is 00*/
				p2->code[10]=0; /*E,R,A in case of registers is 00*/
				break;
			}
			case 3:
			{
				c=operand[1]; /*stores the char of the register number*/
				n=c-'0'; /*stores the number of the register*/
				num_coder(a,n);
				for (i=1;i<9;i++)
					p2->code[i]=a[i-1]; 
				p2->code[9]=0; /*E,R,A in case of registers is 00*/
				p2->code[10]=0; /*E,R,A in case of registers is 00*/
				break;
			}
			case 4:
				break;
		}
	}
	else /*meaning the order can be either rts or stop - nothing to do!*/
		return;
}
