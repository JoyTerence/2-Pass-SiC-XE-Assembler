#include<stdio.h>
#include<string.h>
#include "convert.c"

void make6bits(char hex[])
{
  char buffer[10];
  int i,j=0;
  for(i=0;i<6-strlen(hex);i++)
    buffer[j++]='0';
  for(i=0;i<strlen(hex);i++)
    buffer[j++]=hex[i];
  strcpy(hex,buffer);
}

main()
{
  FILE *in = fopen("final.txt","r");
  FILE *another = fopen("length.txt","r");
  FILE *out = fopen("listing.txt","w");
  char *cmd,buffer[100],hex[100],hex1[100],buf[100],list[100][100];
  int i=0,j,first=1,number,address,length,total_length=0;
  while( fgets(buffer,sizeof(buffer),in)!= NULL)
  {
    cmd = strtok(buffer," \n");
    if(first)
    {
      number = atoi(cmd);
      fgets(buf,sizeof(buf),another);
      length = atoi(buf);
      address = number;
      dec_hex(hex,address);
      dec_hex(hex1,length);
      make6bits(hex);
      make6bits(hex1);
      fprintf(out,"H^SIC-XE^%s^%s\n",hex,hex1);      
      first = 0;
      continue;
    }  
    strcpy(list[i++],cmd);
  }
  for(j=0;j<i;j++)
    total_length += strlen(list[j]);
  dec_hex(hex,total_length);
  dec_hex(hex1,address);
  make6bits(hex1);
  fprintf(out,"T^%s^%s",hex1,hex);
  for(j=0;j<i;j++)
    fprintf(out,"^%s",list[j]);
  fprintf(out,"\n");
  dec_hex(hex,address);
  make6bits(hex);
  fprintf(out,"E^%s\n",hex);
} 
