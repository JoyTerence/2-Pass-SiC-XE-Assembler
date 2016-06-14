#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define OPCODE_NUMBER 16

static int LOC;
static int program_length;
static int starting_address;
static int symtab_size = 1;

const struct OPCODE
{
  char name[7];
  char code[2];
  int format;
}

OPCODES[] = {
 { "ADD", "18", 3 }, { "CLEAR", "B4", 2 }, { "COMP", "28", 3 },
 { "DIV", "24", 3 }, { "JSUB" , "48", 3 }, { "LDA" , "00", 3 },
 { "LDL", "8" , 3 }, { "LDCH" , "50", 3 }, { "MUL" , "20", 3 },
 { "SUB", "1C", 3 }, { "+JSUB", "48", 4 }, { "RSUB", "4C", 3 },
 { "COMPR","A0",2 }, { "STL"  , "17", 3 }, { "FIX" , "C4", 1 },
 { "HIO", "F4", 1 }
};

static struct table
{
  char name[7];
  int address;
  int type;
  int length;
}SYMTAB[1000];

void insert(char lexeme[])
{
   int i;
   for(i=0;i<symtab_size;i++)
   {
      if(strcmp(SYMTAB[i].name,lexeme)==0)
      {
         printf("Duplicate Label : %s\n",lexeme);
         exit(0);
      }
   }
   strcpy(SYMTAB[symtab_size-1].name,lexeme);
   SYMTAB[symtab_size-1].address = LOC;
   symtab_size++;
}

void print_symtab()
{
  FILE *s;
  s = fopen("symtab.txt","w");
  int i;
  for(i=0;i<symtab_size-1;i++)
  {
     fprintf(s,"%s %d\n",SYMTAB[i].name,SYMTAB[i].address);
     printf("%s\t%d\n",SYMTAB[i].name,SYMTAB[i].address);
  }
}

void set_starting_address(int a)
{
  LOC = a;
  starting_address = LOC;
}

int check_in_opcode_table(char *token)
{
  int i;
  for(i=0;i<OPCODE_NUMBER;i++)
    if(strcmp(token,OPCODES[i].name)==0)
      return i+1;
  return 0;
}

void opcode(char *token)
{
  int n = check_in_opcode_table(token);
  if(n!=0)
    LOC += OPCODES[n-1].format;
  else if(n==0)
    printf("Unknown Opcode %s\n",token);
}

void opcode_number(char *token,int number)
{
  int flag = 0;
  int n = check_in_opcode_table(token);
  if(strcmp(token,"WORD")==0)
  {
    LOC += 3;
    flag = 1;
  }
  else if(strcmp(token,"RESW")==0)
  {
    LOC += 3*number;
    flag = 1;
  }
  else if(strcmp(token,"RESB")==0)
  {
    LOC += number;
    flag = 1;
  }
  else if(strcmp(token,"BYTE")==0)
  {
    LOC += number;
  }
  else
    LOC += OPCODES[n-1].format;
  if(n==0 && flag==0)
    printf("Unknown Opcode %s\n",token);
}

int get_object_code(char op_code[],char opcode[])
{
  int n = check_in_opcode_table(opcode);
  if(n!=0)
    strcpy(op_code,OPCODES[n-1].code);
  return n; 
}

