#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "data_structures.c"
#include "convert.c"
#include<math.h>

int n=0,i=0,x=0,b=0,p=0,e=0;
int CUR_LOC,TARGET_LOC;
char buffer[100];
char object_code[10];

FILE *obj;

void find_address_hex(char label[],char label_address[])
{
  char hex[100],buffer[100],*cmd;
  int n;
  FILE *in = fopen("symtab.txt","r");
  while(fgets(buffer,sizeof(buffer),in)!=NULL)
  {
    cmd = strtok(buffer," \n");
    if(strcmp(cmd,label)==0)
    {
      cmd = strtok(NULL," \n");
      label_address[0]='0';
      label_address[1]='0';
      label_address[2] = '\0';
      n = atoi(cmd);
      dec_hex(hex,n);
      strcat(label_address,hex);
      return;
    }
  }
  printf("No such Label %s\n",label);
}

int find_address(char label[])
{
  char hex[100],buffer[100],*cmd;
  FILE *in = fopen("symtab.txt","r");
  while(fgets(buffer,sizeof(buffer),in)!=NULL)
  {
    cmd = strtok(buffer," \n");
    if(strcmp(cmd,label)==0)
    {
      cmd = strtok(NULL," \n");
      return (atoi(cmd));
    }
  }
}

void format4(char opcode[],char op_code[],char label[])
{
   char label_address[20];
   if(strcmp(opcode,"+JSUB")==0)
   {
     strcpy(op_code,"4B1");
     find_address_hex(label,label_address);
   }
   strcat(op_code,label_address);
   printf("%s\n",op_code);
   fprintf(obj,"%s\n",op_code);
}

void format3i(char opcode[],char op_code[],char label[])
{
   char address[4]={'0','0','0','0'};
   int i,k=2,n=strlen(label);
   op_code[1] += 1;
   for(i=0;i<=4-n;i++)
     op_code[k++]='0';
   for(i=1;i<n;i++)
     op_code[k++] = label[i];
   printf("%s\n",op_code);   
   fprintf(obj,"%s\n",op_code);
}

void format3(char opcode[],char op_code[],char label[])
{
   int CUR,dec,k=0,j,n,difference,lim;
   char label_address[20],temp[20],hex_temp[20];
   TARGET_LOC = find_address(label);
   n = check_in_opcode_table(opcode);
   if(n!=0)
   {
     CUR = CUR_LOC;
     CUR_LOC = CUR_LOC + OPCODES[n-1].format;
   }
   difference = TARGET_LOC - CUR_LOC;
   if(difference<0)
   {
     lim = pow(2,12) - 1;
     difference = lim + difference + 1;
   }
   dec_hex(label_address,difference); 
   for(j=0;j<3-strlen(label_address);j++)
      temp[k++] = '0';
   for(j=0;j<strlen(label_address);j++)
      temp[k++] = label_address[j];
   temp[k] = '\0';
   strcpy(label_address,temp);
   dec = hex_dec(op_code);   
   dec_bin(temp,dec);
   if(strcmp(opcode,"STL")==0 || strcmp(opcode,"SUB")==0 || strcmp(opcode,"ADD")==0)
   {
     temp[6]='1';
     temp[7]='1';
     strcat(temp,"0010");
   } 
   bin_hex(temp,hex_temp);
   strcat(hex_temp,label_address);
   strcpy(op_code,hex_temp);
   printf("%s\n",op_code);
   fprintf(obj,"%s\n",op_code);
}

void format2(char opcode[],char op_code[],char label[])
{
  int i,j=0;
  for(i=0;i<2;i++)
  {
    if(label[j]=='A')
      strcat(op_code,"0");
    else if(label[j]=='X')
      strcat(op_code,"1");
    else if(label[j]=='B')
      strcat(op_code,"3");
    else if(label[j]=='S') 
      strcat(op_code,"4");
    else
      printf("Wrong Register %c\n",label[j]);
    j += 2;
  }
  printf("%s\n",op_code);
  fprintf(obj,"%s\n",op_code);
}

void generate_objcode(char opcode[],char op_code[],char label[])
{
  int n  = check_in_opcode_table(opcode);
  if(opcode[0]=='+')
     format4(opcode,op_code,label);
  else if(label[0]=='#')
     format3i(opcode,op_code,label);
  else if(strchr(label,','))
     format2(opcode,op_code,label);
  else if(n!=0) 
    if(OPCODES[n-1].format==3)
      format3(opcode,op_code,label);
}

void command(char opcode[],char label[])
{
  int n;
  char op_code[20]={'\0'};
  n = get_object_code(op_code,opcode);  
  if(n!=0)
     generate_objcode(opcode,op_code,label);
}

main()
{
  FILE *in,*another;
  char *cmd,opcode[10],label[10];
  in = fopen("output.txt","r");
  obj =  fopen("final.txt","w");
  another = fopen("length.txt","w");
  while(fgets(buffer,sizeof(buffer),in)!=NULL)
  {
    cmd=strtok(buffer," \n");
    CUR_LOC = atoi(cmd);
    cmd = strtok(NULL," \n");
    if(strcmp(cmd,"START")==0)
    {
       starting_address = CUR_LOC;
       fprintf(obj,"%d\n",CUR_LOC);
       continue;
    }
    else if(strcmp(cmd,"END")==0)
    {
//      fprintf(obj,"%d %s %s\n",CUR_LOC,"END","START");
      fprintf(another,"%d\n",CUR_LOC - starting_address);
      exit(0);
    }
    if(strcmp(cmd,"RSUB")==0)
    {
       fprintf(obj,"4F0000\n");
       printf("4F0000\n");
    }
    if( strcmp(cmd,"FIX")==0)
    {
       fprintf(obj,"F4\n");
       printf("F4\n");
       continue;
    }
    if( strcmp(cmd,"HIO")==0)
    {
       fprintf(obj,"C4\n");        
       printf("C4\n");
       continue;
    }
    strcpy(opcode,cmd);
    if(strcmp(cmd,"RSUB")!=0)
    {
       cmd = strtok(NULL," \n");
       strcpy(label,cmd);
       command(opcode,label);
    }
  }
}
