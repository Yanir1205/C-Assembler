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
#include "file_to_array.h"

extern void add_error(char [],int);

int file_to_array(FILE *fp) { /*recieves a pointer to a file and insert the file's lines to an array + remove whitespaces */
	int i=1,j=0; /* counters to run thru the char array */
	char tempchar;
	enum braketval { NONE, FIRSTOPEN, FIRSTCLOSE, SECONDOPEN, SECONDCLOSE };
	/* enum to know which state are the brakets on the specific line */
	enum charval { WHITESPACE, TAB, EOL, SEMICOLON, REGULAR, POINTS, THERMAL, BRAKET, COLON };
	/* enum to know what was the previous character " "=0, \t=1, \n=2, ;=3, char=4, :=5, #=6, []=7, ,=8 */
	enum charval chartype=2; /* assume we start on end of line, so we won't put space in the begining of line */
	enum braketval braket=0; /* no brakets seen yet */
	while ((tempchar = (char)fgetc(fp)) != EOF) { /* read one character at a time, until we reach EOF */
		switch((int)tempchar) {
			case '\n' :
				switch(chartype) {
					case 5 :
						add_error("Usage of end of line after points is illegal",i);
						break;
					case 6 :
						add_error("Usage of end of line after thermal is illegal",i);
						break;
					case 7 :
						if ((braket>0) && (braket<4)) 
							add_error("Usage of end of line without closing both brakets is illegal",i);
						break;
					case 8 :
						add_error("Usage of end of line after a colon is illegal",i);
						break;
					default :
						break;
				}
				if (is_error(i))
					clear_line(i); /* clear the line on the command char array - we won't need to process anything on this line, as it is not compatible with guidelines  */
				if (chartype!=2) {
					chartype=2;
					braket=0;
					lines[i][j]='\n';
					j=0;
					i++;
				}
				break;
			case ';' : /* semicolon - comment, we don't insert it to the array */
				if (chartype>3) /*if the previous character is a regular character this is an error*/
					add_error("Usage of semicolon after character other than space or tab is illegal",i);
				chartype=3;
				break;
			case ' ' : /* space, we insert only one */
				if (((chartype>3) && (chartype<6)) || ((chartype==7) && (braket==4))){
						lines[i][j]=' ';
						j++;
						chartype=0;
				}
				else if (chartype==2)
					chartype=0;
				else if (chartype==0)
					chartype=0;
				else if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				break;
			case '\t' : /* tab, we insert only one space instead */
				if (((chartype>3) && (chartype<6)) || ((chartype==7) && (braket==4))){
						lines[i][j]=' ';
						j++;
						chartype=1;
				}
				else if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				break;
			case ':' :
				if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else if (chartype==4) {
						lines[i][j]=':';
						j++;
				}
				else
					add_error("Usage of points after charater other than letters or numbers is illegal",i);
				chartype=5;
				break;
			case '#' :
				if (chartype < 2) {
					lines[i][j]='#';
					j++;
				}
				else if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else
					add_error("Usage of thermal after charater other than space or tab is illegal",i);
				chartype=6;
				break;
			case '[' :
				if ((chartype <2) || (chartype==4) || (chartype==7)) {
					if (braket == 0) {
						braket=1;
						lines[i][j]='[';
						j++;
					}
					else if (braket == 1) 
						add_error("Opening a second braket without closing the first one is illegal",i);
					else if (braket == 2)  {
						braket=3;
						lines[i][j]='[';
						j++;
					}
					else { 
						if (braket == 3) 
							add_error("Opening another braket without closing the second one is illegal",i);
						add_error("Opening a third braket is illegal",i); /* will work on both cases, braket 3+4 */
					}
					chartype=7;
				}
				else if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else	{
					add_error("Using braket not after tab, space or another braket is illegal",i);
				}
				break;
			case ']' : 
				if ((chartype <2) || (chartype==4) || (chartype==7)) {
					if (braket == 0) {
						add_error("Closing a braket without opening the first one is illegal",i);
					}
					else if (braket == 1)  {
						if (chartype==7) {
							add_error("Closing a braket without defining any values inside is illegal",i);
						}
						else {
							lines[i][j]=']';
							j++;
						}
						braket=2;
					}
					else if (braket == 2)
						add_error("Closing a braket without opening the second one is illegal",i);
					else if (braket == 3) {
						if (chartype==7) {
							add_error("Closing a braket without defining any values inside is illegal",i);
						}
						else {
							lines[i][j]=']';
							j++;
						}
						braket=4;		
					}
					else if (braket == 4)
						add_error("Closing a third braket is illegal",i);
					chartype=7;
				}
				else if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else
					add_error("Using braket not after text, tab, space or another braket is illegal",i);
				break;
			case ',' :
				if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else if ((chartype==4) || (chartype<2)|| (chartype==7)) {
						lines[i][j]=',';
						j++;
				}
				else
					add_error("Using colon not after text, tab or space  is illegal",i);
				break;				
			default : /* any other character - put the char in the array */
				if (is_error(i))
					clear_line(i); /* clear the line on the command char array - we won't need to process anything on this line, as it is not compatible with guidelines  */
				if (isdigit(tempchar))
					if (chartype==2)
						add_error("Using a digit at start of line is illegal",i);
				if (chartype==3); /* if there's a semicolon before it's fine, we don't insert the comment to the array */
				else {
					chartype=4;
					lines[i][j]=tempchar;
					j++;
				}
		}
	}
	if ((is_error(i)) && (chartype!=2)) /* if the file ends with EOF but without /n before, we need to check if the line has an error and clear the line */
		clear_line(i); /* clear the line on the command char array - we won't need to process anything on this line, as it is not compatible with guidelines  */
	return i;
}
