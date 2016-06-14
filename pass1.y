%{
#include<stdio.h>
#include<string.h>
#include "data_structures.c"
#include<stdlib.h>

FILE *out;

%}

%token START END
%union
{
   int number;
   char* string;
}
%token <string> LABEL
%token <string> OPCODE
%token <number> NUMBER
%token <string> RSUB
%token <string> FORMAT1
%%
S  : START Y X END START   
   ;
Y  : NUMBER                    { 
				  fprintf(out,"%d %s\n",$1,"START");
                                  set_starting_address($1);
			       }
X  : X1 X | X2 X | X3 X | X4 X | X5 X | 
   ;
X1 : LABEL OPCODE LABEL        { 
				 fprintf(out,"%d %s %s\n",LOC,$2,$3);
				 insert($1);
                                 opcode($2);
			       }
X2 : LABEL OPCODE NUMBER       { 
				 fprintf(out,"%d %s %d\n",LOC,$2,$3);
			         insert($1);
                                 opcode_number($2,$3);
			       }
   | LABEL OPCODE '#' NUMBER   {
           			 fprintf(out,"%d %s #%d\n",LOC,$2,$4);
                                 insert($1);
                                 opcode_number($2,$4);
                               }
   ;
X3 : OPCODE LABEL              {
				 fprintf(out,"%d %s %s\n",LOC,$1,$2);
 				 opcode($1);
			       }
X4 : OPCODE NUMBER             {
				 fprintf(out,"%d %s %d\n",LOC,$1,$2); 
				 opcode($1);
			       }
   | OPCODE '#' NUMBER         {
	                         fprintf(out,"%d %s #%d\n",LOC,$1,$3);
				 opcode($1);
			       } 
X5 : RSUB                      {
                                 fprintf(out,"%d %s\n",LOC,$1);
				 opcode($1);
                               }
   | FORMAT1                   {
 				 fprintf(out,"%d %s\n",LOC,$1);
				 opcode($1);
                               }
%%

void yyerror()
{
   printf("Error\n");
   exit(0);
}

main()
{
  out = fopen("output.txt","w");
  file_setup();
  printf("Starting Program Analysis\n");
  yyparse(); 
  fprintf(out,"%d %s",LOC,"END START\n");
  printf("Program is Syntactically Correct\n");
  printf("SYMTAB generated\n");
  print_symtab();
}
