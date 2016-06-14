%{
#include "y.tab.h"
#include<stdio.h>
%}

%%
"START"						                	{ return START;  }
"END"					                 		{ return END;    }
[0-9]+                                                                  { 
                                                                          yylval.number = atoi(yytext);
									  return NUMBER;
 									}
(ADD|STL|CLEAR|COMP|DIV|JSUB|COMPR|LDL|MUL|SUB|WORD|RESW|RESB|BYTE)     { 
									  yylval.string = strdup(yytext);		
									  return OPCODE;
									}
"+JSUB"									{
									  yylval.string = strdup(yytext);
									  return OPCODE;
									}
"RSUB" 									{
                                                                          yylval.string = strdup(yytext);
									  return RSUB;
									}
(FIX|HIO)								{
									  yylval.string = strdup(yytext);
		                                                          return FORMAT1;
									}
[,a-zA-Z]+			                       			{ 
							                  yylval.string = strdup(yytext);
									  return LABEL;  
									}
#									{ return yytext[0]; }
(SIC-XE) 								{}
%%

int yywrap()
{
  return 1;
}

void file_setup()
{
  yyin = fopen("input.txt","r");
  yyout = fopen("output.txt","w");
}
