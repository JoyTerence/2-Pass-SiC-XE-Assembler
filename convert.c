#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

void dec_hex(char hex[],int decimal_number)
{
  long decimalnum,remainder,quotient;
  int i=0,j=0,temp;
  char hex1[10];
  quotient = decimal_number;
  while(quotient != 0)
  { 
    temp = quotient % 16;
    if(temp < 10)
      temp += 48;
    else
      temp += 55;
    hex1[i++] = temp;
    quotient /= 16;
  }
  hex1[i]='\0';
  for(i=strlen(hex1)-1,j=0;i>=0;i--,j++)
    hex[j] = hex1[i];
  hex[j] = '\0';
//  printf("%s\n",hex);
} 

int hex_dec(char buffer[])
{
  int x = strtol(buffer,0,16);
  return x;
}

void dec_bin(char dec[],int n)
{
   int rem,i,j,binary =0,k=0;
   char token[10];
   while(n!=0)
   {
     rem = n%2;
     if(rem)
       token[k++] = '1';
     else
       token[k++] = '0';
     n /= 2;
   }
   token[k] = '\0';
   while( k!= 8)
      token[k++] = '0';
   token[k] = '\0';
   for(i=strlen(token)-1,j=0;i>=0;i--,j++)
      dec[j] = token[i];
   dec[j] = '\0';
}

int bin_dec(int n)
{
  int decimal =0,i=0,rem;
  while(n!=0)
  {  
    rem = n%10;
    n /= 10;
    decimal += rem*pow(2,i);
    ++i;
  }
  return decimal;
}

void bin_hex(char bin[],char hex[])
{
  int temp;
  char t;
  long int i=0,j=0;
  while( bin[i])
  {
    bin[i] -= 48;
    ++i;
  }
  --i;
  while(i-2 >= 0)
  {
    temp = bin[i-3] *8 + bin[i-2] *4 + bin[i-1] *2 + bin[i];
    if (temp > 9)
       hex[j++] = temp + 55;
    else
       hex[j++] = temp + 48;
    i = i - 4;  
  }
  if( i==1 )
    hex[j] = bin[i-1] * 2 + bin[i] + 48;
  else if(i==0)
    hex[j] = bin[i] + 48;
  else 
    j--;
  hex[++j]='\0';
  for(i=0;i<j/2;i++)
  {
    t = hex[i];
    hex[i] = hex[j-i-1];
    hex[j-i-1] = t;
  }
}
/*
main()
{
  char temp[100] = "0001100100000101";
  char hex[100];
  bin_hex(temp,hex);
  printf("%s\n",hex);
}
*/
